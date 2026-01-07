#include "hough_common.h"
#include "hough_line.h"
#include "hough_circle.h"
#include "image_processor.h"
#include <mpi.h>
#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>

// Forward declarations
cv::Mat mpi_to_grayscale(const cv::Mat& image, int rank, int size);
cv::Mat mpi_detect_edges(const cv::Mat& grayscale, int rank, int size,
                         double low_threshold, double high_threshold);
HoughLineDetector* create_mpi_line_detector(int width, int height, int rank, int size,
                                           double rho_res, double theta_res, int thresh);
HoughCircleDetector* create_mpi_circle_detector(int width, int height, int rank, int size,
                                                int min_r, int max_r, int thresh);

struct Config {
    std::string input_file;
    std::string output_dir;
    std::string mode;  // "lines", "circles", "both"
    int threshold;
    int min_radius;
    int max_radius;
    double low_threshold;
    double high_threshold;
    
    Config() : mode("both"), threshold(100), min_radius(10), max_radius(100),
               low_threshold(50), high_threshold(150) {}
};

bool parse_arguments(int argc, char* argv[], Config& config) {
    if (argc < 3) {
        std::cerr << "Usage: mpirun -np <num_processes> " << argv[0] 
                  << " <input_image> <output_dir> [options]" << std::endl;
        std::cerr << "Options:" << std::endl;
        std::cerr << "  --mode <lines|circles|both>  Detection mode (default: both)" << std::endl;
        std::cerr << "  --threshold <value>      Detection threshold (default: 100)" << std::endl;
        std::cerr << "  --min-radius <value>     Minimum circle radius (default: 10)" << std::endl;
        std::cerr << "  --max-radius <value>     Maximum circle radius (default: 100)" << std::endl;
        return false;
    }
    
    config.input_file = argv[1];
    config.output_dir = argv[2];
    
    for (int i = 3; i < argc; i += 2) {
        std::string arg = argv[i];
        if (i + 1 >= argc) break;
        
        if (arg == "--mode") {
            config.mode = argv[i + 1];
        } else if (arg == "--threshold") {
            config.threshold = std::stoi(argv[i + 1]);
        } else if (arg == "--min-radius") {
            config.min_radius = std::stoi(argv[i + 1]);
        } else if (arg == "--max-radius") {
            config.max_radius = std::stoi(argv[i + 1]);
        }
    }
    
    return true;
}

void save_timing(const std::string& filename, int num_processes, 
                double preprocessing_time, double hough_time, double total_time) {
    std::ofstream file(filename);
    file << "Performance Metrics (MPI Implementation)" << std::endl;
    file << "=========================================" << std::endl;
    file << "Number of processes: " << num_processes << std::endl;
    file << "Preprocessing time: " << preprocessing_time << " ms" << std::endl;
    file << "Hough transform time: " << hough_time << " ms" << std::endl;
    file << "Total time: " << total_time << " ms" << std::endl;
    file.close();
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    Config config;
    bool valid_args = true;
    
    if (rank == 0) {
        valid_args = parse_arguments(argc, argv, config);
    }
    
    // Broadcast validity
    MPI_Bcast(&valid_args, 1, MPI_C_BOOL, 0, MPI_COMM_WORLD);
    
    if (!valid_args) {
        MPI_Finalize();
        return 1;
    }
    
    // Broadcast configuration
    int input_len = config.input_file.length();
    int output_len = config.output_dir.length();
    int mode_len = config.mode.length();
    
    MPI_Bcast(&input_len, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&output_len, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&mode_len, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    if (rank != 0) {
        config.input_file.resize(input_len);
        config.output_dir.resize(output_len);
        config.mode.resize(mode_len);
    }
    
    MPI_Bcast(const_cast<char*>(config.input_file.data()), input_len, MPI_CHAR, 0, MPI_COMM_WORLD);
    MPI_Bcast(const_cast<char*>(config.output_dir.data()), output_len, MPI_CHAR, 0, MPI_COMM_WORLD);
    MPI_Bcast(const_cast<char*>(config.mode.data()), mode_len, MPI_CHAR, 0, MPI_COMM_WORLD);
    MPI_Bcast(&config.threshold, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&config.min_radius, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&config.max_radius, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    if (rank == 0) {
        std::cout << "MPI Hough Transform" << std::endl;
        std::cout << "==================" << std::endl;
        std::cout << "Input: " << config.input_file << std::endl;
        std::cout << "Output: " << config.output_dir << std::endl;
        std::cout << "Processes: " << size << std::endl;
        std::cout << "Mode: " << config.mode << std::endl;
        
        // Create output directory
        std::filesystem::create_directories(config.output_dir);
    }
    
    double start_time = HoughUtils::get_time_ms();
    
    // Load image (master only)
    cv::Mat image;
    int img_rows = 0, img_cols = 0;
    
    if (rank == 0) {
        image = ImageProcessor::load_image(config.input_file);
        if (!ImageProcessor::is_valid_image(image)) {
            std::cerr << "Error: Failed to load image" << std::endl;
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        img_rows = image.rows;
        img_cols = image.cols;
        std::cout << "Image size: " << img_cols << "x" << img_rows << std::endl;
    }
    
    // Broadcast image dimensions
    MPI_Bcast(&img_rows, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&img_cols, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    // Preprocessing with MPI parallelization
    double preprocess_start = HoughUtils::get_time_ms();
    
    if (rank == 0) std::cout << "Converting to grayscale..." << std::endl;
    cv::Mat grayscale = mpi_to_grayscale(image, rank, size);
    
    if (rank == 0) std::cout << "Detecting edges..." << std::endl;
    cv::Mat edges = mpi_detect_edges(grayscale, rank, size,
                                     config.low_threshold, config.high_threshold);
    
    double preprocess_end = HoughUtils::get_time_ms();
    double preprocess_time = preprocess_end - preprocess_start;
    
    // Save edge image (master only)
    if (rank == 0) {
        ImageProcessor::save_image(config.output_dir + "/edges.png", edges);
    }
    
    // Prepare visualization (master only)
    cv::Mat result;
    if (rank == 0) {
        result = ImageProcessor::prepare_visualization(image);
    }
    
    // Hough transform
    double hough_start = HoughUtils::get_time_ms();
    
    if (config.mode == "lines" || config.mode == "both") {
        if (rank == 0) std::cout << "Detecting lines..." << std::endl;
        
        HoughLineDetector* line_detector = create_mpi_line_detector(
            img_cols, img_rows, rank, size,
            1.0, M_PI / 180.0, config.threshold);
        
        std::vector<HoughLine> lines = line_detector->detect(edges);
        delete line_detector;
        
        if (rank == 0) {
            std::cout << "Found " << lines.size() << " lines" << std::endl;
            HoughUtils::draw_lines(result, lines);
            HoughUtils::save_lines_to_file(config.output_dir + "/lines.txt", lines);
        }
    }
    
    if (config.mode == "circles" || config.mode == "both") {
        if (rank == 0) std::cout << "Detecting circles..." << std::endl;
        
        HoughCircleDetector* circle_detector = create_mpi_circle_detector(
            img_cols, img_rows, rank, size,
            config.min_radius, config.max_radius, config.threshold);
        
        std::vector<HoughCircle> circles = circle_detector->detect(edges);
        delete circle_detector;
        
        if (rank == 0) {
            std::cout << "Found " << circles.size() << " circles" << std::endl;
            HoughUtils::draw_circles(result, circles);
            HoughUtils::save_circles_to_file(config.output_dir + "/circles.txt", circles);
        }
    }
    
    double hough_end = HoughUtils::get_time_ms();
    double hough_time = hough_end - hough_start;
    
    // Save result image (master only)
    if (rank == 0) {
        ImageProcessor::save_image(config.output_dir + "/result.png", result);
        
        double total_time = HoughUtils::get_time_ms() - start_time;
        
        // Save timing
        save_timing(config.output_dir + "/timing.txt", size, 
                   preprocess_time, hough_time, total_time);
        
        std::cout << "\nPerformance:" << std::endl;
        std::cout << "  Preprocessing: " << preprocess_time << " ms" << std::endl;
        std::cout << "  Hough transform: " << hough_time << " ms" << std::endl;
        std::cout << "  Total: " << total_time << " ms" << std::endl;
        std::cout << "\nResults saved to " << config.output_dir << std::endl;
    }
    
    MPI_Finalize();
    return 0;
}


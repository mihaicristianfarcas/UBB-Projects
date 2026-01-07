#include "hough_common.h"
#include "hough_line.h"
#include "image_processor.h"
#include <iostream>
#include <string>
#include <thread>
#include <fstream>
#include <filesystem>

// Forward declarations
cv::Mat parallel_to_grayscale(const cv::Mat& image, int num_threads);
cv::Mat parallel_detect_edges(const cv::Mat& grayscale, int num_threads, 
                               double low_threshold, double high_threshold);
HoughLineDetector* create_threaded_line_detector(int width, int height, int num_threads,
                                                double rho_res, double theta_res, int thresh);

struct Config {
    std::string input_file;
    std::string output_dir;
    int num_threads;
    int threshold;
    double low_threshold;
    double high_threshold;
    
    Config() : num_threads(std::thread::hardware_concurrency()),
               threshold(100), low_threshold(50), high_threshold(150) {}
};

bool parse_arguments(int argc, char* argv[], Config& config) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <input_image> <output_dir> [options]" << std::endl;
        std::cerr << "Options:" << std::endl;
        std::cerr << "  --threads <num>          Number of threads (default: hardware concurrency)" << std::endl;
        std::cerr << "  --threshold <value>      Detection threshold (default: 100)" << std::endl;
        return false;
    }
    
    config.input_file = argv[1];
    config.output_dir = argv[2];
    
    for (int i = 3; i < argc; i += 2) {
        std::string arg = argv[i];
        if (i + 1 >= argc) break;
        
        if (arg == "--threads") {
            config.num_threads = std::stoi(argv[i + 1]);
        } else if (arg == "--threshold") {
            config.threshold = std::stoi(argv[i + 1]);
        }
    }
    
    return true;
}

void save_timing(const std::string& filename, double preprocessing_time, 
                double hough_time, double total_time) {
    std::ofstream file(filename);
    file << "Performance Metrics (Threaded Implementation)" << std::endl;
    file << "==============================================" << std::endl;
    file << "Preprocessing time: " << preprocessing_time << " ms" << std::endl;
    file << "Hough transform time: " << hough_time << " ms" << std::endl;
    file << "Total time: " << total_time << " ms" << std::endl;
    file.close();
}

int main(int argc, char* argv[]) {
    Config config;
    if (!parse_arguments(argc, argv, config)) {
        return 1;
    }
    
    std::cout << "Threaded Hough Transform" << std::endl;
    std::cout << "========================" << std::endl;
    std::cout << "Input: " << config.input_file << std::endl;
    std::cout << "Output: " << config.output_dir << std::endl;
    std::cout << "Threads: " << config.num_threads << std::endl;
    
    // Create output directory
    std::filesystem::create_directories(config.output_dir);
    
    double start_time = HoughUtils::get_time_ms();
    
    // Load image
    cv::Mat image = ImageProcessor::load_image(config.input_file);
    if (!ImageProcessor::is_valid_image(image)) {
        std::cerr << "Error: Failed to load image" << std::endl;
        return 1;
    }
    
    std::cout << "Image size: " << image.cols << "x" << image.rows << std::endl;
    
    // Preprocessing with parallelization
    double preprocess_start = HoughUtils::get_time_ms();
    
    std::cout << "Converting to grayscale..." << std::endl;
    cv::Mat grayscale = parallel_to_grayscale(image, config.num_threads);
    
    std::cout << "Detecting edges..." << std::endl;
    cv::Mat edges = parallel_detect_edges(grayscale, config.num_threads, 
                                         config.low_threshold, config.high_threshold);
    
    double preprocess_end = HoughUtils::get_time_ms();
    double preprocess_time = preprocess_end - preprocess_start;
    
    // Save edge image
    ImageProcessor::save_image(config.output_dir + "/edges.png", edges);
    
    // Prepare visualization
    cv::Mat result = ImageProcessor::prepare_visualization(image);
    
    // Hough transform
    double hough_start = HoughUtils::get_time_ms();
    
    std::cout << "Detecting lines..." << std::endl;
    HoughLineDetector* line_detector = create_threaded_line_detector(
        image.cols, image.rows, config.num_threads, 
        1.0, M_PI / 180.0, config.threshold);
    
    std::vector<HoughLine> lines = line_detector->detect(edges);
    delete line_detector;
    
    std::cout << "Found " << lines.size() << " lines" << std::endl;
    
    // Draw lines
    HoughUtils::draw_lines(result, lines);
    
    // Save results
    HoughUtils::save_lines_to_file(config.output_dir + "/lines.txt", lines);
    
    double hough_end = HoughUtils::get_time_ms();
    double hough_time = hough_end - hough_start;
    
    // Save result image
    ImageProcessor::save_image(config.output_dir + "/result.png", result);
    
    double total_time = HoughUtils::get_time_ms() - start_time;
    
    // Save timing
    save_timing(config.output_dir + "/timing.txt", preprocess_time, hough_time, total_time);
    
    std::cout << "\nPerformance:" << std::endl;
    std::cout << "  Preprocessing: " << preprocess_time << " ms" << std::endl;
    std::cout << "  Hough transform: " << hough_time << " ms" << std::endl;
    std::cout << "  Total: " << total_time << " ms" << std::endl;
    std::cout << "\nResults saved to " << config.output_dir << std::endl;
    
    return 0;
}


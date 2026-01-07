#include "image_processor.h"
#include <thread>
#include <vector>
#include <algorithm>

// Parallel grayscale conversion
void grayscale_thread(const cv::Mat& input, cv::Mat& output, int start_row, int end_row) {
    for (int y = start_row; y < end_row; ++y) {
        const cv::Vec3b* input_row = input.ptr<cv::Vec3b>(y);
        uchar* output_row = output.ptr<uchar>(y);
        
        for (int x = 0; x < input.cols; ++x) {
            const cv::Vec3b& pixel = input_row[x];
            // Standard grayscale conversion: 0.299*R + 0.587*G + 0.114*B
            output_row[x] = static_cast<uchar>(
                0.114 * pixel[0] + 0.587 * pixel[1] + 0.299 * pixel[2]
            );
        }
    }
}

cv::Mat parallel_to_grayscale(const cv::Mat& image, int num_threads) {
    if (image.channels() == 1) {
        return image.clone();
    }
    
    cv::Mat grayscale(image.rows, image.cols, CV_8UC1);
    
    std::vector<std::thread> threads;
    int rows_per_thread = image.rows / num_threads;
    
    for (int i = 0; i < num_threads; ++i) {
        int start_row = i * rows_per_thread;
        int end_row = (i == num_threads - 1) ? image.rows : (i + 1) * rows_per_thread;
        
        threads.emplace_back(grayscale_thread, std::ref(image), std::ref(grayscale), start_row, end_row);
    }
    
    for (auto& thread : threads) {
        thread.join();
    }
    
    return grayscale;
}

// Parallel edge detection using Canny
void canny_thread(const cv::Mat& input, cv::Mat& output, int start_row, int end_row, 
                  double low_threshold, double high_threshold) {
    // Process a region of the image
    if (start_row > 0) start_row -= 2;  // Overlap for edge smoothness
    if (end_row < input.rows) end_row += 2;
    
    cv::Mat region = input(cv::Range(start_row, end_row), cv::Range(0, input.cols));
    cv::Mat region_blurred, region_edges;
    
    cv::GaussianBlur(region, region_blurred, cv::Size(5, 5), 1.5);
    cv::Canny(region_blurred, region_edges, low_threshold, high_threshold);
    
    // Copy back to output (excluding overlap)
    int copy_start = (start_row > 0) ? 2 : 0;
    int copy_end = region_edges.rows - ((end_row < input.rows) ? 2 : 0);
    int output_start = start_row + copy_start;
    
    region_edges(cv::Range(copy_start, copy_end), cv::Range(0, region_edges.cols))
        .copyTo(output(cv::Range(output_start, output_start + copy_end - copy_start), 
                      cv::Range(0, output.cols)));
}

cv::Mat parallel_detect_edges(const cv::Mat& grayscale, int num_threads, 
                               double low_threshold, double high_threshold) {
    cv::Mat edges = cv::Mat::zeros(grayscale.rows, grayscale.cols, CV_8UC1);
    
    std::vector<std::thread> threads;
    int rows_per_thread = grayscale.rows / num_threads;
    
    for (int i = 0; i < num_threads; ++i) {
        int start_row = i * rows_per_thread;
        int end_row = (i == num_threads - 1) ? grayscale.rows : (i + 1) * rows_per_thread;
        
        threads.emplace_back(canny_thread, std::ref(grayscale), std::ref(edges), 
                           start_row, end_row, low_threshold, high_threshold);
    }
    
    for (auto& thread : threads) {
        thread.join();
    }
    
    return edges;
}


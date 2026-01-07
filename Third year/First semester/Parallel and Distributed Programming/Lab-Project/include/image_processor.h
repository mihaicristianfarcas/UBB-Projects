#ifndef IMAGE_PROCESSOR_H
#define IMAGE_PROCESSOR_H

#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

class ImageProcessor {
public:
    // Load image from file
    static cv::Mat load_image(const std::string& filename);
    
    // Save image to file
    static bool save_image(const std::string& filename, const cv::Mat& image);
    
    // Convert to grayscale (serial version)
    static cv::Mat to_grayscale(const cv::Mat& image);
    
    // Apply Canny edge detection (serial version)
    static cv::Mat detect_edges(const cv::Mat& grayscale, double low_threshold = 50, double high_threshold = 150);
    
    // Get list of edge pixel coordinates
    static std::vector<cv::Point> get_edge_pixels(const cv::Mat& edges);
    
    // Create a copy of the image for visualization
    static cv::Mat prepare_visualization(const cv::Mat& original);
    
    // Validate image
    static bool is_valid_image(const cv::Mat& image);
};

#endif // IMAGE_PROCESSOR_H


#include "image_processor.h"
#include <iostream>

cv::Mat ImageProcessor::load_image(const std::string& filename) {
    cv::Mat image = cv::imread(filename, cv::IMREAD_COLOR);
    if (image.empty()) {
        std::cerr << "Error: Could not load image from " << filename << std::endl;
        return cv::Mat();
    }
    return image;
}

bool ImageProcessor::save_image(const std::string& filename, const cv::Mat& image) {
    if (!is_valid_image(image)) {
        std::cerr << "Error: Cannot save invalid image" << std::endl;
        return false;
    }
    return cv::imwrite(filename, image);
}

cv::Mat ImageProcessor::to_grayscale(const cv::Mat& image) {
    if (!is_valid_image(image)) {
        std::cerr << "Error: Invalid image for grayscale conversion" << std::endl;
        return cv::Mat();
    }
    
    cv::Mat grayscale;
    if (image.channels() == 1) {
        // Already grayscale
        grayscale = image.clone();
    } else if (image.channels() == 3 || image.channels() == 4) {
        // Convert from BGR/BGRA to grayscale
        cv::cvtColor(image, grayscale, cv::COLOR_BGR2GRAY);
    } else {
        std::cerr << "Error: Unsupported number of channels: " << image.channels() << std::endl;
        return cv::Mat();
    }
    
    return grayscale;
}

cv::Mat ImageProcessor::detect_edges(const cv::Mat& grayscale, double low_threshold, double high_threshold) {
    if (!is_valid_image(grayscale)) {
        std::cerr << "Error: Invalid image for edge detection" << std::endl;
        return cv::Mat();
    }
    
    if (grayscale.channels() != 1) {
        std::cerr << "Error: Edge detection requires grayscale image" << std::endl;
        return cv::Mat();
    }
    
    cv::Mat edges;
    
    // Apply Gaussian blur to reduce noise
    cv::Mat blurred;
    cv::GaussianBlur(grayscale, blurred, cv::Size(5, 5), 1.5);
    
    // Apply Canny edge detection
    cv::Canny(blurred, edges, low_threshold, high_threshold);
    
    return edges;
}

std::vector<cv::Point> ImageProcessor::get_edge_pixels(const cv::Mat& edges) {
    std::vector<cv::Point> edge_points;
    
    if (!is_valid_image(edges)) {
        return edge_points;
    }
    
    // Reserve space to avoid frequent reallocations
    int non_zero_count = cv::countNonZero(edges);
    edge_points.reserve(non_zero_count);
    
    // Iterate through the image and collect edge pixel coordinates
    for (int y = 0; y < edges.rows; ++y) {
        const uchar* row_ptr = edges.ptr<uchar>(y);
        for (int x = 0; x < edges.cols; ++x) {
            if (row_ptr[x] > 0) {
                edge_points.emplace_back(x, y);
            }
        }
    }
    
    return edge_points;
}

cv::Mat ImageProcessor::prepare_visualization(const cv::Mat& original) {
    cv::Mat visualization;
    
    if (!is_valid_image(original)) {
        return cv::Mat();
    }
    
    // Convert to color if grayscale
    if (original.channels() == 1) {
        cv::cvtColor(original, visualization, cv::COLOR_GRAY2BGR);
    } else {
        visualization = original.clone();
    }
    
    return visualization;
}

bool ImageProcessor::is_valid_image(const cv::Mat& image) {
    return !image.empty() && image.rows > 0 && image.cols > 0;
}


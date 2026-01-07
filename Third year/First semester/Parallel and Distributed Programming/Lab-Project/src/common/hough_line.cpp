#include "hough_line.h"
#include "image_processor.h"

HoughLineDetector::HoughLineDetector(int width, int height, double rho_res, double theta_res, int thresh)
    : image_width(width), image_height(height), 
      rho_resolution(rho_res), theta_resolution(theta_res), threshold(thresh) {
}

// Serial implementation
SerialHoughLineDetector::SerialHoughLineDetector(int width, int height, double rho_res, double theta_res, int thresh)
    : HoughLineDetector(width, height, rho_res, theta_res, thresh) {
}

std::vector<HoughLine> SerialHoughLineDetector::detect(const cv::Mat& edges) {
    // Create accumulator
    LineAccumulator accumulator(image_width, image_height, rho_resolution, theta_resolution);
    
    // Get edge pixels
    std::vector<cv::Point> edge_pixels = ImageProcessor::get_edge_pixels(edges);
    
    // Vote in accumulator for each edge pixel
    for (const auto& point : edge_pixels) {
        accumulator.vote(point.x, point.y);
    }
    
    // Find peaks in accumulator
    return accumulator.find_peaks(threshold);
}


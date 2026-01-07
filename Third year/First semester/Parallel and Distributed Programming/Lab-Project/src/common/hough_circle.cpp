#include "hough_circle.h"
#include "image_processor.h"

HoughCircleDetector::HoughCircleDetector(int width, int height, int min_r, int max_r, int thresh)
    : image_width(width), image_height(height), 
      min_radius(min_r), max_radius(max_r), threshold(thresh) {
}

// Serial implementation
SerialHoughCircleDetector::SerialHoughCircleDetector(int width, int height, int min_r, int max_r, int thresh)
    : HoughCircleDetector(width, height, min_r, max_r, thresh) {
}

std::vector<HoughCircle> SerialHoughCircleDetector::detect(const cv::Mat& edges) {
    std::vector<HoughCircle> all_circles;
    
    // Get edge pixels
    std::vector<cv::Point> edge_pixels = ImageProcessor::get_edge_pixels(edges);
    
    // Process each radius
    for (int radius = min_radius; radius <= max_radius; ++radius) {
        // Create accumulator for this radius
        CircleAccumulator accumulator(image_width, image_height);
        
        // Vote in accumulator for each edge pixel
        for (const auto& point : edge_pixels) {
            accumulator.vote(point.x, point.y, radius);
        }
        
        // Find peaks for this radius
        std::vector<HoughCircle> circles = accumulator.find_peaks(radius, threshold);
        all_circles.insert(all_circles.end(), circles.begin(), circles.end());
    }
    
    // Sort all circles by votes
    std::sort(all_circles.begin(), all_circles.end(), std::greater<HoughCircle>());
    
    return all_circles;
}


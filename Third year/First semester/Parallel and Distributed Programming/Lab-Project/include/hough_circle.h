#ifndef HOUGH_CIRCLE_H
#define HOUGH_CIRCLE_H

#include "hough_common.h"
#include <opencv2/opencv.hpp>
#include <vector>

// Base class for circle detection using Hough transform
class HoughCircleDetector {
protected:
    int image_width;
    int image_height;
    int min_radius;
    int max_radius;
    int threshold;
    
public:
    HoughCircleDetector(int width, int height, int min_r = 10, 
                        int max_r = 100, int thresh = 100);
    
    virtual ~HoughCircleDetector() = default;
    
    // Detect circles in edge image (to be implemented by derived classes)
    virtual std::vector<HoughCircle> detect(const cv::Mat& edges) = 0;
    
    // Set detection parameters
    void set_threshold(int thresh) { threshold = thresh; }
    void set_radius_range(int min_r, int max_r) { min_radius = min_r; max_radius = max_r; }
    
    // Get parameters
    int get_threshold() const { return threshold; }
    int get_min_radius() const { return min_radius; }
    int get_max_radius() const { return max_radius; }
};

// Serial implementation for reference/testing
class SerialHoughCircleDetector : public HoughCircleDetector {
public:
    SerialHoughCircleDetector(int width, int height, int min_r = 10, 
                             int max_r = 100, int thresh = 100);
    
    std::vector<HoughCircle> detect(const cv::Mat& edges) override;
};

#endif // HOUGH_CIRCLE_H


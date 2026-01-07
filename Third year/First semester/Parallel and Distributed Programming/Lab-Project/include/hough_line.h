#ifndef HOUGH_LINE_H
#define HOUGH_LINE_H

#include "hough_common.h"
#include <opencv2/opencv.hpp>
#include <vector>

// Base class for line detection using Hough transform
class HoughLineDetector {
protected:
    int image_width;
    int image_height;
    double rho_resolution;
    double theta_resolution;
    int threshold;
    
public:
    HoughLineDetector(int width, int height, double rho_res = 1.0, 
                      double theta_res = M_PI / 180.0, int thresh = 100);
    
    virtual ~HoughLineDetector() = default;
    
    // Detect lines in edge image (to be implemented by derived classes)
    virtual std::vector<HoughLine> detect(const cv::Mat& edges) = 0;
    
    // Set detection threshold
    void set_threshold(int thresh) { threshold = thresh; }
    
    // Get parameters
    int get_threshold() const { return threshold; }
};

// Serial implementation for reference/testing
class SerialHoughLineDetector : public HoughLineDetector {
public:
    SerialHoughLineDetector(int width, int height, double rho_res = 1.0, 
                           double theta_res = M_PI / 180.0, int thresh = 100);
    
    std::vector<HoughLine> detect(const cv::Mat& edges) override;
};

#endif // HOUGH_LINE_H


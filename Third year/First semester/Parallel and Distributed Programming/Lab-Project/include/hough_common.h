#ifndef HOUGH_COMMON_H
#define HOUGH_COMMON_H

#include <opencv2/opencv.hpp>
#include <vector>
#include <cmath>
#include <algorithm>

// Structure to represent a detected line in Hough space
struct HoughLine {
    double rho;      // Distance from origin to the line
    double theta;    // Angle in radians
    int votes;       // Number of votes in accumulator
    
    HoughLine() : rho(0), theta(0), votes(0) {}
    HoughLine(double r, double t, int v) : rho(r), theta(t), votes(v) {}
    
    // For sorting by votes
    bool operator>(const HoughLine& other) const {
        return votes > other.votes;
    }
};

// Structure to represent a detected circle
struct HoughCircle {
    int x;           // Center x coordinate
    int y;           // Center y coordinate
    int radius;      // Circle radius
    int votes;       // Number of votes in accumulator
    
    HoughCircle() : x(0), y(0), radius(0), votes(0) {}
    HoughCircle(int cx, int cy, int r, int v) : x(cx), y(cy), radius(r), votes(v) {}
    
    // For sorting by votes
    bool operator>(const HoughCircle& other) const {
        return votes > other.votes;
    }
};

// Class to manage 2D accumulator space for line detection
class LineAccumulator {
private:
    std::vector<int> data;
    int rho_size;
    int theta_size;
    double rho_resolution;
    double theta_resolution;
    double max_rho;
    
public:
    LineAccumulator(int img_width, int img_height, double rho_res = 1.0, double theta_res = M_PI / 180.0);
    
    void vote(int x, int y);
    void accumulate(int rho_idx, int theta_idx);
    int get_vote(int rho_idx, int theta_idx) const;
    void merge(const LineAccumulator& other);
    std::vector<HoughLine> find_peaks(int threshold, int min_distance = 10);
    
    int get_rho_size() const { return rho_size; }
    int get_theta_size() const { return theta_size; }
    double get_rho_resolution() const { return rho_resolution; }
    double get_theta_resolution() const { return theta_resolution; }
    double get_max_rho() const { return max_rho; }
    
    // Convert accumulator indices to rho/theta values
    double idx_to_rho(int idx) const;
    double idx_to_theta(int idx) const;
    
    // Convert rho/theta values to indices
    int rho_to_idx(double rho) const;
    int theta_to_idx(double theta) const;
};

// Class to manage 3D accumulator space for circle detection (one radius at a time)
class CircleAccumulator {
private:
    std::vector<int> data;
    int width;
    int height;
    int current_radius;
    
public:
    CircleAccumulator(int img_width, int img_height);
    
    void reset();
    void vote(int edge_x, int edge_y, int radius);
    void accumulate(int x, int y);
    int get_vote(int x, int y) const;
    void merge(const CircleAccumulator& other);
    std::vector<HoughCircle> find_peaks(int radius, int threshold, int min_distance = 10);
    
    int get_width() const { return width; }
    int get_height() const { return height; }
};

// Utility functions
namespace HoughUtils {
    // Non-maximum suppression for line detection
    bool is_local_maximum(const LineAccumulator& acc, int rho_idx, int theta_idx, int window_size);
    
    // Non-maximum suppression for circle detection
    bool is_local_maximum(const CircleAccumulator& acc, int x, int y, int window_size);
    
    // Draw detected lines on image
    void draw_lines(cv::Mat& image, const std::vector<HoughLine>& lines, 
                    const cv::Scalar& color = cv::Scalar(0, 0, 255));
    
    // Draw detected circles on image
    void draw_circles(cv::Mat& image, const std::vector<HoughCircle>& circles,
                      const cv::Scalar& color = cv::Scalar(0, 255, 0));
    
    // Save results to text file
    void save_lines_to_file(const std::string& filename, const std::vector<HoughLine>& lines);
    void save_circles_to_file(const std::string& filename, const std::vector<HoughCircle>& circles);
    
    // Timing utilities
    double get_time_ms();
}

#endif // HOUGH_COMMON_H


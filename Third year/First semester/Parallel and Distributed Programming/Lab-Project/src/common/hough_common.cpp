#include "hough_common.h"
#include <chrono>
#include <fstream>
#include <iostream>

// LineAccumulator implementation
LineAccumulator::LineAccumulator(int img_width, int img_height, double rho_res, double theta_res)
    : rho_resolution(rho_res), theta_resolution(theta_res) {
    
    // Calculate maximum rho (diagonal of image)
    max_rho = std::sqrt(img_width * img_width + img_height * img_height);
    
    // Calculate accumulator dimensions
    rho_size = static_cast<int>(2 * max_rho / rho_resolution) + 1;
    theta_size = static_cast<int>(M_PI / theta_resolution);
    
    // Initialize accumulator
    data.resize(rho_size * theta_size, 0);
}

void LineAccumulator::vote(int x, int y) {
    // For each angle theta, calculate rho and vote
    for (int t = 0; t < theta_size; ++t) {
        double theta = idx_to_theta(t);
        double rho = x * std::cos(theta) + y * std::sin(theta);
        int rho_idx = rho_to_idx(rho);
        
        if (rho_idx >= 0 && rho_idx < rho_size) {
            data[rho_idx * theta_size + t]++;
        }
    }
}

void LineAccumulator::accumulate(int rho_idx, int theta_idx) {
    if (rho_idx >= 0 && rho_idx < rho_size && theta_idx >= 0 && theta_idx < theta_size) {
        data[rho_idx * theta_size + theta_idx]++;
    }
}

int LineAccumulator::get_vote(int rho_idx, int theta_idx) const {
    if (rho_idx >= 0 && rho_idx < rho_size && theta_idx >= 0 && theta_idx < theta_size) {
        return data[rho_idx * theta_size + theta_idx];
    }
    return 0;
}

void LineAccumulator::merge(const LineAccumulator& other) {
    if (data.size() == other.data.size()) {
        for (size_t i = 0; i < data.size(); ++i) {
            data[i] += other.data[i];
        }
    }
}

std::vector<HoughLine> LineAccumulator::find_peaks(int threshold, int min_distance) {
    std::vector<HoughLine> lines;
    
    // Find all points above threshold
    for (int r = 0; r < rho_size; ++r) {
        for (int t = 0; t < theta_size; ++t) {
            int votes = get_vote(r, t);
            if (votes >= threshold) {
                if (HoughUtils::is_local_maximum(*this, r, t, min_distance)) {
                    double rho = idx_to_rho(r);
                    double theta = idx_to_theta(t);
                    lines.emplace_back(rho, theta, votes);
                }
            }
        }
    }
    
    // Sort by votes (descending)
    std::sort(lines.begin(), lines.end(), std::greater<HoughLine>());
    
    return lines;
}

double LineAccumulator::idx_to_rho(int idx) const {
    return (idx * rho_resolution) - max_rho;
}

double LineAccumulator::idx_to_theta(int idx) const {
    return idx * theta_resolution;
}

int LineAccumulator::rho_to_idx(double rho) const {
    return static_cast<int>((rho + max_rho) / rho_resolution);
}

int LineAccumulator::theta_to_idx(double theta) const {
    return static_cast<int>(theta / theta_resolution);
}

// CircleAccumulator implementation
CircleAccumulator::CircleAccumulator(int img_width, int img_height)
    : width(img_width), height(img_height), current_radius(0) {
    data.resize(width * height, 0);
}

void CircleAccumulator::reset() {
    std::fill(data.begin(), data.end(), 0);
}

void CircleAccumulator::vote(int edge_x, int edge_y, int radius) {
    current_radius = radius;
    
    // Vote for all possible circle centers
    for (int angle = 0; angle < 360; ++angle) {
        double theta = angle * M_PI / 180.0;
        int cx = edge_x + static_cast<int>(radius * std::cos(theta));
        int cy = edge_y + static_cast<int>(radius * std::sin(theta));
        
        if (cx >= 0 && cx < width && cy >= 0 && cy < height) {
            data[cy * width + cx]++;
        }
    }
}

void CircleAccumulator::accumulate(int x, int y) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        data[y * width + x]++;
    }
}

int CircleAccumulator::get_vote(int x, int y) const {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        return data[y * width + x];
    }
    return 0;
}

void CircleAccumulator::merge(const CircleAccumulator& other) {
    if (data.size() == other.data.size()) {
        for (size_t i = 0; i < data.size(); ++i) {
            data[i] += other.data[i];
        }
    }
}

std::vector<HoughCircle> CircleAccumulator::find_peaks(int radius, int threshold, int min_distance) {
    std::vector<HoughCircle> circles;
    
    // Find all points above threshold
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int votes = get_vote(x, y);
            if (votes >= threshold) {
                if (HoughUtils::is_local_maximum(*this, x, y, min_distance)) {
                    circles.emplace_back(x, y, radius, votes);
                }
            }
        }
    }
    
    // Sort by votes (descending)
    std::sort(circles.begin(), circles.end(), std::greater<HoughCircle>());
    
    return circles;
}

// HoughUtils implementation
namespace HoughUtils {

bool is_local_maximum(const LineAccumulator& acc, int rho_idx, int theta_idx, int window_size) {
    int center_votes = acc.get_vote(rho_idx, theta_idx);
    
    for (int dr = -window_size; dr <= window_size; ++dr) {
        for (int dt = -window_size; dt <= window_size; ++dt) {
            if (dr == 0 && dt == 0) continue;
            
            int r = rho_idx + dr;
            int t = theta_idx + dt;
            
            if (acc.get_vote(r, t) > center_votes) {
                return false;
            }
        }
    }
    
    return true;
}

bool is_local_maximum(const CircleAccumulator& acc, int x, int y, int window_size) {
    int center_votes = acc.get_vote(x, y);
    
    for (int dy = -window_size; dy <= window_size; ++dy) {
        for (int dx = -window_size; dx <= window_size; ++dx) {
            if (dx == 0 && dy == 0) continue;
            
            if (acc.get_vote(x + dx, y + dy) > center_votes) {
                return false;
            }
        }
    }
    
    return true;
}

void draw_lines(cv::Mat& image, const std::vector<HoughLine>& lines, const cv::Scalar& color) {
    for (const auto& line : lines) {
        double rho = line.rho;
        double theta = line.theta;
        
        // Convert from polar to Cartesian coordinates
        double cos_theta = std::cos(theta);
        double sin_theta = std::sin(theta);
        double x0 = cos_theta * rho;
        double y0 = sin_theta * rho;
        
        // Calculate two points on the line
        int x1 = static_cast<int>(x0 + 10000 * (-sin_theta));
        int y1 = static_cast<int>(y0 + 10000 * cos_theta);
        int x2 = static_cast<int>(x0 - 10000 * (-sin_theta));
        int y2 = static_cast<int>(y0 - 10000 * cos_theta);
        
        cv::line(image, cv::Point(x1, y1), cv::Point(x2, y2), color, 2);
    }
}

void draw_circles(cv::Mat& image, const std::vector<HoughCircle>& circles, const cv::Scalar& color) {
    for (const auto& circle : circles) {
        cv::circle(image, cv::Point(circle.x, circle.y), circle.radius, color, 2);
        cv::circle(image, cv::Point(circle.x, circle.y), 2, cv::Scalar(0, 0, 255), -1);  // Draw center
    }
}

void save_lines_to_file(const std::string& filename, const std::vector<HoughLine>& lines) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << " for writing" << std::endl;
        return;
    }
    
    file << "Detected Lines: " << lines.size() << std::endl;
    file << "Format: rho (pixels), theta (radians), votes" << std::endl;
    file << "----------------------------------------" << std::endl;
    
    for (size_t i = 0; i < lines.size(); ++i) {
        file << "Line " << (i + 1) << ": "
             << "rho=" << lines[i].rho << ", "
             << "theta=" << lines[i].theta << ", "
             << "votes=" << lines[i].votes << std::endl;
    }
    
    file.close();
}

void save_circles_to_file(const std::string& filename, const std::vector<HoughCircle>& circles) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << " for writing" << std::endl;
        return;
    }
    
    file << "Detected Circles: " << circles.size() << std::endl;
    file << "Format: x, y, radius (pixels), votes" << std::endl;
    file << "----------------------------------------" << std::endl;
    
    for (size_t i = 0; i < circles.size(); ++i) {
        file << "Circle " << (i + 1) << ": "
             << "x=" << circles[i].x << ", "
             << "y=" << circles[i].y << ", "
             << "radius=" << circles[i].radius << ", "
             << "votes=" << circles[i].votes << std::endl;
    }
    
    file.close();
}

double get_time_ms() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::high_resolution_clock::now().time_since_epoch()
    ).count();
}

} // namespace HoughUtils


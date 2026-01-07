#include "hough_line.h"
#include "image_processor.h"
#include <thread>
#include <mutex>
#include <vector>

class ThreadedHoughLineDetector : public HoughLineDetector {
private:
    int num_threads;
    
    void vote_thread(const std::vector<cv::Point>& edge_pixels, 
                    size_t start_idx, size_t end_idx,
                    LineAccumulator& local_accumulator) {
        for (size_t i = start_idx; i < end_idx; ++i) {
            local_accumulator.vote(edge_pixels[i].x, edge_pixels[i].y);
        }
    }
    
public:
    ThreadedHoughLineDetector(int width, int height, int num_threads_,
                             double rho_res = 1.0, double theta_res = M_PI / 180.0, int thresh = 100)
        : HoughLineDetector(width, height, rho_res, theta_res, thresh), 
          num_threads(num_threads_) {
    }
    
    std::vector<HoughLine> detect(const cv::Mat& edges) override {
        // Get edge pixels
        std::vector<cv::Point> edge_pixels = ImageProcessor::get_edge_pixels(edges);
        
        if (edge_pixels.empty()) {
            return std::vector<HoughLine>();
        }
        
        // Create local accumulators for each thread
        std::vector<LineAccumulator> local_accumulators;
        for (int i = 0; i < num_threads; ++i) {
            local_accumulators.emplace_back(image_width, image_height, rho_resolution, theta_resolution);
        }
        
        // Divide work among threads
        std::vector<std::thread> threads;
        size_t pixels_per_thread = edge_pixels.size() / num_threads;
        
        for (int i = 0; i < num_threads; ++i) {
            size_t start_idx = i * pixels_per_thread;
            size_t end_idx = (i == num_threads - 1) ? edge_pixels.size() : (i + 1) * pixels_per_thread;
            
            threads.emplace_back(&ThreadedHoughLineDetector::vote_thread, this,
                               std::ref(edge_pixels), start_idx, end_idx, 
                               std::ref(local_accumulators[i]));
        }
        
        // Wait for all threads to finish
        for (auto& thread : threads) {
            thread.join();
        }
        
        // Merge accumulators
        LineAccumulator global_accumulator(image_width, image_height, rho_resolution, theta_resolution);
        for (const auto& local_acc : local_accumulators) {
            global_accumulator.merge(local_acc);
        }
        
        // Find peaks
        return global_accumulator.find_peaks(threshold);
    }
};

// Factory function to create threaded line detector
HoughLineDetector* create_threaded_line_detector(int width, int height, int num_threads,
                                                double rho_res, double theta_res, int thresh) {
    return new ThreadedHoughLineDetector(width, height, num_threads, rho_res, theta_res, thresh);
}


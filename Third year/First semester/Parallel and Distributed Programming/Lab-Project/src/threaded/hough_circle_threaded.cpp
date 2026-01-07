#include "hough_circle.h"
#include "image_processor.h"
#include <thread>
#include <vector>

class ThreadedHoughCircleDetector : public HoughCircleDetector {
private:
    int num_threads;
    
    void vote_thread(const std::vector<cv::Point>& edge_pixels,
                    size_t start_idx, size_t end_idx,
                    int radius, CircleAccumulator& local_accumulator) {
        for (size_t i = start_idx; i < end_idx; ++i) {
            local_accumulator.vote(edge_pixels[i].x, edge_pixels[i].y, radius);
        }
    }
    
public:
    ThreadedHoughCircleDetector(int width, int height, int num_threads_,
                               int min_r = 10, int max_r = 100, int thresh = 100)
        : HoughCircleDetector(width, height, min_r, max_r, thresh),
          num_threads(num_threads_) {
    }
    
    std::vector<HoughCircle> detect(const cv::Mat& edges) override {
        std::vector<HoughCircle> all_circles;
        
        // Get edge pixels
        std::vector<cv::Point> edge_pixels = ImageProcessor::get_edge_pixels(edges);
        
        if (edge_pixels.empty()) {
            return all_circles;
        }
        
        // Process each radius
        for (int radius = min_radius; radius <= max_radius; ++radius) {
            // Create local accumulators for each thread
            std::vector<CircleAccumulator> local_accumulators;
            for (int i = 0; i < num_threads; ++i) {
                local_accumulators.emplace_back(image_width, image_height);
            }
            
            // Divide work among threads
            std::vector<std::thread> threads;
            size_t pixels_per_thread = edge_pixels.size() / num_threads;
            
            for (int i = 0; i < num_threads; ++i) {
                size_t start_idx = i * pixels_per_thread;
                size_t end_idx = (i == num_threads - 1) ? edge_pixels.size() : (i + 1) * pixels_per_thread;
                
                threads.emplace_back(&ThreadedHoughCircleDetector::vote_thread, this,
                                   std::ref(edge_pixels), start_idx, end_idx,
                                   radius, std::ref(local_accumulators[i]));
            }
            
            // Wait for all threads to finish
            for (auto& thread : threads) {
                thread.join();
            }
            
            // Merge accumulators
            CircleAccumulator global_accumulator(image_width, image_height);
            for (const auto& local_acc : local_accumulators) {
                global_accumulator.merge(local_acc);
            }
            
            // Find peaks for this radius
            std::vector<HoughCircle> circles = global_accumulator.find_peaks(radius, threshold);
            all_circles.insert(all_circles.end(), circles.begin(), circles.end());
        }
        
        // Sort all circles by votes
        std::sort(all_circles.begin(), all_circles.end(), std::greater<HoughCircle>());
        
        return all_circles;
    }
};

// Factory function to create threaded circle detector
HoughCircleDetector* create_threaded_circle_detector(int width, int height, int num_threads,
                                                     int min_r, int max_r, int thresh) {
    return new ThreadedHoughCircleDetector(width, height, num_threads, min_r, max_r, thresh);
}


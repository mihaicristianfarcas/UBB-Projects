#include "hough_circle.h"
#include "image_processor.h"
#include <mpi.h>
#include <vector>

class MPIHoughCircleDetector : public HoughCircleDetector {
private:
    int rank;
    int size;
    
public:
    MPIHoughCircleDetector(int width, int height, int mpi_rank, int mpi_size,
                          int min_r = 10, int max_r = 100, int thresh = 100)
        : HoughCircleDetector(width, height, min_r, max_r, thresh),
          rank(mpi_rank), size(mpi_size) {
    }
    
    std::vector<HoughCircle> detect(const cv::Mat& edges) override {
        std::vector<HoughCircle> all_circles;
        std::vector<cv::Point> edge_pixels;
        
        // Master gets edge pixels
        if (rank == 0) {
            edge_pixels = ImageProcessor::get_edge_pixels(edges);
        }
        
        // Broadcast number of edge pixels
        int num_pixels = edge_pixels.size();
        MPI_Bcast(&num_pixels, 1, MPI_INT, 0, MPI_COMM_WORLD);
        
        if (num_pixels == 0) {
            return all_circles;
        }
        
        // Prepare data for scatter
        std::vector<int> pixel_data;
        if (rank == 0) {
            pixel_data.resize(num_pixels * 2);
            for (int i = 0; i < num_pixels; ++i) {
                pixel_data[i * 2] = edge_pixels[i].x;
                pixel_data[i * 2 + 1] = edge_pixels[i].y;
            }
        }
        
        // Calculate distribution
        int pixels_per_proc = num_pixels / size;
        int extra_pixels = num_pixels % size;
        
        std::vector<int> send_counts(size);
        std::vector<int> displacements(size);
        
        for (int i = 0; i < size; ++i) {
            int proc_pixels = pixels_per_proc + (i < extra_pixels ? 1 : 0);
            send_counts[i] = proc_pixels * 2;
            displacements[i] = (i > 0) ? displacements[i-1] + send_counts[i-1] : 0;
        }
        
        int my_pixels = pixels_per_proc + (rank < extra_pixels ? 1 : 0);
        std::vector<int> my_pixel_data(my_pixels * 2);
        
        // Scatter edge pixels
        MPI_Scatterv(rank == 0 ? pixel_data.data() : nullptr,
                     send_counts.data(), displacements.data(), MPI_INT,
                     my_pixel_data.data(), send_counts[rank], MPI_INT,
                     0, MPI_COMM_WORLD);
        
        // Process each radius
        for (int radius = min_radius; radius <= max_radius; ++radius) {
            // Create local accumulator and vote
            CircleAccumulator local_accumulator(image_width, image_height);
            
            for (int i = 0; i < my_pixels; ++i) {
                int x = my_pixel_data[i * 2];
                int y = my_pixel_data[i * 2 + 1];
                local_accumulator.vote(x, y, radius);
            }
            
            // Reduce accumulators
            int acc_size = image_width * image_height;
            std::vector<int> local_acc_data(acc_size);
            std::vector<int> global_acc_data;
            
            // Copy local accumulator data
            for (int y = 0; y < image_height; ++y) {
                for (int x = 0; x < image_width; ++x) {
                    local_acc_data[y * image_width + x] = local_accumulator.get_vote(x, y);
                }
            }
            
            if (rank == 0) {
                global_acc_data.resize(acc_size);
            }
            
            // Reduce accumulator data
            MPI_Reduce(local_acc_data.data(),
                      rank == 0 ? global_acc_data.data() : nullptr,
                      acc_size, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
            
            // Master finds peaks for this radius
            if (rank == 0) {
                CircleAccumulator global_accumulator(image_width, image_height);
                
                for (int y = 0; y < image_height; ++y) {
                    for (int x = 0; x < image_width; ++x) {
                        int votes = global_acc_data[y * image_width + x];
                        for (int v = 0; v < votes; ++v) {
                            global_accumulator.accumulate(x, y);
                        }
                    }
                }
                
                std::vector<HoughCircle> circles = global_accumulator.find_peaks(radius, threshold);
                all_circles.insert(all_circles.end(), circles.begin(), circles.end());
            }
        }
        
        // Sort by votes
        if (rank == 0) {
            std::sort(all_circles.begin(), all_circles.end(), std::greater<HoughCircle>());
        }
        
        return all_circles;
    }
};

// Factory function
HoughCircleDetector* create_mpi_circle_detector(int width, int height, int rank, int size,
                                                int min_r, int max_r, int thresh) {
    return new MPIHoughCircleDetector(width, height, rank, size, min_r, max_r, thresh);
}


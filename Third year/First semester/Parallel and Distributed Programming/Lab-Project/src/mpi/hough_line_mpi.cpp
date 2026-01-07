#include "hough_line.h"
#include "image_processor.h"
#include <mpi.h>
#include <vector>

class MPIHoughLineDetector : public HoughLineDetector {
private:
    int rank;
    int size;
    
public:
    MPIHoughLineDetector(int width, int height, int mpi_rank, int mpi_size,
                        double rho_res = 1.0, double theta_res = M_PI / 180.0, int thresh = 100)
        : HoughLineDetector(width, height, rho_res, theta_res, thresh),
          rank(mpi_rank), size(mpi_size) {
    }
    
    std::vector<HoughLine> detect(const cv::Mat& edges) override {
        std::vector<cv::Point> edge_pixels;
        
        // Master gets edge pixels
        if (rank == 0) {
            edge_pixels = ImageProcessor::get_edge_pixels(edges);
        }
        
        // Broadcast number of edge pixels
        int num_pixels = edge_pixels.size();
        MPI_Bcast(&num_pixels, 1, MPI_INT, 0, MPI_COMM_WORLD);
        
        if (num_pixels == 0) {
            return std::vector<HoughLine>();
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
        
        // Create local accumulator and vote
        LineAccumulator local_accumulator(image_width, image_height, rho_resolution, theta_resolution);
        
        for (int i = 0; i < my_pixels; ++i) {
            int x = my_pixel_data[i * 2];
            int y = my_pixel_data[i * 2 + 1];
            local_accumulator.vote(x, y);
        }
        
        // Reduce accumulators
        int acc_size = local_accumulator.get_rho_size() * local_accumulator.get_theta_size();
        std::vector<int> local_acc_data(acc_size);
        std::vector<int> global_acc_data;
        
        // Copy local accumulator data
        for (int r = 0; r < local_accumulator.get_rho_size(); ++r) {
            for (int t = 0; t < local_accumulator.get_theta_size(); ++t) {
                local_acc_data[r * local_accumulator.get_theta_size() + t] = 
                    local_accumulator.get_vote(r, t);
            }
        }
        
        if (rank == 0) {
            global_acc_data.resize(acc_size);
        }
        
        // Reduce all accumulator data to rank 0
        MPI_Reduce(local_acc_data.data(), 
                   rank == 0 ? global_acc_data.data() : nullptr,
                   acc_size, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
        
        // Master finds peaks
        std::vector<HoughLine> lines;
        if (rank == 0) {
            // Create global accumulator and populate it
            LineAccumulator global_accumulator(image_width, image_height, rho_resolution, theta_resolution);
            
            for (int r = 0; r < global_accumulator.get_rho_size(); ++r) {
                for (int t = 0; t < global_accumulator.get_theta_size(); ++t) {
                    int votes = global_acc_data[r * global_accumulator.get_theta_size() + t];
                    for (int v = 0; v < votes; ++v) {
                        global_accumulator.accumulate(r, t);
                    }
                }
            }
            
            lines = global_accumulator.find_peaks(threshold);
        }
        
        return lines;
    }
};

// Factory function
HoughLineDetector* create_mpi_line_detector(int width, int height, int rank, int size,
                                           double rho_res, double theta_res, int thresh) {
    return new MPIHoughLineDetector(width, height, rank, size, rho_res, theta_res, thresh);
}


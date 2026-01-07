#include "image_processor.h"
#include <mpi.h>
#include <vector>

// MPI parallel grayscale conversion
cv::Mat mpi_to_grayscale(const cv::Mat& image, int rank, int size) {
    int rows = image.rows;
    int cols = image.cols;
    int channels = image.channels();
    
    cv::Mat grayscale;
    
    if (rank == 0) {
        grayscale = cv::Mat(rows, cols, CV_8UC1);
    }
    
    // Broadcast image dimensions
    MPI_Bcast(&rows, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&cols, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&channels, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    if (channels == 1) {
        // Already grayscale
        return image.clone();
    }
    
    // Calculate rows per process
    int rows_per_proc = rows / size;
    int extra_rows = rows % size;
    
    // Each process gets its chunk size
    std::vector<int> send_counts(size);
    std::vector<int> displacements(size);
    
    for (int i = 0; i < size; ++i) {
        int proc_rows = rows_per_proc + (i < extra_rows ? 1 : 0);
        send_counts[i] = proc_rows * cols * channels;
        displacements[i] = (i > 0) ? displacements[i-1] + send_counts[i-1] : 0;
    }
    
    int my_rows = rows_per_proc + (rank < extra_rows ? 1 : 0);
    std::vector<uchar> recv_buffer(my_rows * cols * channels);
    std::vector<uchar> send_buffer;
    
    if (rank == 0) {
        send_buffer.assign(image.data, image.data + rows * cols * channels);
    }
    
    // Scatter image data
    MPI_Scatterv(rank == 0 ? send_buffer.data() : nullptr, 
                 send_counts.data(), displacements.data(), MPI_UNSIGNED_CHAR,
                 recv_buffer.data(), send_counts[rank], MPI_UNSIGNED_CHAR,
                 0, MPI_COMM_WORLD);
    
    // Convert to grayscale locally
    std::vector<uchar> gray_buffer(my_rows * cols);
    for (int i = 0; i < my_rows * cols; ++i) {
        int idx = i * channels;
        gray_buffer[i] = static_cast<uchar>(
            0.114 * recv_buffer[idx] + 0.587 * recv_buffer[idx + 1] + 0.299 * recv_buffer[idx + 2]
        );
    }
    
    // Prepare gather parameters
    std::vector<int> recv_counts(size);
    std::vector<int> recv_displacements(size);
    
    for (int i = 0; i < size; ++i) {
        int proc_rows = rows_per_proc + (i < extra_rows ? 1 : 0);
        recv_counts[i] = proc_rows * cols;
        recv_displacements[i] = (i > 0) ? recv_displacements[i-1] + recv_counts[i-1] : 0;
    }
    
    if (rank == 0) {
        grayscale = cv::Mat(rows, cols, CV_8UC1);
    }
    
    // Gather results
    MPI_Gatherv(gray_buffer.data(), recv_counts[rank], MPI_UNSIGNED_CHAR,
                rank == 0 ? grayscale.data : nullptr,
                recv_counts.data(), recv_displacements.data(), MPI_UNSIGNED_CHAR,
                0, MPI_COMM_WORLD);
    
    return grayscale;
}

// MPI parallel edge detection
cv::Mat mpi_detect_edges(const cv::Mat& grayscale, int rank, int size, 
                         double low_threshold, double high_threshold) {
    int rows = grayscale.rows;
    int cols = grayscale.cols;
    
    cv::Mat edges;
    
    if (rank == 0) {
        edges = cv::Mat(rows, cols, CV_8UC1);
    }
    
    // Broadcast dimensions
    MPI_Bcast(&rows, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&cols, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    // Calculate rows per process with overlap for edge detection
    int rows_per_proc = rows / size;
    int extra_rows = rows % size;
    
    int my_rows = rows_per_proc + (rank < extra_rows ? 1 : 0);
    int overlap = 2;  // Overlap for Gaussian blur and Canny
    
    // Add overlap rows
    int my_start_row = 0;
    for (int i = 0; i < rank; ++i) {
        my_start_row += rows_per_proc + (i < extra_rows ? 1 : 0);
    }
    
    int extended_start = std::max(0, my_start_row - overlap);
    int extended_end = std::min(rows, my_start_row + my_rows + overlap);
    int extended_rows = extended_end - extended_start;
    
    std::vector<uchar> recv_buffer(extended_rows * cols);
    
    if (rank == 0) {
        // Master distributes with overlap
        for (int i = 0; i < size; ++i) {
            int proc_rows = rows_per_proc + (i < extra_rows ? 1 : 0);
            int start_row = 0;
            for (int j = 0; j < i; ++j) {
                start_row += rows_per_proc + (j < extra_rows ? 1 : 0);
            }
            
            int ext_start = std::max(0, start_row - overlap);
            int ext_end = std::min(rows, start_row + proc_rows + overlap);
            int ext_rows = ext_end - ext_start;
            
            if (i == 0) {
                std::copy(grayscale.data + ext_start * cols,
                         grayscale.data + ext_end * cols,
                         recv_buffer.begin());
            } else {
                std::vector<uchar> send_buf(ext_rows * cols);
                std::copy(grayscale.data + ext_start * cols,
                         grayscale.data + ext_end * cols,
                         send_buf.begin());
                MPI_Send(send_buf.data(), ext_rows * cols, MPI_UNSIGNED_CHAR, i, 0, MPI_COMM_WORLD);
            }
        }
    } else {
        MPI_Recv(recv_buffer.data(), extended_rows * cols, MPI_UNSIGNED_CHAR, 
                0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    
    // Process locally
    cv::Mat local_gray(extended_rows, cols, CV_8UC1, recv_buffer.data());
    cv::Mat local_blurred, local_edges;
    
    cv::GaussianBlur(local_gray, local_blurred, cv::Size(5, 5), 1.5);
    cv::Canny(local_blurred, local_edges, low_threshold, high_threshold);
    
    // Extract the valid region (without overlap)
    int valid_start = (my_start_row > 0) ? overlap : 0;
    int valid_rows = my_rows;
    
    cv::Mat valid_edges = local_edges(cv::Range(valid_start, valid_start + valid_rows), 
                                      cv::Range(0, cols));
    
    // Gather results
    std::vector<int> recv_counts(size);
    std::vector<int> recv_displacements(size);
    
    for (int i = 0; i < size; ++i) {
        int proc_rows = rows_per_proc + (i < extra_rows ? 1 : 0);
        recv_counts[i] = proc_rows * cols;
        recv_displacements[i] = (i > 0) ? recv_displacements[i-1] + recv_counts[i-1] : 0;
    }
    
    MPI_Gatherv(valid_edges.data, recv_counts[rank], MPI_UNSIGNED_CHAR,
                rank == 0 ? edges.data : nullptr,
                recv_counts.data(), recv_displacements.data(), MPI_UNSIGNED_CHAR,
                0, MPI_COMM_WORLD);
    
    return edges;
}


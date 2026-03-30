#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <iomanip>
#include <Eigen/Dense>


using namespace std;

class MatrixGenerator final {
public:

// Function to generate a random data matrix for demonstration
static Eigen::MatrixXd generateRandomData(int rows, int cols) {
    Eigen::MatrixXd data = Eigen::MatrixXd::Random(rows, cols);
    // Scale and offset to get values in a more general range, e.g., [0, 10]
    //data = (data + Eigen::MatrixXd::Constant(rows, cols, 1.0)) * 5.0;  
    data = (data + Eigen::MatrixXd::Constant(rows, cols, 1.0)) * 100.0;  
    return data;
}

// Function to compute the correlation matrix
static Eigen::MatrixXd computeCorrelationMatrix(const Eigen::MatrixXd& data) {
    int n_rows = data.rows();
    int n_cols = data.cols();

    // 1. Center the data (subtract column means)
    // The .rowwise() - .colwise().mean() expression efficiently subtracts the mean of each column from all elements in that column.
    Eigen::MatrixXd centered_data = data.rowwise() - data.colwise().mean();

    // 2. Compute the covariance matrix
    // The formula for the sample covariance matrix is (centered.adjoint() * centered) / (n - 1).
    // Note: centered.adjoint() is the conjugate transpose, which is just the transpose for real matrices.
    Eigen::MatrixXd cov_matrix = (centered_data.adjoint() * centered_data) / static_cast<double>(n_rows - 1);

    // 3. Convert the covariance matrix to a correlation matrix
    // The correlation between two variables x and y is cov(x, y) / (stddev(x) * stddev(y)).
    // The diagonal elements of the covariance matrix are the variances.
    Eigen::VectorXd std_dev = cov_matrix.diagonal().array().sqrt();
    
    Eigen::MatrixXd corr_matrix = Eigen::MatrixXd::Zero(n_cols, n_cols);
    for (int i = 0; i < n_cols; ++i) {
        for (int j = 0; j < n_cols; ++j) {
            // Normalize by the product of the standard deviations of the respective columns
            corr_matrix(i, j) = cov_matrix(i, j) / (std_dev(i) * std_dev(j));
        }
    }
    
    // Ensure the diagonal elements are exactly 1.0 due to potential floating point errors
    corr_matrix.diagonal().setConstant(1.0);

    return corr_matrix;
}

// Function to generate random correlation matrix
static std::vector<std::vector<double>> generateRandomCorr(int n, int k) {
    // 1. Generate random factor loadings matrix W (n x k)
    // Using random uniform values between 0 and 1
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    std::vector<std::vector<double>> W(n, std::vector<double>(k));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < k; ++j) {
            W[i][j] = dis(gen);
        }
    }

    // 2. Compute A = W * W^T (n x n)
    std::vector<std::vector<double>> A(n, std::vector<double>(n, 0.0));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            for (int l = 0; l < k; ++l) {
                A[i][j] += W[i][l] * W[j][l];
            }
        }
    }

    // 3. Normalize A to get correlation matrix C
    // C[i][j] = A[i][j] / sqrt(A[i][i] * A[j][j])
    std::vector<std::vector<double>> C(n, std::vector<double>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            C[i][j] = A[i][j] / std::sqrt(A[i][i] * A[j][j]);
        }
    }

    return C;
}


    // Prevent instantiation (C++11 and later)
    MatrixGenerator() = delete; 

private:
    // Private static data member (shared state)
    static int internalState;

};
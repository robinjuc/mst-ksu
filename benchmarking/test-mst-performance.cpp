#include <thread> // For std::this_thread::sleep_for
#include <vector>
#include "MatrixGenerator.h"
#include "time-clock.h"
#include "../algorithms/Prim.h"
#include "../algorithms/Boruvka.h"

int graph[V_BORUVKA][V_BORUVKA] = {0};

void generateMatrix()
{
    int n = 10; // Size of matrix
    int k = 3; // Number of factors
    auto corrMatrix = MatrixGenerator::generateRandomCorr(n, k);

    // Print Matrix
    std::cout << std::fixed << std::setprecision(4);
    for (const auto& row : corrMatrix) {
        for (double val : row) {
            std::cout << val << "\t";
        }
        std::cout << "\n";
    }
}

void testPrim() {
    int graph[V_PRIM][V_PRIM] = { {0, 2, 0, 6, 0}, {2, 0, 3, 8, 5}, {0, 3, 0, 0, 7}, {6, 8, 0, 0, 9}, {0, 5, 7, 9, 0} };
    
    Prim::primMSTArrayAdj(graph);    
}

void testBoruvka() {
    // Adjacency Matrix: graph[u][v] = weight (0 if no edge)
    int graph[V_BORUVKA][V_BORUVKA] = {
        {0, 10, 6, 5},
        {10, 0, 0, 15},
        {6, 0, 0, 4},
        {5, 15, 4, 0}
    };
    Boruvka::boruvkaMSTAdj(graph);
}


void printAdjacencyMatrix()
{
    for (int i = 0; i < V_PRIM; i++)            
    {
        for (int j = 0; j < V_PRIM; j++)
        {
            std::cout << graph[i][j] << "\t";            
        }
        std::cout << "\n";
    }    
}

void generateCorrelationMatrix()
{
    int n = V_PRIM; // Size of matrix
    int k = 3; // Number of factors
    auto corrMatrix = MatrixGenerator::generateRandomCorr(n, k);


    for (int i = 0; i < V_PRIM; i++)            
        for (int j = 0; j < V_PRIM; j++)
        {
            if (i == j)
            {
                graph[i][j] = 0.0;
            }
            else
            {
                graph[i][j] = corrMatrix[i][j] * 1000;
            }
        }    
}

void generateCorrelationMatrixEigen()
{
    int rows = V_PRIM; // Number of observations/samples
    int cols = V_PRIM;  // Number of variables/features

    // Generate a sample data matrix
    Eigen::MatrixXd data = MatrixGenerator::generateRandomData(rows, cols);

    // Compute the correlation matrix
    Eigen::MatrixXd correlation = MatrixGenerator::computeCorrelationMatrix(data);

    for (int i = 0; i < correlation.rows(); ++i) {
        for (int j = 0; j < correlation.cols(); ++j) {
            if (i == j)
                graph[i][j] = 0;
            else
                graph[i][j] = correlation(i, j) * 100;
        }
    }
}

void testMatrix()
{
    int rows = 10; // Number of observations/samples
    int cols = 10;  // Number of variables/features

    // Generate a sample data matrix
    Eigen::MatrixXd data = MatrixGenerator::generateRandomData(rows, cols);
    std::cout << "Original Data Matrix (" << rows << "x" << cols << "):" << std::endl << data << std::endl << std::endl;

    // Compute the correlation matrix
    Eigen::MatrixXd correlation = MatrixGenerator::computeCorrelationMatrix(data);

    // Print the result
    std::cout << "Correlation Matrix (" << cols << "x" << cols << "):" << std::endl << correlation << std::endl;

}

int main() {
    double time_ms = 0.0;

    generateCorrelationMatrixEigen(); //2000 vertices - Russel 2000 Index (2000 stocks)​
    //printAdjacencyMatrix();

    time_ms = measureFunctionTime(Prim::primMSTArrayAdj, graph);
    std::cout << "Prim array-based took: " << time_ms << " ms\n";

    time_ms = measureFunctionTime(Boruvka::boruvkaMSTAdj, graph, 10);
    std::cout << "Parallel Boruvka took: " << time_ms << " ms\n";


    //time_ms = measureFunctionTime(testMatrix);
    //std::cout << "Test took: " << time_ms << " ms\n";

    return 0;
}

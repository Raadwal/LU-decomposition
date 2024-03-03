#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <mpi.h>

#include "Matrix.h"
#include "LuSolver.h"

int main(int argc, char *argv[]) {
    // Initializes variables to store MPI informations
    int num_procs, rank;
    // Initialzie variable to store time of LU decomposition
    double run_time;
    // A variable containing information about whether the matrices will be printed on the screen.
    std::string print_matrices;

    // Initialize MPI environment
    MPI_Init(&argc, &argv);

    // Get the number of available nodes (processes)
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    // Get the rank of the current node
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Initialize matrix A
    Matrix A(0, 0);

    // In the main process...
    if (rank == 0) {
        try {
            // Read the file path
            std::string file_path;
            std::cout << "Enter the matrix file path: ";
            std::cin >> file_path;

            // Ask the user if they want to print the matrices 
            std::cout << "Do you want to print the results? [YES/NO]" << std::endl;
            std::cout << "Answer: ";
            std::cin >> print_matrices;
            std::transform(print_matrices.begin(), print_matrices.end(), print_matrices.begin(), ::tolower);

            // Load data into matrix A
            A.readFromFile(file_path);

            // Print data from matrix A
            if(print_matrices == "yes") {
                std::cout << "Matrix A:" << std::endl;
                std::cout << A << std::endl;
            }
        } catch (const std::exception &e) {
            std::cerr << "Error: " << e.what() << std::endl;
            return 1;
        }
    }

    // Allocate memory for matrices L and U
    Matrix L(A.rows(), A.cols());
    Matrix U(A.rows(), A.cols());
    
    // Initialize LuSovlver object
    LuSolver solver;
    // Initialize timer
    run_time = MPI_Wtime();

    // Pass matrices A, L, and U to a method performing LU decomposition. 
    solver.decompose(A, L, U, rank, num_procs);

    // Measure the execution time of the LU decomposition operation.
    run_time = MPI_Wtime() - run_time;

    // In the main process...
    if (rank == 0) {
        // Print the result of the LU decomposition to files.
        if(print_matrices == "yes") {
            std::cout << "Matrix L:" << std::endl;
            std::cout << L << std::endl;

            std::cout << "Matrix U:" << std::endl;
            std::cout << U << std::endl;
        }

        // Print the execution time of the LU decomposition operation
        std::cout << "Time: " << run_time << std::endl;

        // Ask the user if they want to save the data to a file
        std::string save_to_file;
        std::cout << "Do you want to save the results to files? [YES/NO]" << std::endl;
        std::cout << "Answer: ";
        std::cin >> save_to_file;
        std::transform(save_to_file.begin(), save_to_file.end(), save_to_file.begin(), ::tolower);

        // Saving the result of the LU decomposition to files.
        if (save_to_file == "yes") {
            try {
                L.saveToFile("results/L.txt");
                U.saveToFile("results/U.txt");
            } catch (const std::exception &e) {
                std::cerr << "Error: " << e.what() << std::endl;
                return 1;
            }
            std::cout << "The result has been successfully saved. The files are located in the 'results' folder." << std::endl;
        }
    }

    // Finalize MPI environment
    MPI_Finalize();
    return 0;
}
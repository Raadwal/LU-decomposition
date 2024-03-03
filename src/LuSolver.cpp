#include "LuSolver.h"
#include <cmath>
#include <mpi.h>
#include <string>

void LuSolver::decompose(Matrix& A, Matrix& L, Matrix& U, int rank, int num_procs) {
    validateMatrix(A);

    // Getting number of rows (equals to number of column) and send in to all processes
    int no_rows = A.rows();
    MPI_Bcast(&no_rows, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
    
    // Send a copy of matrix A to all processes
    Matrix A_local(no_rows, no_rows);
    if(rank == 0) {
        A_local = A;
    }
    MPI_Bcast(A_local.data(), no_rows * no_rows, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Gaussian elimination algorithm
    // Iterate through each row except the last one
    for (int i = 0; i < no_rows - 1; i++) {
        // Pointer to the i-th diagonal element
        double *diag_row = &A_local.data()[i * no_rows + i];
        // Iterate through the rows below the current diagonal row
        for (int j = i + 1; j < no_rows; j++) {
            // Check which process should perform operation on current row
            if (j % num_procs == rank) {
                // Get pointer to the current row - easier operations
                double *current_row = &A_local.data()[j * no_rows + i];

                // Calculate value used in the row operation for Gaussian elimination.
                double scaling_factor = current_row[0] / diag_row[0];
                
                // Preventing undefined values in a matrix.
                if(current_row[0] == 0)
                    break;

                // Updates the elements in the row
                for (int k = 1; k < no_rows - i; k++) {
                    (current_row)[k] = (current_row)[k] - scaling_factor * diag_row[k];
                }
                
                // Update first elements of the row to the scaling factor
                current_row[0] = scaling_factor;
            }
        }

        // Brodcast the updated rows to all other processes
        for (int j = i + 1; j < no_rows; j++) {
            MPI_Bcast(&A_local.data()[j * no_rows + i], no_rows - i, MPI_DOUBLE, j % num_procs, MPI_COMM_WORLD);
        }
    }
    
    // Assign the values from matrix A_local to the corresponding matrices L and U
    if(rank == 0) {
        for (int i = 0; i < no_rows; ++i) {
            for (int j = 0; j < no_rows; ++j) {
                if (j < i) {
                    L(i, j) = A_local(i, j);
                } else if (j == i) {
                    L(i, j) = 1;
                    U(i, j) = A_local(i, j);
                } else {
                    U(i, j) = A_local(i, j);
                }
            }
        }
    }
}

void LuSolver::validateMatrix(const Matrix& A) const {
    // Check if the matrix is square.
    if (A.rows() != A.cols()) {
        throw std::runtime_error("Matrix A must be square");
    }

    //Check if the matrix is nonsingular.
    for (int i = 0; i < A.rows(); i++) {
        if (std::fabs(A(i, i)) < 1e-12) {
            throw std::runtime_error("Matrix A is singular");
        }
    }
}

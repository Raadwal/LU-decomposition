#pragma once

#include <mpi.h>

#include "Matrix.h"

// A class representing an LU decomposition solver.
class LuSolver {
public:
    // Constructs a default LU solver.
    LuSolver() {}

    // Decomposes the matrix A into lower triangular matrix L and upper triangular matrix U using MPI.
    // The decomposition follows these steps:
    // If ( rank == row_index % no_processes ):
    //  1. Processes the row
    //  2. Broadcast the row
    void decompose(Matrix& A, Matrix& L, Matrix& U, int rank, int num_procs);

private:
    // Validates the input matrix A to ensure it is a square and nonsingular matrix
    void validateMatrix(const Matrix& A) const;
};
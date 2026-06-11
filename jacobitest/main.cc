#include <iostream>
#include <random>
#include <iomanip>
#include <tuple>

// Include your custom headers
#include "matrix.h"
#include "jacobi.h" // The complex solver with forced sorting math
#include "eigen.h"  // The real solver using atan2

int main() {
    // 1. Define the dimension of the n x n matrix
    const size_t n = 100; 
    const size_t n_vals = 5;
    
    // 2. Instantiate a REAL matrix using your library
    qm::rmat A(n, n);

    // 3. Setup a random number generator
    std::mt19937 gen(42);
    std::uniform_real_distribution<double> dist(-10.0, 10.0);

    // 4. Fill 'A' with random values symmetrically
    for (size_t i = 0; i < n; ++i) {
        A(i, i) = dist(gen);
        for (size_t j = i + 1; j < n; ++j) {
            double val = dist(gen);
            A(i, j) = val;
            A(j, i) = val; 
        }
    }

    // =========================================================
    // TEST 1: Complex Solver (jacobi.h)
    // =========================================================
    // Convert real matrix to complex.
    // We must pass a copy because jacobi_with_eigenvector modifies it in-place.
    qm::cmat Ac = qm::to_complex(A);
    
    auto [lowest_E_complex, lowest_eigvec_complex] = qm::jacobi_full(Ac, n_vals);

    std::cout << "=========================================\n";
    std::cout << " TEST 1: Complex Solver (jacobi.h)\n";
    std::cout << "=========================================\n";
    std::cout << "Lowest Eigenvalue: " << std::fixed << std::setprecision(4) << lowest_E_complex[0] << "\n";
    
    std::cout << "Corresponding Eigenvector (Real components):\n(";
    for (size_t i = 0; i < lowest_eigvec_complex[0].size(); ++i) {
        std::cout << std::setw(8) << std::real(lowest_eigvec_complex[0][i]);
        if (i + 1 < lowest_eigvec_complex[0].size()) std::cout << ", ";
    }
    std::cout << ")\n\n";

    std::cout << "All Eigenvalues:\n";
    for (size_t i = 0; i < n; ++i) {
        std::cout << "E[" << i << "] = " << lowest_E_complex[i] << "\n";
    }
    std::cout << "\n";

    // =========================================================
    // TEST 2: Real Solver (eigen.h)
    // =========================================================
    // We can pass A directly. eigen.h accepts it by value (rmat A), 
    // so it creates its own local copy and returns a tuple.
    auto [w, V] = qm::jacobi(A, n_vals);

    // Because eigen.h sorts automatically via atan2, the lowest is at index 0.
    double lowest_E_real = w[0];
    
    // V[0] grabs the first column (the corresponding eigenvector)
    qm::rvec lowest_eigvec_real = V[0];

    std::cout << "=========================================\n";
    std::cout << " TEST 2: Real Solver (eigen.h)\n";
    std::cout << "=========================================\n";
    std::cout << "Lowest Eigenvalue: " << std::fixed << std::setprecision(4) << lowest_E_real << "\n";
    
    std::cout << "Corresponding Eigenvector:\n(";
    for (size_t i = 0; i < lowest_eigvec_real.size(); ++i) {
        std::cout << std::setw(8) << lowest_eigvec_real[i];
        if (i + 1 < lowest_eigvec_real.size()) std::cout << ", ";
    }
    std::cout << ")\n\n";

    std::cout << "All Eigenvalues:\n";
    for (size_t i = 0; i < n; ++i) {
        std::cout << "E[" << i << "] = " << w[i] << "\n";
    }
    std::cout << "\n";


    return 0;
}
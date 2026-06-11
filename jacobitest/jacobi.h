#pragma once
#include <tuple>
#include <cmath>
#include <complex>
#include "matrix.h"

#define SELF (*this)

namespace qm {
    // Helper function to find lowest eigenvalue and eigenvector using Jacobi diagonalization
// Returns: pair of (eigenvalue, eigenvector) where eigenvector is normalized
std::tuple<rvec, cmat> jacobi_with_eigenvector(cmat& A, int max_sweeps = 100, size_t nvals = 0) {
    size_t n = A.size1();

    // If nvals is 0 or too large, sweep the whole matrix (up to n-1).
    size_t p_max = (nvals == 0 || nvals >= n) ? n - 1 : nvals;

    // Initialize eigenvector matrix as identity (tracks rotations)
    cmat V = eye<cld>(n);
    rvec w(n);

    // 1. Setup Phase: Only calculate average error for the rows we actually sweep!
    ld sum_off_diag = 0.0;
    for (size_t p = 0; p < p_max; ++p) {
        for (size_t q = p + 1; q < n; ++q) sum_off_diag += std::abs(A(p, q));
    }
    
    // Adjust denominator to match the number of elements we checked
    ld threshold = sum_off_diag / (p_max * n); 
    int total_rotations;

    for (int sweep = 0; sweep < max_sweeps; ++sweep) {
        ld max_off_diag = 0.0;
        total_rotations = 0; // Track if we actually did anything this sweep

        // PARTIAL Jacobi sweep: only go up to p_max
        for (size_t p = 0; p < p_max; ++p) {
            for (size_t q = p + 1; q < n; ++q) {
                ld off_diag_mag = std::abs(A(p, q));
                if (off_diag_mag > max_off_diag) max_off_diag = off_diag_mag;

                if (off_diag_mag > threshold) {
                    total_rotations++;
                    
                    // Calculate the rotation angles
                    ld app = std::real(A(p, p));
                    ld aqq = std::real(A(q, q));
                    cld apq = A(p, q);

                    ld tau = (aqq - app) / (2.0 * off_diag_mag);

                    ld t;
                    if (tau >= 0.0) {
                        t = 1.0 / (tau + std::sqrt(1.0 + tau * tau));
                    } else {
                        t = -tau + std::sqrt(1.0 + tau * tau);
                    }

                    ld cos_t = 1.0 / std::sqrt(1.0 + t * t);
                    ld sin_t = t * cos_t;
                    cld phase = std::conj(apq) / off_diag_mag; // Phase to handle complex elements

                    // Apply Givens rotation to A
                    for (size_t i = 0; i < n; ++i) {
                        cld ip = A(i, p);
                        cld iq = A(i, q);
                        A(i, p) = cos_t * ip - sin_t * phase * iq;
                        A(i, q) = sin_t * std::conj(phase) * ip + cos_t * iq;
                    }
                    for (size_t i = 0; i < n; ++i) {
                        cld pi = A(p, i);
                        cld qi = A(q, i);
                        A(p, i) = cos_t * pi - sin_t * std::conj(phase) * qi;
                        A(q, i) = sin_t * phase * pi + cos_t * qi;
                    }

                    // Apply same rotation to eigenvector matrix V
                    for (size_t i = 0; i < n; ++i) {
                        cld vip = V(i, p);
                        cld viq = V(i, q);
                        V(i, p) = cos_t * vip - sin_t * phase * viq;
                        V(i, q) = sin_t * std::conj(phase) * vip + cos_t * viq;
                    }
                }
            }
        }
        
        // 3. Lower the threshold for the next sweep
        if (threshold > ZERO_LIMIT) {
            threshold *= 0.2; 
        }

        // 4. Convergence Check (We hit the floor AND found no errors)
        if (max_off_diag < ZERO_LIMIT && total_rotations == 0) {
            break; 
        }    
    }


    // Extract the strictly real eigenvalues from the sorted diagonal
    for(size_t i = 0; i < n; i++) {
        w[i] = std::real(A(i, i));
    }
    
    return std::make_tuple(w, V);
}

// Jacobi diagonalization of Hermitian matrix A.
// Iteratively zeroes off-diagonal elements via complex Givens rotations.
// Uses atan2 formulation to naturally sort eigenvalues (smallest to index 0).
std::tuple<rvec, cmat> jacobi(cmat A, int nvals = 0) {
    int n = A.size1();
    if (nvals == 0) nvals = n;

    rvec w(n);
    cmat V(n, n);
    V.setid(); // Initialize to identity matrix

    bool changed;
    do {
        changed = false;
        for (int p = 0; p < nvals; p++) {
            for (int q = p + 1; q < n; q++) {
                
                cld apq = A(p, q);
                ld mag_apq = std::abs(apq);
                
                // Skip if the off-diagonal element is effectively zero
                if (mag_apq < 1e-15) continue; 

                // Diagonal elements of a Hermitian matrix are strictly real
                ld app = std::real(A(p, p));
                ld aqq = std::real(A(q, q));

                // Restored your original atan2 logic for natural sorting!
                // We use mag_apq as the effective real off-diagonal magnitude.
                ld phi = 0.5 * std::atan2(2.0 * mag_apq, aqq - app);
                ld c = std::cos(phi);
                ld s = std::sin(phi);
                
                // Extract the complex phase of the target element (e^{i * gamma})
                cld phase = apq / mag_apq; 

                // Determine new diagonal elements
                ld app1 = c * c * app - 2.0 * s * c * mag_apq + s * s * aqq;
                ld aqq1 = s * s * app + 2.0 * s * c * mag_apq + c * c * aqq;

                // If precision limits allow a change, apply the rotation
                if (app1 != app || aqq1 != aqq) {
                    changed = true;
                    A(p, p) = app1;
                    A(q, q) = aqq1;
                    A(p, q) = 0.0;
                    A(q, p) = 0.0;

                    // Update off-diagonal elements uniformly (using complex phase)
                    for (int i = 0; i < n; i++) {
                        if (i == p || i == q) continue;
                        
                        cld aip = A(i, p);
                        cld aiq = A(i, q);
                        
                        A(i, p) = c * aip - s * phase * aiq;
                        A(i, q) = s * std::conj(phase) * aip + c * aiq;
                        
                        // Enforce strict Hermitian symmetry on the lower triangle
                        A(p, i) = std::conj(A(i, p));
                        A(q, i) = std::conj(A(i, q));
                    }

                    // Apply same rotation to eigenvector matrix V
                    for (int i = 0; i < n; i++) {
                        cld vip = V(i, p);
                        cld viq = V(i, q);
                        
                        V(i, p) = c * vip - s * phase * viq;
                        V(i, q) = s * std::conj(phase) * vip + c * viq;
                    }
                }
            }
        }
    } while (changed);
    
    // Extract the strictly real eigenvalues from the sorted diagonal
    for(int i = 0; i < n; i++) {
        w[i] = std::real(A(i, i));
    }
    
    return std::make_tuple(w, V);
}
}
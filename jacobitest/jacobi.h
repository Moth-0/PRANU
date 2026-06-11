#pragma once
#include <tuple>
#include <cmath>
#include <complex>
#include "matrix.h"

#define SELF (*this)

namespace qm {
std::tuple<rvec, cmat> jacobi_full(cmat A, int max_sweeps = 1000, size_t nvals = 0) {
    size_t n = A.size1();
    size_t p_max = (nvals == 0 || nvals >= n) ? n - 1 : nvals;

    rvec w(n);
    cmat V = eye<cld>(n);

    ld sum_off_diag = 0.0;
    for (size_t p = 0; p < p_max; ++p) {
        for (size_t q = p + 1; q < n; ++q) sum_off_diag += std::abs(A(p, q));
    }
    
    ld threshold = sum_off_diag / (p_max * n); 
    int total_rotations;

    for (int sweep = 0; sweep < max_sweeps; ++sweep) {
        ld max_off_diag = 0.0;
        total_rotations = 0;

        for (size_t p = 0; p < p_max; ++p) {
            for (size_t q = p + 1; q < n; ++q) {
                
                cld apq = A(p, q);
                ld mag_apq = std::abs(apq);
                if (mag_apq > max_off_diag) max_off_diag = mag_apq;

                if (mag_apq > threshold && mag_apq > 1e-15) {
                    total_rotations++;
                    
                    ld app = std::real(A(p, p));
                    ld aqq = std::real(A(q, q));

                    ld phi = 0.5 * std::atan2(2.0 * mag_apq, aqq - app);
                    ld c = std::cos(phi);
                    ld s = std::sin(phi);
                    
                    cld phase = apq / mag_apq;

                    ld app1 = c * c * app - 2.0 * s * c * mag_apq + s * s * aqq;
                    ld aqq1 = s * s * app + 2.0 * s * c * mag_apq + c * c * aqq;

                    if (app1 != app || aqq1 != aqq) {
                        A(p, p) = app1;
                        A(q, q) = aqq1;
                        A(p, q) = 0.0;
                        A(q, p) = 0.0;

                        for (size_t i = 0; i < n; ++i) {
                            if (i == p || i == q) continue;
                            
                            cld aip = A(i, p);
                            cld aiq = A(i, q);
                            
                            A(i, p) = c * aip - s * std::conj(phase) * aiq;
                            A(i, q) = s * phase * aip + c * aiq;
                            
                            A(p, i) = std::conj(A(i, p));
                            A(q, i) = std::conj(A(i, q));
                        }

                        for (size_t i = 0; i < n; ++i) {
                            cld vip = V(i, p);
                            cld viq = V(i, q);
                            
                            V(i, p) = c * vip - s * std::conj(phase) * viq;
                            V(i, q) = s * phase * vip + c * viq;
                        }
                    }
                }
            }
        }
        
        if (threshold > 1e-15) threshold *= 0.2; 
        if (max_off_diag < 1e-15 && total_rotations == 0) break;     
    }

    // Extract all real eigenvalues from the diagonal
    // The first 'p_max' elements will be sorted from lowest to highest.
    for(size_t i = 0; i < n; ++i) {
        w[i] = std::real(A(i, i));
    }
    
    return std::make_tuple(w, V);
}

} // namespace qm
#pragma once
#include "matrix.h"

#define SELF (*this)

// Jacobi diagonalization of symmetric matrix M.
// Iteratively zeroes off-diagonal elements via Givens rotations.
// Returns eigenvalues (diagonal of rotated M) and eigenvector matrix V.
namespace qm {
std::tuple<rvec,rmat> jacobi(rmat A, int nvals = 0) {
    int n = A.size1();
    if (nvals == 0) nvals = n;

    rvec w(n);
    rmat V(n, n);
	V.setid();

    bool changed;
    do {
        changed = false;
        for (int p = 0; p < nvals; p++) {
            for (int q = p + 1; q < n; q++) {
                ld app = A(p,p);
                ld aqq = A(q,q);
                ld apq = A(p, q);
                ld phi = 0.5 * std::atan2(2 * apq, aqq - app);
                ld c = std::cos(phi);
                ld s = std::sin(phi);
                ld app1 = c * c * app - 2 * s * c * apq + s * s * aqq;
                ld aqq1 = s * s * app + 2 * s * c * apq + c * c * aqq;

                if (app1 != app || aqq1 != aqq) {
                    changed = true;
                    A(p,p) = app1;
                    A(q,q) = aqq1;
                    A(p,q) = 0.0;

                    for (int i = 0; i < p; i++) {
                        ld aip = A(i, p);
                        ld aiq = A(i, q);
                        A(i, p) = c * aip - s * aiq;
                        A(i, q) = c * aiq + s * aip;
                    }

                    for (int i = p + 1; i < q; i++) {
                        ld api = A(p, i);
                        ld aiq = A(i, q);
                        A(p, i) = c * api - s * aiq;
                        A(i, q) = c * aiq + s * api;
                    }

                    for (int i = q + 1; i < n; i++) {
                        ld api = A(p, i);
                        ld aqi = A(q, i);
                        A(p, i) = c * api - s * aqi;
                        A(q, i) = c * aqi + s * api;
                    }

                    for (int i = 0; i < n; i++) {
                        ld vip = V(i, p);
                        ld viq = V(i, q);
                        V(i, p) = c * vip - s * viq;
                        V(i, q) = c * viq + s * vip;
                    }
                }
            }
        }
    } while (changed);
	for(int i=0;i<n;i++) w[i]=A(i,i);
    return std::make_tuple(w,V);
}
}
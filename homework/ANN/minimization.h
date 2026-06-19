# pragma once 
# include <functional>
# include "matrix.h"
# include "QR.h"

namespace pp {

using func = std::function<double(vector)>;

vector gradient(const func& f, vector x) {
    double fx = f(x);
    size_t n = x.size();
    vector gf(n);
    double dxi; 

    for(size_t i=0; i<n; i++){
        dxi = (1+std::abs(x[i]))*std::pow(2.0, -26.0);
        x[i] += dxi;
        gf[i] = (f(x) - fx)/dxi;
        x[i] -= dxi; 
    }

    return gf;
}

matrix hessian(const func& f, vector x){
    size_t n = x.size();
    matrix H(n,n);
    vector gfx = gradient(f, x);
    vector dgf(n);
    double dxj; 

    for(size_t j=0; j<n; j++) {
        dxj = (1+std::abs(x[j]))*std::pow(2.0, -13.0);
        x[j] += dxj;
        dgf = gradient(f, x) - gfx;
        for(size_t i=0; i<n; i++) H(i,j) = dgf[i]/dxj;
        x[j] -= dxj;
    }

    return H;
}

vector newton(const func& f, vector x, double acc = 1e-3, bool verbose = false) {
    size_t n = x.size();
    vector g(n);
    matrix H(n, n);
    vector dx(n);
    int steps = 0;

    while(steps < 20000) {
        steps++;
        g = gradient(f, x);
        if(g.norm() < acc) break;
        
        H = hessian(f, x);

        for(size_t i=0; i<n; i++) H(i,i) += 1e-2;
        dx = QR(H).solve(-g);

        // --- NEW: SANITIZE THE MATH ---
        // 1. Force the vector to point downhill (flip it if it's uphill)
        double dot_val = 0;
        for(size_t i=0; i<n; i++) dot_val += g[i]*dx[i];
        if (dot_val > 0) {
            for(size_t i=0; i<n; i++) dx[i] = -dx[i];
        }

        // 2. Put a strict speed limit on it 
        double norm_dx = dx.norm();
        if (norm_dx > 1.0) {
            for(size_t i=0; i<n; i++) dx[i] *= (1.0 / norm_dx);
        }
        // ------------------------------

        double fx = f(x);
        double λ = 1.0;

        while(λ >= 1.0/1024.0) {
            if(f(x+λ*dx) < fx) break;
            λ *= 0.5;
        }
        x += λ*dx;
    }

    if (verbose) std::cout << "Minimization steps: " << steps << "\n";
    return x;
};

} // namespace pp
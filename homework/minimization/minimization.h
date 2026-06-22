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

    while(steps < 100000) {
        steps++;
        g = gradient(f, x);
        if(g.norm() < acc) break;
        H = hessian(f, x);

        for(size_t i=0; i<n; i++) H(i,i) += 1e-6;
        dx = QR(H).solve(-g);

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

// Part C ----------------------
void eval_grad_hess_central(const func& f, vector x, vector& g, matrix& H) {
    double fx = f(x);
    size_t n = x.size();
    g = vector(n);
    H = matrix(n, n);
    
    std::vector<double> dx(n);
    std::vector<double> f_plus(n), f_minus(n);
    
    // Optimal delta x = epsilon^(1/3)
    double optimal_pow = std::pow(2.0, -17.0); 

    // 1. Calculate the Gradient and the Diagonal of the Hessian
    for(size_t i=0; i<n; i++) {
        dx[i] = (1 + std::abs(x[i])) * optimal_pow;
        
        x[i] += dx[i];
        f_plus[i] = f(x);
        
        x[i] -= 2.0 * dx[i];
        f_minus[i] = f(x);
        
        x[i] += dx[i]; // Restore x[i]

        // Central Difference Gradient
        g[i] = (f_plus[i] - f_minus[i]) / (2.0 * dx[i]);
        
        // Central Difference Hessian (Diagonal)
        H(i, i) = (f_plus[i] - 2.0 * fx + f_minus[i]) / (dx[i] * dx[i]);
    }

    // 2. Calculate Off-Diagonal Elements of the Hessian
    for(size_t i=0; i<n; i++) {
        for(size_t j=i+1; j<n; j++) {
            x[i] += dx[i]; x[j] += dx[j];
            double f_pp = f(x);
            
            x[j] -= 2.0 * dx[j];
            double f_pm = f(x);
            
            x[i] -= 2.0 * dx[i];
            double f_mm = f(x);
            
            x[j] += 2.0 * dx[j];
            double f_mp = f(x);
            
            x[i] += dx[i]; x[j] -= dx[j]; // Restore
            
            // Central Difference Hessian (Mixed Partials)
            H(i, j) = (f_pp - f_pm - f_mp + f_mm) / (4.0 * dx[i] * dx[j]);
            H(j, i) = H(i, j); // The Hessian is symmetric
        }
    }
}

vector newton_central(const func& f, vector x, double acc = 1e-3, bool verbose = false) {
    size_t n = x.size();
    vector g(n);
    matrix H(n, n);
    vector dx(n);
    int steps = 0;

    while(steps < 100000) {
        steps++;
        
        // Populate g and H simultaneously
        eval_grad_hess_central(f, x, g, H);
        
        if(g.norm() < acc) break;

        for(size_t i=0; i<n; i++) H(i,i) += 1e-6; // Regularization
        dx = QR(H).solve(-g);

        double fx = f(x);
        double λ = 1.0;

        while(λ >= 1.0/1024.0) {
            if(f(x+λ*dx) < fx) break;
            λ *= 0.5;
        }
        x += λ*dx;
    }

    if (verbose) std::cout << "Central Minimization steps: " << steps << "\n";
    return x;
}

} // namespace pp
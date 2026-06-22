#pragma once

#include "matrix.h"
#include "QR.h"

namespace pp {

using Func = std::function<double(vector)>;
using Func_list = std::vector<Func>;

// Helper to evaluate the list of functions f at point x
vector eval(const Func_list& flist, const vector& x) {
    vector res(flist.size());
    for(size_t i = 0; i < flist.size(); ++i) {
        res[i] = flist[i](x);
    }
    return res;
}

/* Part A implimentataion 
matrix jacobian(const Func_list& f, vector x, const vector& fx) {
    size_t dim = x.size();
    matrix J(dim, dim);

    for(size_t i=0; i<dim; i++) {
        double dx = std::abs(x[i])*std::pow(2,-26);
        //if(dx < ZERO_LIMIT) dx = ZERO_LIMIT;
        x[i] += dx;
        vector df = eval(f, x)-fx;
        for(size_t j=0; j<dim; j++){
            J(j,i) = df[j]/dx;
        }
        x[i] -= dx;
    }

    return J;
}

vector newton (const Func_list& f, vector x, double acc = 1e-3, double alpha_min = 1e-3, int max_iter = 200){
    vector fx = eval(f, x); 
    for(int i=0; i<max_iter; i++){
        if (fx.norm() < acc) break;
        matrix J = jacobian(f, x, fx);
        QR QRJ(J);
        vector Dx = QRJ.solve(-fx);
        double alpha = 1.0;

        vector z;
        vector fz;
        while(true) {
            z = x + alpha * Dx;
            fz = eval(f, z);
            // std::cerr << fx.norm() << "\n"; 
            if(fz.norm() < fx.norm()) break;
            if(alpha < alpha_min) {std::cerr << "failed\n"; break;}
            alpha /= 2.0; 
        }

        x = z; 
        fx = fz;
    }

    return x;
}
*/

// Part C update

// OPTIMIZATION 1: Pass pre-allocated J by reference
void update_jacobian(const Func_list& f, vector x, const vector& fx, matrix& J) {
    size_t dim = x.size();

    for(size_t i=0; i<dim; i++) {
        double dx = std::abs(x[i])*std::pow(2,-26);
        if(dx == 0) dx = std::pow(2,-26); // Guard against exactly 0
        
        x[i] += dx;
        vector df = eval(f, x) - fx;
        for(size_t j=0; j<dim; j++){
            J(j,i) = df[j]/dx;
        }
        x[i] -= dx;
    }
}

vector newton (const Func_list& f, vector x, double acc = 1e-3, double alpha_min = 1e-3, int max_iter = 200) {
    size_t dim = x.size();
    matrix J(dim, dim); // Allocate the Jacobian matrix exactly ONCE
    
    vector fx = eval(f, x); 
    
    for(int i=0; i<max_iter; i++) {
        double norm_fx = fx.norm();
        if (norm_fx < acc) break;
        
        // Update the existing matrix in-place
        update_jacobian(f, x, fx, J);
        
        QR QRJ(J);
        vector Dx = QRJ.solve(-fx);
        
        double alpha = 1.0;
        vector z;
        vector fz;
        
        double g0 = 0.5 * norm_fx * norm_fx;
        
        while(true) {
            z = x + alpha * Dx;
            fz = eval(f, z);
            double norm_fz = fz.norm();
            
            if(norm_fz < norm_fx) break; 
            
            // OPTIMIZATION 2: Quadratic interpolation
            double g_alpha = 0.5 * norm_fz * norm_fz;
            double c = (g_alpha - g0 + 2.0 * alpha * g0) / (alpha * alpha);
            double alpha_new = g0 / c;
            
            // Fallback: If the new alpha is too small or larger than half the current, 
            // the parabola guess was bad. Fall back to standard halving.
            if (alpha_new < alpha / 10.0 || alpha_new > alpha / 2.0) {
                alpha /= 2.0;
            } else {
                alpha = alpha_new;
            }
        }

        x = z; 
        fx = fz;
    }

    return x;
}

} // namespace pp
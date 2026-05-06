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


matrix jacobian(Func_list f, vector x, vector fx) {
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

vector newton (Func_list f, vector x, double acc = 1e-3, double alpha_min = 1e-3, int max_iter = 200){
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

} // namespace pp
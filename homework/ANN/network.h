#pragma once 

#include <functional>

#include "matrix.h"
#include "minimization.h"

namespace pp {

using func = std::function<double(vector)>;

struct network {
    // This network uses the wavelet as activation function 
    // x*exp(-x*x)

    size_t n; // number of hidden neurons 
    vector params; // parameter linear matrix {a_1,b_1,w_1,a_2,...}

    // Activation
    network(size_t hidden_number){
        n = hidden_number;
        params = vector(3*n);
        for(size_t i = 0; i < n; i++) {
            // 'a' (shift): Spread evenly between -1 and 1
            params[3*i + 0] = -1.0 + 2.0 * i / (n > 1 ? n - 1.0 : 1.0); 
            // 'b' (width)
            params[3*i + 1] = 2.0; 
            // 'w' (weight): 
            params[3*i + 2] = 1.0; 
        }
    };

    network()  = default;
    ~network() = default;

    double respond(double x, const vector& p) {
        double sum = 0; 
        for(size_t i=0; i<n; i++){
            double a = p[3*i+0];
            double b = p[3*i+1];
            double w = p[3*i+2];

            double z = (x - a) / b;
            sum += w * z * std::exp(-z * z);
        }
        return sum;
    }

    double respond(double x) {
        return respond(x, params);
    }

    double cost(const vector& p, const vector& x, const vector& y) {
        double sum = 0;
        for(size_t k=0; k<x.size(); k++) {
            double val = respond(x[k], p) - y[k];
            sum += val*val;
        }
        return sum;
    }

    void train(const vector& x, const vector& y) {
        auto objective = [this, x, y](vector param_guess) -> double {
            return cost(param_guess, x, y); 
        };

        std::cout << "--- STARTING TRAINING ---\n";     
        params = newton(objective, params, 1e-3, true);
    }

    // --- Part B ---
    double derivative(double x) {
        double sum = 0; 
        for(size_t i=0; i<n; i++){
            double a = params[3*i+0];
            double b = params[3*i+1];
            double w = params[3*i+2];

            double z = (x - a) / b;
            double df_dz = (1.0 - 2.0 * z * z) * std::exp(-z * z);
            sum += w * (1.0 / b) * df_dz;
        }
        return sum;
    }

    double second_derivative(double x) {
        double sum = 0; 
        for(size_t i=0; i<n; i++){
            double a = params[3*i+0];
            double b = params[3*i+1];
            double w = params[3*i+2];

            double z = (x - a) / b;
            double ddf_dz = (4.0 * z * z * z - 6.0 * z) * std::exp(-z * z);
            sum += w * (1.0 / (b * b)) * ddf_dz;
        }
        return sum;
    }
    
    double antiderivative(double x) {
        double sum = 0; 
        for(size_t i=0; i<n; i++){
            double a = params[3*i+0];
            double b = params[3*i+1];
            double w = params[3*i+2];
            if (std::abs(b) < 1e-8) b = 1e-8; 

            double z = (x - a) / b;
            
            // Network u-substitution: multiply by b
            double int_f = -0.5 * std::exp(-z * z);
            sum += w * b * int_f;
        }
        return sum;
    }
};

} // namespace
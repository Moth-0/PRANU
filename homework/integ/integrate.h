#pragma once

#include <vector>
#include <functional>
#include <cmath>

using vector = std::vector<double>;

struct IntResult {
    double val;
    double err;
};

IntResult integrate(const std::function<double(double)>& f, double a, double b,
                                    double acc=1e-4, double eps=1e-4, double f2=NAN, double f3=NAN
){
    double h = b-a; 
    if(std::isnan(f2)) {f2 = f(a+2*h/6); f3 = f(a+4*h/6); }

    double f1 = f(a+h/6); double f4 = f(a+5*h/6);
    double Q = (2*f1 + f2 + f3 + 2*f4)/6*h;     //Higher order
    double q = (f1 + f2 + f3 + f4 )/4*h;        // Lower Order
    
    double err = std::abs(Q-q);
    if (err <= acc+eps*std::abs(Q)) return {Q, err};
    else {
        auto [Q_left, err_left]   = integrate(f,a,(a+b)/2,acc/std::sqrt(2),eps,f1,f2);
        auto [Q_right, err_right] = integrate(f,(a+b)/2,b,acc/std::sqrt(2),eps,f3,f4);
        return {Q_left+Q_right, std::sqrt(err_left*err_left+err_right*err_right)};
    }
}

IntResult Clenshaw_Curtis(const std::function<double(double)>& f, double a, double b, 
                                          double acc=1e-4, double eps=1e-4
){

    // Infinity transformations
    if (a == - INFINITY && b == INFINITY) {
        auto f_map = [&f](double t) -> double {
            double term = (1-t)/t;
            return (f(term)+f(-term))/t/t;
        };
        return Clenshaw_Curtis(f_map, 0.0, 1.0, acc, eps);
    }
    else if (a == -INFINITY) {
        auto f_map = [&f, a, b](double t) -> double {
            double term = (1-t)/t;
            return f(b-term)/t/t;
        };
        return Clenshaw_Curtis(f_map, 0.0, 1.0, acc, eps);
    }
    else if (b == INFINITY) {
        auto f_map = [&f, a, b](double t) -> double {
            double term = (1-t)/t;
            return f(a+term)/t/t;
        };
        return Clenshaw_Curtis(f_map, 0.0, 1.0, acc, eps);
    }


    // Clenshaw_Curtis transformation
    auto new_f = [f, a, b](double t) -> double {
        return f(0.5*(a+b)+0.5*(b-a)*std::cos(t)) * 0.5*(b-a)*std::sin(t);
    };
    return integrate(new_f, 0.0, M_PI, acc, eps);
}
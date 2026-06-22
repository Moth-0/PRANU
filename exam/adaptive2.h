#pragma once

#include <functional>
#include <cmath>

namespace pp {

std::tuple<double, double> adaptive2(const std::function<double(double)>& f, 
                                     double a, double b,
                                     double acc=1e-4, double eps=1e-4, 
                                     double f2=NAN, double f3=NAN
){
    double h = b-a; 
    if(std::isnan(f2)) {f2 = f(a+2*h/6); f3 = f(a+4*h/6); }

    double f1 = f(a+h/6); double f4 = f(a+5*h/6);
    double Q = (2*f1 + f2 + f3 + 2*f4)/6*h;     //Higher order
    double q = (f1 + f2 + f3 + f4 )/4*h;        // Lower Order
    
    double err = std::abs(Q-q);
    if (err <= acc+eps*std::abs(Q)) return {Q, err};
    else {
        auto [Q_left, err_left]   = adaptive2(f,a,(a+b)/2,acc/std::sqrt(2),eps,f1,f2);
        auto [Q_right, err_right] = adaptive2(f,(a+b)/2,b,acc/std::sqrt(2),eps,f3,f4);
        return {Q_left+Q_right, std::sqrt(err_left*err_left+err_right*err_right)};
    }
}

} // namespace pp
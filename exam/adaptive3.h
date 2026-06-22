#pragma once

#include <functional>
#include<cmath>


namespace pp {

std::tuple<double, double> adaptive3(const std::function<double(double)>& f, 
                                     double a, double b,
                                     double acc=1e-4, double eps=1e-4, 
                                     double f2=NAN
){
    double h = b-a; 
    // If first call get the middle point 
    if(std::isnan(f2)) {f2 = f(a+3*h/6);}

    // Calculate thetwo other points
    double f1 = f(a+h/6); double f3 = f(a+5*h/6);

    // Higher order using wi = {3/8, 2/8, 3/8}
    double Q = (3*f1 + 2*f2 + 3*f3) * h/8; 
    // Lower order using vi = {1/3, 1/3, 1/3}
    double q = (f1 + f2 + f3) * h/3; 
    
    double err = std::abs(Q-q);
    // Check if have hit the acepted accuracy
    if (err <= acc+eps*std::abs(Q)) return {Q, err};
    
    // Recursion limit test
    if (a + h/3 == a) {
        return {Q, err};
    }
    
    else {
        // Split into three more calls, passing the three values to simplify calculations 
        auto [Q_left, err_left]   = adaptive3(f, a, a+h/3, acc/std::sqrt(3), eps, f1);
        auto [Q_mid, err_mid]     = adaptive3(f, a+h/3, a+2*h/3, acc/std::sqrt(3), eps, f2);
        auto [Q_right, err_right] = adaptive3(f, a+2*h/3, b, acc/std::sqrt(3), eps, f3);
        
        return {Q_left+Q_mid+Q_right, std::sqrt(err_left*err_left + err_mid*err_mid + err_right*err_right)};
    }
}

} // namespace pp
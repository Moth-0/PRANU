#pragma once 

#include <functional>

namespace pp {

template <typename Integrator>
std::tuple<double, double> ClenshawCurtis(const std::function<double(double)>& f, 
                                          Integrator integrator,
                                          double a, double b, 
                                          double acc=1e-4, double eps=1e-4

){
    // Clenshaw_Curtis transformation
    auto new_f = [f, a, b](double t) -> double {
        return f(0.5*(a+b)+0.5*(b-a)*std::cos(t)) * 0.5*(b-a)*std::sin(t);
    };
    return integrator(new_f, 0.0, M_PI, acc, eps);
}

} // namespace pp
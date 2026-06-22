#pragma once

#include <functional>
#include <cmath>

namespace pp {

template <typename Integrator>
std::tuple<double, double> adaptive_inf(const std::function<double(double)>& f, 
                                         Integrator integrator,
                                         double a, double b, 
                                         double acc=1e-4, double eps=1e-4
){
    // Infinity transformations
    if (a == - INFINITY && b == INFINITY) {
        auto f_map = [&f](double t) -> double {
            double term = (1-t)/t;
            return (f(term)+f(-term))/t/t;
        };
        return integrator(f_map, 0.0, 1.0, acc, eps);
    }
    else if (a == -INFINITY) {
        auto f_map = [&f, a, b](double t) -> double {
            double term = (1-t)/t;
            return f(b-term)/t/t;
        };
        return integrator(f_map, 0.0, 1.0, acc, eps);
    }
    else if (b == INFINITY) {
        auto f_map = [&f, a, b](double t) -> double {
            double term = (1-t)/t;
            return f(a+term)/t/t;
        };
        return integrator(f_map, 0.0, 1.0, acc, eps);
    }
    else {
        return integrator(f, a, b, acc, eps);
    }
}

} // namespace pp
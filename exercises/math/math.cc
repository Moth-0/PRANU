#include <iostream>
#include <numbers>
#include <cmath>
#include <complex>
#include "sfuns.h"




int main() {
    double pi = std::numbers::pi;
    double eu = std::numbers::e;
    using complex = std::complex<double>;
    complex i(0,1);

    double a = std::sqrt(2);
    double b = std::pow(2, 0.2);
    double c = std::exp(pi);
    complex d = std::exp(i);
    double e = std::pow(pi, eu);
    complex f = std::pow(pi, i);
    complex g = std::pow(i, i);
    complex h = std::log(i);

    std::cout << "Sqrt(2) = " << a << std::endl;
    std::cout << "2^1/5 = " << b << std::endl;
    std::cout << "e^π = " << c << std::endl;
    std::cout << "e^i = " << d << std::endl;
    std::cout << "π^e = " << e << std::endl;
    std::cout << "π^i = " << f << std::endl;
    std::cout << "i^i = " << g << std::endl;
    std::cout << "log(i) = " << h << std::endl;
    std::cout << std::endl;

    for (double i = 1; i <= 10; i++) {
        std::cout << "Γ" << i << " = " << sfuns::fgamma(i) << std::endl;
    }
    return 0;
}
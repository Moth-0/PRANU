#include<iostream>
#include<limits>
#include<cmath>
#include<iomanip>

int main() {
    float f = 1.0;
    double d = 1.0;
    long double l = 1.0;

    while ((float) 1.0+f != 1.0) {
        f /= 2.0;
    }
    f *= 2.0;

    while ((double) 1.0+d != 1.0) {
        d /= 2.0;
    }
    d *= 2.0;

    while ((long double) 1.0+l != 1.0) {
        l /= 2.0;
    }
    l *= 2.0;

    std::cout << "float eps= " << f << std::endl;
    std::cout << "double eps= " << d << std::endl;
    std::cout << "long double eps= " << l << std::endl;

    std::cout << "\n Numeric limits: " << std::endl;
    std::cout << std::numeric_limits<float>::epsilon() << std::endl;
    std::cout << std::numeric_limits<double>::epsilon() << std::endl;
    std::cout << std::numeric_limits<long double>::epsilon() << std::endl;

    std::cout << "\n Power limits: " << std::endl;
    std::cout << "float pow(2, -23) = " << std::pow(2, -23) << std::endl;
    std::cout << "double pow(2, -52) = " << std::pow(2, -52) << std::endl;

    // Part 2
    double epsilon = std::pow(2,-52);
    double tiny = epsilon/2;
    double a = 1+tiny+tiny;
    double b = tiny+tiny+1;
    std::cout << "a==b ? " << (a==b ? "true":"false") << "\n";
    std::cout << "a>1  ? " << (a>1  ? "true":"false") << "\n";
    std::cout << "b>1  ? " << (b>1  ? "true":"false") << "\n";


    std::cout << std::fixed << std::setprecision(17);
    std::cout << "       tiny=" << tiny << "\n";
    std::cout << "1+tiny+tiny=" << a << "\n";
    std::cout << "tiny+tiny+1=" << b << "\n";

    return 0;
}
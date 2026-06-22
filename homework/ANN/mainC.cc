#include <iostream>
#include <fstream>
#include "network.h"

using namespace pp;

using ode_func = std::function<double(double, double, double, double)>;

int main() {
    network ann(4); 

    // Phi(x, y, y', y'') = 0
    // For y'' + y = 0, Phi is just (ddy + y)
    ode_func oscillator = [](double x, double y, double dy, double ddy) {
        return ddy + y;
    };

    // 3. Problem Parameters
    double a = 0.0;       // Start of domain
    double b = 6.28;      // End of domain (2*pi)
    double c = 0.0;       // Boundary condition location
    double yc = 1.0;      // y(0) = 1
    double dyc = 0.0;     // y'(0) = 0

    // (alpha=100, beta=100, N=200 default parameters are used)
    ann.train_ode(oscillator, a, b, c, yc, dyc);

    // 5. Output the results
    std::ofstream out("ode_results.data");
    for(double x = a; x <= b; x += 0.1) {
        out << x << " " << ann.respond(x) << "\n";
    }
    out.close();

    return 0;
}
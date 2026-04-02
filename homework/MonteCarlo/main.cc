#include <iostream>
#include <fstream> 

#include "montecarlo.h"

int main() {
    // --- Part A ---
    auto unit_circ = [](vector x) -> double {
        if (x[0]*x[0] + x[1]*x[1] <= 1.0) return 1.0;
        return 0.0;
    };


    std::ofstream out1("circ.data");
    out1 << "N val err \n";
    vector a = {-1.0, -1.0}; vector b = {1.0, 1.0};
    for (int N=1e2; N<1e7; N*=2) {
        auto [val, err] = plainmc(unit_circ, a, b, N);
        out1 << N << " " << val << " " << err << "\n";
    }
    
    // Watson Triple Integral
    auto watson = [](vector x) -> double {
        double term = 1; for(int i=0; i<3; i++) term *= cos(x[i]);
        return 1/(1-term)/M_PI/M_PI/M_PI;
    };

    vector a2 = {1e-8, 1e-8, 1e-8}; vector b2 = {M_PI, M_PI, M_PI};
    int N = 5e6;
    auto [val, err] = plainmc(watson, a2, b2, N);
    double exact = 1.393203929685676859;

    std::cout << " - Watson Triple Integral \n";
    std::cout << "Estimated: " << val << std::endl;
    std::cout << "Exact:     " << exact << std::endl;
    std::cout << "Est Error: " << err << std::endl;
    std::cout << "Act Error: " << std::abs(exact - val) << std::endl;
    return 0;
}
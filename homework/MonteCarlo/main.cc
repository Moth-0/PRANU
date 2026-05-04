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
    
    // Elipsoid 
    double ep_a=1.0, ep_b=2.0, ep_c=3.0;
    auto elipsoid = [ep_a,ep_b,ep_c](vector x) -> double {
        double value = x[0]*x[0]/(ep_a*ep_a) + x[1]*x[1]/(ep_b*ep_b) + x[2]*x[2]/(ep_c*ep_c);
        return (value <= 1.0) ? 1.0 : 0.0;
    };

    vector a2 = {-ep_a, -ep_b, -ep_c}; vector b2 = {ep_a, ep_b, ep_c};
    int N = 5e6;
    auto [val, err] = plainmc(elipsoid, a2, b2, N);
    double exact = 4.0/3.0 * M_PI * ep_a * ep_b * ep_c;

    std::cout << " - Ellipsoid Volume Integral \n";
    std::cout << "Estimated: " << val << std::endl;
    std::cout << "Exact:     " << exact << std::endl;
    std::cout << "Est Error: " << err << std::endl;
    std::cout << "Act Error: " << std::abs(exact - val) << std::endl;


    // --- Part B ---
    auto sine_wave = [](vector x) -> double {
        return std::sin(x[0])*std::sin(x[1]);
    }; 

    // boundaries: 0 to Pi
    vector a3 = {0.0, 0.0}; vector b3 = {M_PI, M_PI};

    double exact3 = 4.0; // Actual integral value
    std::ofstream out2("sine.data");
    out2 << "N err_plain err_quasi \n";
    for (int N=1e2; N<1e7; N*=2) {
        auto [valp, ep] = plainmc(sine_wave, a3, b3, N);
        auto [valq, eq] = quacimc(sine_wave, a3, b3, N);
        double err_p = std::abs(exact3-valp); double err_q = std::abs(exact3-valq); 
        out2 << N << " " << err_p << " " << err_q << "\n";
    }

    auto cos_func = [](vector x) -> double {
        double val = 1.0; for(int i=0;i<3;i++) val*= std::cos(x[i]);
        return 1.0 / (1.0-val) / std::pow(M_PI, 3);
    }; 

    vector a4 = {1e-6, 1e-6, 1e-6}; vector b4 = {M_PI, M_PI, M_PI};
    double exact4 = 1.3932039296856768591842462603255;

    auto [valp, ep] = plainmc(cos_func, a4, b4, 1e6);
    auto [valc, ec] = cppmc(cos_func, a4, b4, 1e6);
    auto [valq, eq] = quacimc(cos_func, a4, b4, 1e6);

    std::cout << "\n\n - Cos Integral \n";
    std::cout << "plainmc: " << valp << " ± " << ep << std::endl;    
    std::cout << "cppmc:   " << valc << " ± " << ec << std::endl;
    std::cout << "quacimc: " << valq << " ± " << eq << std::endl;
    std::cout << "Exact:   " << exact4 << std::endl;


    return 0;
}
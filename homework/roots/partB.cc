#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>

#include "matrix.h"
#include "newton.h"
#include "ODE.h"

using namespace pp;

auto FE(double E) {
    return [E](double r, vector y) -> vector {
        vector dydr(2);
        dydr[0] = y[1];
        dydr[1] = -2.0 * ((1.0/r)*y[0]+E*y[0]);
        return dydr;
    };
}

int main(int argc, char *argv[]) {
    double r_min = 0.01;
    double r_max = 8.0; 
    double acc = 0.01;
    double eps = 0.01; 
    bool plt = false; 

    // Read command line 
    for(int i=1; i<argc; i++){
        std::string arg = std::string(argv[i]);
        if(arg == "-r_min") r_min = std::stod(argv[i+1]);
        if(arg == "-r_max") r_max = std::stod(argv[i+1]);
        if(arg == "-acc")   acc = std::stod(argv[i+1]);
        if(arg == "-eps")   eps = std::stod(argv[i+1]);
        if(arg == "-plt")   plt = true;
    }

    vector FE_init(2);
    FE_init[0] = r_min - r_min*r_min; //u(r)
    FE_init[1] = 1.0 - 2.0*r_min;     //u(r)'
    auto M = [r_min, r_max, FE_init, acc, eps](vector E) -> double {
        auto [x, y] = driver(FE(E[0]), r_min, r_max, FE_init, 0.1, acc, eps);
        return y.back()[0];
    };

    vector root = newton(Func_list{M}, {-1.0});
    double E0 = root[0];
    std::cout << E0 << "\n";

    if(plt==true){
        auto [r, f] = driver(FE(E0), r_min, r_max, FE_init, 0.1, acc, eps);
        
        std::ofstream file("partB.data");

        if (!file.is_open()) {
            std::cerr << "Error: Could not open file \n";
            return 1;
        }

        file << std::setprecision(6) << std::fixed;

        // Header
        file << "#Ground state\n";
        file << E0 << "\n";
        file << "#r f f' \n";

        // Write the data
        for (size_t i = 0; i < r.size(); ++i) {
            file << r[i]; 

            // Loop through the state vector components
            for (size_t j = 0; j < f[i].size(); ++j) {
                file << " " << f[i][j];
            }
            file << "\n";
        }

        file.close();
    }

    return 0;
}
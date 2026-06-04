#include <iostream>
#include <fstream>
#include "matrix.h"
#include "minimization.h"

using namespace pp;

int main () {
    std::cout << " --- Part A --- \n";

    auto Rosenbrock = [](vector x) -> double {
        double v1 = 1 - x[0];
        double v2 = x[1] - x[0]*x[0];
        return v1*v1 + 100.0 * v2*v2;
    };

    auto Himmel = [](vector x) -> double {
        double v1 = x[0]*x[0] + x[1] - 11;
        double v2 = x[0] + x[1]*x[1] - 7;
        return v1*v1 + v2*v2;
    };
    
    vector res1 = newton(Rosenbrock, (vector){-1.0, 1.0}, 1e-3, true);
    std::cout << "x,y: " << res1 << "    f(x,y) = "<< Rosenbrock(res1) << "\n\n";

    vector res2 = newton(Himmel, (vector){2.0, 2.0}, 1e-3, true);
    std::cout << "x,y: " << res2 << "    f(x,y) = "<< Himmel(res2) << "\n\n";


    std::cout << "\n --- Part B --- \n";

    std::ifstream ifile("higgs.in");
    vector energy, signal, error;
    double x,y,z;
    while (ifile >> x >> y >> z) {
        energy.push_back(x); signal.push_back(y); error.push_back(z);
    }
    ifile.close();

    auto BW = [](double E, double m, double A, double Γ) -> double {
        double em = E-m;
        return A / (em*em +Γ*Γ/4);
    };

    // x[0] = m, x[1] = A, x[2] = Γ
    auto DBW = [energy, signal, error, BW](vector x) -> double {
        int n = energy.size();
        double sum = 0;
        double val = 0; 

        for(int i=0; i<n; i++) {
            val = (BW(energy[i], x[0], x[1], x[2])-signal[i]) / error[i];
            sum += val*val;
        }

        return sum; 
    };

    vector res3 = newton(DBW, (vector){126.0, 10.0, 2.0}, 1e-3, true);
    std::cout << "m,A,Γ: " << res3 << "\n\n";

    std::ofstream ofile("fit_higs.data");
    ofile << "# m A Γ \n";
    ofile << res3[0] << " " << res3[1] << " " << res3[2] << " \n";
    ofile << "# Energy Breit-Wigner\n";
    
    for(double e = 100.0; e <= 160.0; e += 0.1) {
        ofile << e << " " << BW(e, res3[0], res3[1], res3[2]) << "\n";
    }
    ofile.close();
    
    return 0;
}
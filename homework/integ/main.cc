#include <iostream>
#include <functional>
#include <cmath>
#include <fstream>
#include <iomanip>

#include "integrate.h"

double err_f(double x, double acc=1e-4, double eps=1e-4) {
    if (x<0.0) return -err_f(-x, acc, eps);
    if (x<=1.0) {
        auto f_e = [](double z) -> double {return std::exp(-z*z);};
        double integ = integrate(f_e, 0.0, x, acc, eps).val;
        return 2.0/std::sqrt(M_PI) * integ;
    }
    else {
        auto f_e2 = [x](double z) -> double {
            double term = x + (1.0 - z) / z;
            return std::exp(-term*term) / z / z;
        };
        double integ = integrate(f_e2, 0.0, 1.0, acc, eps).val;
        return 1.0 - 2.0/std::sqrt(M_PI) * integ;
    }

}

// Recursion counter helper function
template <typename Func>
auto count_calls(Func function, int& counter) {
    return [&counter, function](double x) -> double {
        counter++;
        return function(x);
    };
}

int main() {
    // --- Part A --- 
    auto f1 = [](double x) ->  double {return std::sqrt(x);};
    auto y1 = integrate(f1, 0.0, 1.0);
    
    
    auto f2 = [](double x) ->  double {return 1/std::sqrt(x);};
    int f2_count1 = 0; 
    auto f2_c1 = count_calls(f2, f2_count1);
    auto y2 = integrate(f2_c1, 0.0, 1.0);
    
    auto f3 = [](double x) ->  double {return std::sqrt(1-x*x);};
    auto y3 = integrate(f3, 0.0, 1.0);
    
    auto f4 = [](double x) ->  double {return std::log(x)/std::sqrt(x);};
    int f4_count1 = 0; 
    auto f4_c1 = count_calls(f4, f4_count1);
    auto y4 = integrate(f4_c1, 0.0, 1.0);
    
    std::cout << " --- Part A --- \n";
    std::cout << "∫_0^1 dx √(x)       = " << y1.val << std::endl;
    std::cout << "∫_0^1 dx 1/√(x)     = " << y2.val << std::endl;
    std::cout << "∫_0^1 dx √(1-x²)    = " << y3.val << std::endl;
    std::cout << "∫_0^1 dx ln(x)/√(x) = " << y4.val << std::endl;


    std::ofstream err_file1("err1.data");
    for (double x=-3.0; x<=3.0; x+=0.01) {
        err_file1 << x << " " << err_f(x) << "\n";
    }

    std::ofstream err_file2("err2.data");
    for (double acc=0.1; acc>=1e-10; acc*=0.5) {
        double val = std::abs(0.84270079294971486934 - err_f(1.0, acc, 0.0));
        err_file2 << acc << " " << val << "\n";
    }

    // --- Part B ---
    int f2_count2 = 0; 
    auto f2_c2 = count_calls(f2, f2_count2);
    auto y5 = Clenshaw_Curtis(f2_c2, 0, 1);

    int f4_count2 = 0; 
    auto f4_c2 = count_calls(f4, f4_count2);
    auto y6 = Clenshaw_Curtis(f4_c2, 0, 1);

    std::cout << "\n --- Part B --- \n Clenshaw-Curtis: \n";
    std::cout << "[ 1/√(x) from 0 to 1 ]\n";
    std::cout << "Clenshaw-Curtis Calc = " << std::setprecision(16) << y5.val << " (" << f2_count2 << " steps)\n";
    std::cout << "Standard Integrator  = " << std::setprecision(16) << y2.val << " (" << f2_count1 << " steps)\n";
    std::cout << "SciPy Exact Match    = 1.9999999999999984\n\n";

    std::cout << "[ ln(x)/√(x) from 0 to 1 ]\n";
    std::cout << "Clenshaw-Curtis Calc = " << std::setprecision(16) << y6.val << " (" << f4_count2 << " steps)\n";
    std::cout << "Standard Integrator  = " << std::setprecision(16) << y4.val << " (" << f4_count1 << " steps)\n";
    std::cout << "SciPy Exact Match    = -4.000000000000085\n\n";
    
    // I Have added the inifite check to Clenshaw_curtis
    auto f5 = [](double x) -> double {return std::exp(-x*x);};
    int f5_counter = 0;
    auto f5_c = count_calls(f5, f5_counter);
    auto y7 = Clenshaw_Curtis(f5_c, -INFINITY, INFINITY);

    auto f6 = [](double x) -> double {return 1/(1+x*x);};
    int f6_counter = 0;
    auto f6_c = count_calls(f6, f6_counter);
    auto y8 = Clenshaw_Curtis(f6_c, 0, INFINITY);

    std::cout << "\n Infinite limits: \n";
    std::cout << "[ Gaussian: e^(-x²) from -∞ to +∞ ]\n";
    std::cout << "Calculated Value     = " << std::setprecision(16) << y7.val << " (" << f5_counter << " steps)\n";
    std::cout << "scipy.integrate.quad = 1.7724538509055159 \n";
    std::cout << "Exact Value: √(π)    = " << std::sqrt(M_PI) << "\n\n";

    std::cout << "[ Cauchy: 1/(1+x²) from 0 to +∞ ]\n";
    std::cout << "Calculated Value     = " << std::setprecision(16) << y8.val << " (" << f6_counter << " steps)\n";
    std::cout << "scipy.integrate.quad = 1.5707963267948966 \n";
    std::cout << "Exact Value: π/2     = " << 0.5 * M_PI << "\n\n";

    // Part C
    std::cout << "\n --- Part C --- \n";
  
    std::cout << "[ Std: √(x) from 0 to 1 ]\n";
    std::cout << "Estimated Error = " << y1.err << "\n";
    std::cout << "Actual Error    = " << std::abs(y1.val - (2.0/3.0)) << "\n\n";

    std::cout << "[ Std: 1/√(x) from 0 to 1 ]\n";
    std::cout << "Estimated Error = " << y2.err << "\n";
    std::cout << "Actual Error    = " << std::abs(y2.val - 2.0) << "\n\n";

    std::cout << "[ Std: √(1-x²) from 0 to 1 ]\n";
    std::cout << "Estimated Error = " << y3.err << "\n";
    std::cout << "Actual Error    = " << std::abs(y3.val - (M_PI/4.0)) << "\n\n";

    std::cout << "[ Std: ln(x)/√(x) from 0 to 1 ]\n";
    std::cout << "Estimated Error = " << y4.err << "\n";
    std::cout << "Actual Error    = " << std::abs(y4.val - (-4.0)) << "\n\n";

    std::cout << "[ CC: 1/√(x) from 0 to 1 ]\n";
    std::cout << "Estimated Error = " << y5.err << "\n";
    std::cout << "Actual Error    = " << std::abs(y5.val - 2.0) << "\n\n";

    std::cout << "[ CC: ln(x)/√(x) from 0 to 1 ]\n";
    std::cout << "Estimated Error = " << y6.err << "\n";
    std::cout << "Actual Error    = " << std::abs(y6.val - (-4.0)) << "\n\n";

    std::cout << "[ CC: e^(-x²) from -∞ to +∞ ]\n";
    std::cout << "Estimated Error = " << y7.err << "\n";
    std::cout << "Actual Error    = " << std::abs(y7.val - std::sqrt(M_PI)) << "\n\n";

    std::cout << "[ CC: 1/(1+x²) from 0 to +∞ ]\n";
    std::cout << "Estimated Error = " << y8.err << "\n";
    std::cout << "Actual Error    = " << std::abs(y8.val - (M_PI/2.0)) << "\n\n";

    return 0;
}
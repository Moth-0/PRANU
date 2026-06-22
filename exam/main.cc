#include <iostream>
#include <fstream>
#include <iomanip> 
#include <cmath>

#include "adaptive2.h"
#include "adaptive3.h"
#include "ClenshawCurtis.h"
#include "adaptive_inf.h"

using namespace pp;

// The error function (erf)
double err_f(double x, double acc=1e-4, double eps=1e-4) {
    if (x<0.0) return -err_f(-x, acc, eps);
    if (x<=1.0) {
        auto f_e = [](double z) -> double {return std::exp(-z*z);};
        auto [val, err] = adaptive3(f_e, 0.0, x, acc, eps);
        return 2.0/std::sqrt(M_PI) * val;
    }
    else {
        auto f_e2 = [x](double z) -> double {
            double term = x + (1.0 - z) / z;
            return std::exp(-term*term) / z / z;
        };
        auto [val, err] = adaptive3(f_e2, 0.0, 1.0, acc, eps);
        return 1.0 - 2.0/std::sqrt(M_PI) * val;
    }
}

int main() {
    // =======================================================================
    // These wrappers allow the use of templated CC and adaptive_inf
    // =======================================================================
    auto pure_adp2 = [](const std::function<double(double)>& f, double a, double b, double acc=1e-4, double eps=1e-4) {
        return adaptive2(f, a, b, acc, eps); 
    };
    
    auto pure_adp3 = [](const std::function<double(double)>& f, double a, double b, double acc=1e-4, double eps=1e-4) {
        return adaptive3(f, a, b, acc, eps); 
    };

    auto cc_adp2 = [&](const std::function<double(double)>& f, double a, double b, double acc=1e-4, double eps=1e-4) {
        return ClenshawCurtis(f, pure_adp2, a, b, acc, eps);
    };
    
    auto cc_adp3 = [&](const std::function<double(double)>& f, double a, double b, double acc=1e-4, double eps=1e-4) {
        return ClenshawCurtis(f, pure_adp3, a, b, acc, eps);
    };


    // =======================================================================
    // 2. FINITE LIMITS TESTING
    // =======================================================================
    int eval_count = 0;

    auto f1 = [](double x) -> double { return std::sqrt(x); };
    auto f2 = [](double x) -> double { return 1.0 / std::sqrt(x); };

    auto f1_wrapper = [&](double x) -> double { eval_count++; return f1(x); };
    auto f2_wrapper = [&](double x) -> double { eval_count++; return f2(x); };

    // Standard Tests
    eval_count = 0; auto [adp2_val_1, adp2_err_1] = adaptive2(f1_wrapper, 0.0, 1.0); int adp2_count_1 = eval_count; 
    eval_count = 0; auto [adp3_val_1, adp3_err_1] = adaptive3(f1_wrapper, 0.0, 1.0); int adp3_count_1 = eval_count;

    eval_count = 0; auto [adp2_val_2, adp2_err_2] = adaptive2(f2_wrapper, 0.0, 1.0); int adp2_count_2 = eval_count;
    eval_count = 0; auto [adp3_val_2, adp3_err_2] = adaptive3(f2_wrapper, 0.0, 1.0); int adp3_count_2 = eval_count;

    std::cout << "The comparison of the adaptive 2 split method from homework \n"
              << "and the new adaptive 3 split integrator. \n"
              << "We can see that the 3 split fails and the 2 split uses a lot of calls \n"
              << "for the function 1/sqrt(x) since it can not evaluate 1/sqrt(0). \n";

    // --- Print the Comparison Table ---
    std::cout << std::string(90, '-') << "\n";
    std::cout << std::left 
              << std::setw(20) << "Function" 
              << std::setw(15) << "Method" 
              << std::setw(15) << "Result" 
              << std::setw(15) << "Error"
              << std::setw(15) << "Est. Error" 
              << std::setw(10) << "Evals" << "\n";
    std::cout << std::string(90, '-') << "\n";

    std::cout << std::setw(20) << "sqrt(x)" 
              << std::setw(15) << "2-Split" 
              << std::setw(15) << adp2_val_1 
              << std::setw(15) << 2.0/3.0 - adp2_val_1 
              << std::setw(15) << adp2_err_1 
              << std::setw(10) << adp2_count_1 << "\n";
              
    std::cout << std::setw(20) << "" 
              << std::setw(15) << "3-Split" 
              << std::setw(15) << adp3_val_1 
              << std::setw(15) << 2.0/3.0 - adp3_val_1 
              << std::setw(15) << adp3_err_1 
              << std::setw(10) << adp3_count_1 << "\n\n";

    std::cout << std::setw(20) << "1/sqrt(x)" 
              << std::setw(15) << "2-Split" 
              << std::setw(15) << adp2_val_2 
              << std::setw(15) << 2.0 - adp2_val_2 
              << std::setw(15) << adp2_err_2 
              << std::setw(10) << adp2_count_2 << "\n";
              
    std::cout << std::setw(20) << "" 
              << std::setw(15) << "3-Split" 
              << std::setw(15) << adp3_val_2 
              << std::setw(15) << 2.0 - adp3_val_2
              << std::setw(15) << adp3_err_2 
              << std::setw(10) << adp3_count_2 << "\n";
    std::cout << std::string(90, '-') << "\n\n";

    // Clenshaw Curtis Tests (Using the toolbox wrappers!)
    std::cout << "To fix this we use Clenshaw Curtis to rewtrite the function, \n"
              << "this allows the function value to be finite: \n"; 
    
    eval_count = 0; auto [adp2_val_3, adp2_err_3] = ClenshawCurtis(f1_wrapper, pure_adp2, 0.0, 1.0); int adp2_count_3 = eval_count; 
    eval_count = 0; auto [adp3_val_3, adp3_err_3] = ClenshawCurtis(f1_wrapper, pure_adp3, 0.0, 1.0); int adp3_count_3 = eval_count;
    eval_count = 0; auto [adp2_val_4, adp2_err_4] = ClenshawCurtis(f2_wrapper, pure_adp2, 0.0, 1.0); int adp2_count_4 = eval_count;
    eval_count = 0; auto [adp3_val_4, adp3_err_4] = ClenshawCurtis(f2_wrapper, pure_adp3, 0.0, 1.0); int adp3_count_4 = eval_count;

    std::cout << std::string(90, '-') << "\n";
    std::cout << std::left 
              << std::setw(20) << "Function" 
              << std::setw(15) << "Method" 
              << std::setw(15) << "Result" 
              << std::setw(15) << "Error"
              << std::setw(15) << "Est. Error" 
              << std::setw(10) << "Evals" << "\n";
    std::cout << std::string(90, '-') << "\n";

    std::cout << std::setw(20) << "sqrt(x)" 
              << std::setw(15) << "CC-2-Split" 
              << std::setw(15) << adp2_val_3 
              << std::setw(15) << 2.0/3.0 - adp2_val_3 
              << std::setw(15) << adp2_err_3 
              << std::setw(10) << adp2_count_3 << "\n";
              
    std::cout << std::setw(20) << "" 
              << std::setw(15) << "CC-3-Split" 
              << std::setw(15) << adp3_val_3 
              << std::setw(15) << 2.0/3.0 - adp3_val_3 
              << std::setw(15) << adp3_err_3 
              << std::setw(10) << adp3_count_3 << "\n\n";

    std::cout << std::setw(20) << "1/sqrt(x)" 
              << std::setw(15) << "CC-2-Split" 
              << std::setw(15) << adp2_val_4 
              << std::setw(15) << 2.0 - adp2_val_4 
              << std::setw(15) << adp2_err_4 
              << std::setw(10) << adp2_count_4 << "\n";
              
    std::cout << std::setw(20) << "" 
              << std::setw(15) << "CC-3-Split" 
              << std::setw(15) << adp3_val_4 
              << std::setw(15) << 2.0 - adp3_val_4
              << std::setw(15) << adp3_err_4 
              << std::setw(10) << adp3_count_4 << "\n";
    std::cout << std::string(90, '-') << "\n\n";


    // =======================================================================
    // 3. INFINITE LIMITS TESTING
    // =======================================================================
    std::cout << "Now we look at some functions with infinite limits, \n" 
              << "and use all four integrators above: \n";

    // Test 3: x*exp(-x^2) from 0 to INF
    auto f3 = [](double x) -> double { return x * std::exp(-x*x); };
    auto f3_wrapper = [&](double x) -> double { eval_count++; return f3(x); };

    eval_count = 0; auto [inf_val_1, inf_err_1] = adaptive_inf(f3_wrapper, pure_adp2, 0.0, INFINITY); int inf_count_1 = eval_count;
    eval_count = 0; auto [inf_val_2, inf_err_2] = adaptive_inf(f3_wrapper, pure_adp3, 0.0, INFINITY); int inf_count_2 = eval_count;
    eval_count = 0; auto [inf_val_3, inf_err_3] = adaptive_inf(f3_wrapper, cc_adp2, 0.0, INFINITY);   int inf_count_3 = eval_count;
    eval_count = 0; auto [inf_val_4, inf_err_4] = adaptive_inf(f3_wrapper, cc_adp3, 0.0, INFINITY);   int inf_count_4 = eval_count;

    // Test 4: exp(-x^2) from -INF to INF
    auto f4 = [](double x) -> double { return std::exp(-x*x); };
    auto f4_wrapper = [&](double x) -> double { eval_count++; return f4(x); };

    eval_count = 0; auto [inf_val_5, inf_err_5] = adaptive_inf(f4_wrapper, pure_adp2, -INFINITY, INFINITY); int inf_count_5 = eval_count;
    eval_count = 0; auto [inf_val_6, inf_err_6] = adaptive_inf(f4_wrapper, pure_adp3, -INFINITY, INFINITY); int inf_count_6 = eval_count;
    eval_count = 0; auto [inf_val_7, inf_err_7] = adaptive_inf(f4_wrapper, cc_adp2, -INFINITY, INFINITY);   int inf_count_7 = eval_count;
    eval_count = 0; auto [inf_val_8, inf_err_8] = adaptive_inf(f4_wrapper, cc_adp3, -INFINITY, INFINITY);   int inf_count_8 = eval_count;

    // --- Print the Infinite Limits Table ---
    std::cout << std::string(90, '-') << "\n";
    std::cout << std::left 
              << std::setw(20) << "Function (Limits)" 
              << std::setw(15) << "Method" 
              << std::setw(15) << "Result" 
              << std::setw(15) << "Error"
              << std::setw(15) << "Est. Error" 
              << std::setw(10) << "Evals" << "\n";
    std::cout << std::string(90, '-') << "\n";

    std::cout << std::setw(20) << "x*exp(-x^2) [0,inf]" 
              << std::setw(15) << "Inf-2-Split" 
              << std::setw(15) << inf_val_1 
              << std::setw(15) << 0.5 - inf_val_1 
              << std::setw(15) << inf_err_1 
              << std::setw(10) << inf_count_1 << "\n";
              
    std::cout << std::setw(20) << "" 
              << std::setw(15) << "Inf-3-Split" 
              << std::setw(15) << inf_val_2 
              << std::setw(15) << 0.5 - inf_val_2 
              << std::setw(15) << inf_err_2 
              << std::setw(10) << inf_count_2 << "\n";

    std::cout << std::setw(20) << "" 
              << std::setw(15) << "CC-Inf-2-Split" 
              << std::setw(15) << inf_val_3 
              << std::setw(15) << 0.5 - inf_val_3 
              << std::setw(15) << inf_err_3 
              << std::setw(10) << inf_count_3 << "\n";

    std::cout << std::setw(20) << "" 
              << std::setw(15) << "CC-Inf-3-Split" 
              << std::setw(15) << inf_val_4 
              << std::setw(15) << 0.5 - inf_val_4 
              << std::setw(15) << inf_err_4 
              << std::setw(10) << inf_count_4 << "\n\n";

    double exact_f4 = std::sqrt(M_PI);
    std::cout << std::setw(20) << "exp(-x^2) [-inf,inf]" 
              << std::setw(15) << "Inf-2-Split" 
              << std::setw(15) << inf_val_5 
              << std::setw(15) << exact_f4 - inf_val_5 
              << std::setw(15) << inf_err_5 
              << std::setw(10) << inf_count_5 << "\n";
              
    std::cout << std::setw(20) << "" 
              << std::setw(15) << "Inf-3-Split" 
              << std::setw(15) << inf_val_6 
              << std::setw(15) << exact_f4 - inf_val_6 
              << std::setw(15) << inf_err_6 
              << std::setw(10) << inf_count_6 << "\n";

    std::cout << std::setw(20) << "" 
              << std::setw(15) << "CC-Inf-2-Split" 
              << std::setw(15) << inf_val_7 
              << std::setw(15) << exact_f4 - inf_val_7 
              << std::setw(15) << inf_err_7 
              << std::setw(10) << inf_count_7 << "\n";

    std::cout << std::setw(20) << "" 
              << std::setw(15) << "CC-Inf-3-Split" 
              << std::setw(15) << inf_val_8 
              << std::setw(15) << exact_f4 - inf_val_8 
              << std::setw(15) << inf_err_8 
              << std::setw(10) << inf_count_8 << "\n";
    std::cout << std::string(90, '-') << "\n\n";

    
    // =======================================================================
    // 4. CONVERGENCE PLOT DATA GENERATOR: erf(1)
    // =======================================================================
    std::cout << "Generating convergence data for erf(1)...\n";
    
    std::ofstream out_file("erf_convergence.txt");
    out_file << "acc actual_error_adp2 evals_adp2 actual_error_adp3 evals_adp3\n";

    double exact_integral = std::erf(1.0) * std::sqrt(M_PI) / 2.0; 

    for (int i = 1; i <= 15; ++i) {
        double acc = std::pow(10, -i);
        double eps = 0.0; 
        
        // Test adaptive2
        int evals2 = 0;
        auto f_erf2 = [&](double x) { evals2++; return std::exp(-x * x); };
        auto [val2, err2_est] = pure_adp2(f_erf2, 0.0, 1.0, acc, eps);
        double actual_error2 = std::abs(exact_integral - val2);

        // Test adaptive3
        int evals3 = 0;
        auto f_erf3 = [&](double x) { evals3++; return std::exp(-x * x); };
        auto [val3, err3_est] = pure_adp3(f_erf3, 0.0, 1.0, acc, eps);
        double actual_error3 = std::abs(exact_integral - val3);

        // Save all data to the file
        out_file << std::scientific << std::setprecision(15) 
                 << acc << " " 
                 << actual_error2 << " " << evals2 << " "
                 << actual_error3 << " " << evals3 << "\n";
    }
    
    out_file.close();
    std::cout << "Data saved to 'erf_convergence.txt'!\n";

    return 0;
}
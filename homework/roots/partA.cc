#include "newton.h"
#include "matrix.h"

using namespace pp;

int main(){

    // --- Part A ---
    auto R_dfdx = [](vector x) -> double {
        return -2.0*(1.0-x[0]) + 200.0*(x[1]-x[0]*x[0]) * (-2.0*x[0]);
    };
    auto R_dfdy = [](vector x) -> double {
        return 200.0*(x[1]-x[0]*x[0]);
    };

    Func_list fl_1 = {R_dfdx, R_dfdy};
    vector point = {0.5, 0.5};
    vector root1 = newton(fl_1, point);

    std::cout << "Roots of Rosenbrock gradient: " << root1 << "\n"
              << "Exact: (1,1)" << "\n" ;


    auto H_dfdx = [](vector x) -> double {
        return 4.0*x[0]*(x[0]*x[0]+x[1]-11.0)
               + 2.0*(x[0]+x[1]*x[1]-7.0);
    };
    auto H_dfdy = [](vector x) -> double {
        return 2.0*(x[0]*x[0]+x[1]-11.0) 
               + 4.0*x[1]*(x[0]+x[1]*x[1]-7.0);
    };

    Func_list fl_2 = {H_dfdx, H_dfdy};
    matrix points = {{-2.0, -2.0}, {-3.0, 3.0}, {-3.0, -3.0}, {3.0, -2.0}};

    std::cout << "Roots of Himmelblau gradient: " << "\n";
    for(size_t i=0; i<4; i++){
        std::cout << "Starting point: " << points[i] 
                  << " Result: " << newton(fl_2, points[i]) << "\n";
    }
    std::cout << "Minima: (3.0, 2.0), (-2.805118, 3.131312), (-3.779310, -3.283186), (3.584428, -1.848126) \n"
              << "Maxima: (-0.270845, -0.923039) \n\n";

    return 0;
}
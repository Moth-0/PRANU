#include <cmath>
#include <iostream>
#include <fstream>

#include "spline.h"
#include "quadspline.h"

int main() {
    // --- Part A ---
    // Datapoints
    vector xA;
    vector yA;
    std::ofstream points_A("points_A.data");
    for (double n=0.0; n<19; n++){
        double x_p = n*0.5;
        double y_p = std::cos(x_p);
        xA.push_back(x_p);
        yA.push_back(y_p);
        points_A << x_p << " " << y_p << "\n";
    }
    points_A.close();

    // Make interpolation file 
    std::ofstream inter_A("inter_A.data");
    for (double z=0.0; z<9.0; z += 0.2){

        double y_val = linterp(xA, yA, z);
        double int_val = linterpInter(xA, yA, z);

        inter_A << z << " " << y_val << " " << int_val << "\n"; 
    }
    inter_A.close();

    // --- Part B --- 
    // Datapoints
    vector xB;
    vector yB;
    std::ofstream points_B("points_B.data");
    for (double i=0; i<7; i++){
        double x_p = i;
        double y_p = (x_p * x_p);
        xB.push_back(x_p);
        yB.push_back(y_p);
        points_B << x_p << " 1 " << x_p << " " << y_p << "\n";
    }
    points_B.close();

    vector ones(xB.size()); 
    for(size_t i=0; i<xB.size(); i++) ones[i] = 1; 

    qspline spline1(xB, ones);
    qspline spline2(xB, xB);
    qspline spline3(xB, yB);

    // Make interpolation file 
    std::ofstream inter_B("inter_B.data");
    for (double z=0.0; z<6.0; z += 0.2) {
        inter_B << z << " " 
              << spline1.eval(z) << " " << spline1.deriv(z) << " " << spline1.integ(z) << " " 
              << spline2.eval(z) << " " << spline2.deriv(z) << " " << spline2.integ(z) << " " 
              << spline3.eval(z) << " " << spline3.deriv(z) << " " << spline3.integ(z) << "\n";
    }
    inter_B.close();


    return 0;
}
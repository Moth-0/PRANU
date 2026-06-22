#include <cmath>
#include <iostream>
#include <fstream>

#include "linspline.h"
#include "quadspline.h"
#include "cubespline.h"

int main() {
    std::cout << "partA.png shows the linear interpolation of a of a cosine, with the anti-derivative. \n"
              << "partB.png shows the quadradic interpolation for three different functions "
              << "with their derivative and anti-derivative. \n"
              << "partC.png shows cubic interpolation with the new double derivative. \n";
              
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

    // -- Part C - Cubic Spline -- 
    // Datapoints
    vector xC;
    vector yC;
    vector zC;
    std::ofstream points_C("points_C.data");
    for (double i=0; i<13; i++){
        double x_p = i*0.5;
        double y_p = (x_p * x_p);
        double z_p = y_p * x_p;
        xC.push_back(x_p);
        yC.push_back(y_p);
        zC.push_back(z_p);
        points_C << x_p << " " << y_p << " " << z_p << "\n";
    }
    points_C.close();

    cspline cubes1(xC, xC);
    cspline cubes2(xC, yC);
    cspline cubes3(xC, zC);

    // Make interpolation file 
    std::ofstream inter_C("inter_C.data");
    for (double z=0.0; z<6.0; z += 0.2) {
        inter_C << z << " " 
              << cubes1.eval(z) << " " << cubes1.deriv(z) << " " << cubes1.deriv2(z) << " " << cubes1.integ(z) << " " 
              << cubes2.eval(z) << " " << cubes2.deriv(z) << " " << cubes2.deriv2(z) << " " << cubes2.integ(z) << " " 
              << cubes3.eval(z) << " " << cubes3.deriv(z) << " " << cubes3.deriv2(z) << " " << cubes3.integ(z) << "\n";
    }
    inter_C.close();

    return 0;
}
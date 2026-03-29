#include <cmath>
#include <iostream>
#include <fstream>

#include "spline.h"

int main() {
    // Datapoints
    vector x;
    vector y;
    std::ofstream points("points.data");
    for (double n=0.0; n<19; n++){
        double x_p = n*0.5;
        double y_p = std::cos(x_p);
        x.push_back(x_p);
        y.push_back(y_p);
        points << x_p << " " << y_p << "\n";
    }
    points.close();

    // Make interpolation file 
    std::ofstream inter("inter.data");
    for (double z=0.0; z<9.0; z += 0.2){

        double y_val = linterp(x, y, z);
        double int_val = linterpInter(x, y, z);

        inter << z << " " << y_val << " " << int_val << "\n"; 
    }
    inter.close();

    return 0;
}
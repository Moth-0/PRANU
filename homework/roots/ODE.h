#pragma once

#include <functional>
#include <vector>
#include <cmath>

#include "matrix.h"

using namespace pp;

std::tuple<vector, vector> rkstep12(
            const std::function<vector(double, const vector&)>& f,
            double x, 
            const vector& y, 
            double h
){
    size_t n = y.size();

    // Initialize
    vector k1(n);
    vector k2(n);
    vector y_mid(n);

    vector y_next(n);
    vector error(n);

    // Calculate k1
    k1 = f(x,y);

    // Calculate midpoint
    double x_mid = x + h * 0.5;
    for(size_t i=0; i<n; i++) y_mid[i] = y[i] + 0.5 * h * k1[i];

    // Calculate k2
    k2 = f(x_mid, y_mid);

    // Calculate next step and error 
    for(size_t i=0; i<n; i++) {
        y_next[i] = y[i] + h * k2[i];
        error[i] = h * (k2[i] - k1[i]);
    }

    return std::tuple{y_next, error};
}

std::tuple<vector, std::vector<vector>> driver (
            const std::function<vector(double, const vector&)> f, 
            double a, double b, 
            vector yinit, 
            double h=0.1,
            double acc=0.01, 
            double eps=0.01
){
    double x=a; vector y=yinit;
    vector xlist; std::vector<vector> ylist;
    xlist.push_back(x);
    ylist.push_back(y);

    while (true) {
        if(x>=b) return std::tuple{xlist, ylist}; // Done
        if(x+h>b) h = b-x; 

        auto [yh, dy] = rkstep12(f, x, y, h);

        double tol = (acc+eps*yh.norm()) * std::sqrt(h/(b-a));
        double err = dy.norm();

        if (err<=tol){ // Accept
            x+=h; y=yh;
            xlist.push_back(x);
            ylist.push_back(y);
        }
        if (err>0) { // Adjust stepsize
            h *= std::min(std::pow(tol/err, 0.25)*0.95, 2.0);
        }
        else h *= 2.0;

        // Limit for how big h can be 
        // Added since Part B wont draw circle since h keeps doubeling
        if(h > 0.4) h = 0.4;
    }
}
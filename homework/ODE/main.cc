#include <iostream>
#include <fstream>
#include <string>

#include "ODE.h"

using namespace pp;

// Helper function to save results to a CSV file
void save_data(const vector& xlist, 
               const std::vector<vector>& ylist, 
               const std::string& filename) 
{
    std::ofstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << " for writing.\n";
        return;
    }

    file << std::setprecision(6) << std::fixed;

    // Header
    file << "t y0 y1\n";

    // Write the data
    for (size_t i = 0; i < xlist.size(); ++i) {
        file << xlist[i]; 

        // Loop through the state vector components
        for (size_t j = 0; j < ylist[i].size(); ++j) {
            file << " " << ylist[i][j];
        }
        file << "\n";
    }

    file.close();
    std::cout << "Successfully saved data to " << filename << "\n";
}

// Helper function for part B
auto f_grav(double epsilon) {
    return [epsilon](double t, const vector& y) -> vector {
        vector dydt(2);
        dydt[0] = y[1];
        dydt[1] = 1 - y[0] + epsilon * y[0] * y[0];
        return dydt;
    };
}

int main () {

    // --- Part A --- 
    // - Simple Harmonic test u''=-u
    auto f_harmonic = [](double t, const vector& y) -> vector {
        vector dydt(2);
        dydt[0] = y[1];
        dydt[1] = -y[0];
        return dydt;
    };

    vector y_init_harm(2);
    y_init_harm[0] = 1.0; // initial position
    y_init_harm[1] = 0.0; // initial velocity

    auto [x_out1, y_out1] = driver(f_harmonic, 0.0, 10.0, y_init_harm);
    std::cout << "Harmonic in: " << y_out1.size() << " steps\n";
    save_data(x_out1, y_out1, "data/harmonic.data");

    // - Damped Pendulum (odeint.manual)
    auto f_damped = [](double t, const vector& y) -> vector {
        double b=0.25; double c=5.0;
        vector dydt(2);
        dydt[0] = y[1];
        dydt[1] = -b * y[1] - c * std::sin(y[0]);
        return dydt;
    };

    vector y_init_damp(2); 
    y_init_damp[0] = 3.0;
    y_init_damp[1] = 0.0;

    auto [x_out2, y_out2] = driver(f_damped, 0.0, 10.0, y_init_damp);
    std::cout << "Damped in: " << y_out2.size() << " steps\n";
    save_data(x_out2, y_out2, "data/damped.data");

    // - Lotka-Volterra
    auto f_lv = [](double t, const vector& y) -> vector {
        double a=1.5; double b=1.0; double c=3.0; double d=1.0;
        vector dydt(2);
        dydt[0] = a * y[0] - b * y[0] * y[1]; 
        dydt[1] = -c * y[1] + d * y[0] * y[1];
        return dydt;
    };

    vector y_init_lv(2); 
    y_init_lv[0] = 10.0;
    y_init_lv[1] = 5.0;

    auto [x_out3, y_out3] = driver(f_lv, 0.0, 15.0, y_init_lv);
    std::cout << "Lotka-Volterra in: " << y_out3.size() << " steps\n";
    save_data(x_out3, y_out3, "data/lv.data");


    // --- Part B ---
    double start = 0.0; double end = 10.0*2.0*M_PI; // 10 rotations

    // Newtonian Circular
    auto f_circ = f_grav(0.0);
    vector y_init_circ(2);
    y_init_circ[0] = 1.0; 
    y_init_circ[1] = 0.0;

    auto [x_circ, y_circ] = driver(f_circ, start, end, y_init_circ);
    save_data(x_circ, y_circ, "data/circ.data");

    // Newtonian Ellipsical
    auto f_elip = f_grav(0.0);
    vector y_init_elip(2);
    y_init_elip[0] = 1.0; 
    y_init_elip[1] = -0.5;

    auto [x_elip, y_elip] = driver(f_elip, start, end, y_init_elip);
    save_data(x_elip, y_elip, "data/elip.data");

    // Relativistic
    auto f_rel = f_grav(0.01);
    vector y_init_rel(2);
    y_init_rel[0] = 1.0; 
    y_init_rel[1] = -0.5;

    auto [x_rel, y_rel] = driver(f_rel, start, end, y_init_rel);
    save_data(x_rel, y_rel, "data/rel.data");

    return 0;
}
#include <iostream>
#include <fstream>
#include <string>

#include "network.h"

using namespace pp;

int main() {
    std::cout << "--- Part A & B---\n";

    network ann(4);

    vector x_train;
    vector y_train;

    // Get training data
    std::ifstream infile("func.in");

    // Skip header
    std::string dummy_header;
    std::getline(infile, dummy_header);

    double x_in, y_in;
    while (infile >> x_in >> y_in) {
        x_train.push_back(x_in);
        y_train.push_back(y_in);
    }

    infile.close();

    ann.train(x_train, y_train);

    // Output model data 
    std::ofstream outfile("model.data");

    outfile << "# x y y' y'' Y\n";
    for(size_t i=0; i<x_train.size(); i++) {
        double x = x_train[i];
        outfile << x << " " << ann.respond(x) 
                     << " " << ann.derivative(x)
                     << " " << ann.second_derivative(x)
                     << " " << ann.antiderivative(x) << "\n";
    }
    std::cout << "Fitted cos(5.0 * x - 1.0) * exp(-x * x) function shown in 'modelfit.png'. \n"
              << "Derivatives from part B also plotted. \n";

    return 0;
}
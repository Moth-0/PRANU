#include <functional>
#include <cmath>
#include <fstream>

#include "matrix.h"
#include "QR.h"

#define FUNC std::function<double(double)>

using namespace pp;

bool approx(double x, double y, double acc=1e-15){
	if(std::abs(x-y) < acc) return true;
	return false;
}

std::tuple<vector, matrix> lsfit(std::vector<FUNC> fs, vector x, vector y, vector dy) {
    std::cerr << "Starting fit \n";
    size_t n = x.size();
    size_t k = fs.size();

    matrix A(n, k);
    vector b(n);
    
    std::cerr << "Making A \n";
    for(size_t i = 0; i<n; i++) {
        b[i] = y[i]/dy[i]; 
        for(size_t j = 0; j<k; j++) {
            A(i, j) = fs[j](x[i])/dy[i];
        }
    }

    std::cerr << "Starting QR \n";
    QR qr(A);

    std::cerr << "Starting solve \n";
    vector res = qr.solve(b);

    // Covarience matrix
    matrix R_inv = qr.R.inverse();
    matrix cov = R_inv * R_inv.T();

    return {res, cov};
}

int main () {
    // Test QR for tall matrices
    matrix A = {{1, 2, 3}, {4, 5, 6}}; // Tall matrix
    QR qr(A);
    matrix out1 = qr.Q*qr.R;
    FOR_MAT(out1) assert(approx(out1(i,j), A(i, j)));

    matrix out2 = qr.Q.T()*qr.Q;
    matrix I(2, 2);
    I.setid();
    FOR_MAT(out2) assert(approx(out2(i,j), I(i,j)));

    // Investigate radioactive decay 
    vector time     = {1, 2, 3, 4, 6, 9, 10, 13, 15}; // (days)
    vector activity = {117, 100, 88, 72, 53, 29.5, 25.2, 15.2, 11.1}; // y of ThX (relative units)
    vector unc      = {6, 5, 4, 4, 4, 3, 3, 2, 2}; 

    vector y = activity;
    for(size_t i=0; i<y.size(); i++) y[i] = std::log(activity[i]);

    vector dy = unc;
    for(size_t i=0; i<dy.size(); i++) dy[i] = unc[i]/activity[i];

    std::vector<FUNC> functions = {
        [](double x) {return 1.0; },
        [](double x) {return x; }
    };

    auto [res, cov] = lsfit(functions, time, y, dy);

    vector err(cov.size1());
    for (size_t i=0; i<cov.size1(); i++) err[i] = std::sqrt(cov(i, i));
    double H = std::log(2)/(-res[1]);
    double err_H = H / (-res[1]) * err[1];

    std::cout << "Test radioactive decay, plot in plot.png: \n"
              << "The uncertainties of part B are also included. \n" 
              << "The dotted lines show the change of δc:\n\n";
    std::cout << "Resulting coef: " << res << " ± " << err << std::endl;
    std::cout << "Half-life = " << H << " ± " << err_H << " days" << std::endl;
    std::cout << "Experimental: 3.632 days";

    // Write to file 
    std::string file = "activity.data";
    std::ofstream dataout(file);
    std::ofstream paramsout("params.gp");

    if (dataout.is_open() && paramsout.is_open()){
        paramsout << "ln_a = " << res[0] << std::endl;
        paramsout << "l = " << -res[1] << std::endl;
        paramsout << "da = " << err[0] << std::endl;
        paramsout << "dl = " << err[1] << std::endl;
        for (size_t i = 0; i < time.size(); ++i) {
            dataout << time[i] << " " << y[i] << " " << dy[i] << std::endl;
        }
    }
    else {
        std::cerr << "Error opening files: " << file << std::endl;
	    return 1;
    }

    return 0;
} // Namespace pp
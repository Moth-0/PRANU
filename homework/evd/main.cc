#include <random>
#include <cassert>
#include <string>
#include <fstream>

#include "matrix.h"
#include "eigen.h"

using namespace pp;

double rand_d(double min, double max) {
    static std::mt19937_64 rng(std::random_device{}());
    std::uniform_real_distribution<double> dist(min, max);
    return dist(rng);
}

bool approx(double x, double y, double acc=1e-15){
	if(std::abs(x-y) < acc) return true;
	//if(std::abs(x-y) < eps*std::max(std::abs(x),std::abs(y))) return true;
	return false;
}

int main (int argc, char *argv[]) {
    std::cout << "--- Part A --- \n";
    matrix L(3,3);
    FOR_MAT(L) if (i >= j) {
        L(i, j) = rand_d(0, 10);
    }

    // A = L L^T Symmetric
    matrix A(3,3);
    FOR_MAT(A) {
        long double s = 0.0;
        for (size_t k = 0; k <= std::min(i, j); ++k) s += L(i, k) * L(j, k);
        A(i, j) = s;
    }

    std::cout << "Random A = \n" << A << "\n\n";

    EVD res(A); 

    matrix out1 = res.V * res.D * res.V.T();

    std::cout << "A = V D V^T = \n" << out1 << "\n\n";
    FOR_MAT(A) assert(approx(out1(i,j), A(i, j)));

    
    matrix out2 = res.V.T()*res.V;
    matrix I(out2.size1(), out2.size2()); 

    std::cout << "V^T V = \n" << out2 << "\n\n"; 
    for(size_t i=0; i<I.size1(); i++) I(i,i) = 1.0;

    FOR_MAT(out2) assert(approx(out2(i,j), I(i,j)));

    
    matrix out3 = res.V.T() * A * res.V;
    std::cout << "D = V^T A V = \n" << out3 << "\n\n";
    FOR_MAT(out3) assert(approx(out3(i,j), res.D(i,j)));


    std::cout << "\n--- Part B.1 --- \n";
    std::cout << " - Implimentation in plots.cc -\n" 
              << " - Numerical wavefunction in energy.png\n";
    
    
    std::cout << "\n--- Part B.2 --- \n";
    std::cout << " - convergence shown in dr.png and rmax.png\n";
    int rmax = 10;
    double dr = 0.1;
    std::string file = "energy.data";
    std::ofstream outstream(file);

    int npoints = (int)(rmax/dr)-1;
    pp::vector r(npoints);
    for(int i=0;i<npoints;i++) r[i] = dr*(i+1);

    pp::matrix H(npoints, npoints);
    for(int i=0;i<npoints-1;i++){
        H(i,i)  =-2*(-0.5/dr/dr);
        H(i,i+1)= 1*(-0.5/dr/dr);
        H(i+1,i)= 1*(-0.5/dr/dr);
    }
    H(npoints-1,npoints-1) = -2*(-0.5/dr/dr);
    for(int i=0;i<npoints;i++) H(i,i) += -1/r[i];

    EVD sys(H);

    double inv_sqrt_dr = 1.0 / std::sqrt(dr);

    if (outstream.is_open()){
        for (int i = 0; i < npoints; ++i) {
        double ri = dr * (i + 1);
        double f0 = sys.V(i, 0) * inv_sqrt_dr; 
        double f1 = sys.V(i, 1) * inv_sqrt_dr; 
        outstream << ri << " " << f0 << " " << f1 << std::endl;
    }

    }
    else {
        std::cerr << "Error opening files: " << file << std::endl;
	    return 1;
    }

    std::cout << "\n--- Part C - Checking runtime O(n3) --- \n";
    std::cout << " - Implimentation in times.cc - \n"
              << " - Result in times.png \n";
    

    return 0;
} // namespace pp

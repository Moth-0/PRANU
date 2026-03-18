#include <random>
#include <cassert>
#include <chrono>

#include "matrix.h"
#include "eigen.h"

using namespace pp;

double rand_d(double min, double max) {
    static std::mt19937_64 rng(std::random_device{}());
    std::uniform_real_distribution<double> dist(min, max);
    return dist(rng);
}


int main (int argc, char *argv[]) {
    
    size_t n = 1; 
    for(int i=1; i<argc; ++i){
        std::string arg = argv[i];
        if(arg == "-n" && i+1 < argc) n = static_cast<size_t>(std::stoull(argv[i+1]));
    }

    matrix L(n,n);
    FOR_MAT(L) if (i >= j) {
        L(i, j) = rand_d(0, 10);
    }

    // A = L L^T Symmetric
    matrix A(n,n);
    FOR_MAT(A) {
        long double s = 0.0;
        for (size_t k = 0; k <= std::min(i, j); ++k) s += L(i, k) * L(j, k);
        A(i, j) = s;
    }

    auto start = std::chrono::high_resolution_clock::now();

    EVD sys(A); 

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;

    std::cout << n << " " << elapsed.count() << std::endl;

    return 0;
}
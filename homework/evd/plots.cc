#include <random>
#include <cassert>

#include "matrix.h"
#include "eigen.h"

using namespace pp; 

int main (int argc, char *argv[]) {
    // Get command line inputs 
    int rmax = 1;
    double dr = 0.1;
    for(int i=1; i<argc; ++i){
        std::string arg = argv[i];
        if(arg == "-rmax" && i+1 < argc) rmax = std::stoi(argv[i+1]);
        if(arg == "-dr" && i+1 < argc) dr = std::stod(argv[i+1]);
    }
    

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
    
    std::cout << rmax << " " << dr << " "<< sys.D(0,0) << std::endl;

    return 0;
} // namespace pp
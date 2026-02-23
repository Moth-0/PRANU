#include<iostream>
#include<vector>
#include"matrix.h"
#include"QR.h"

int main () {
    // Check that decomp works 
    std::cout << "Decomp test \n" << std::endl; 
    pp::matrix A = {{4, 3}, {1, 2}};
    pp::QR result(A); 
    std::cout << "A = \n" << A << "\n\n";
    std::cout << "R = \n" << result.R << "\n\n";
    pp::matrix I = result.Q.T()*result.Q;
    std::cout << "QTQ = \n" << I << "\n\n";
    pp::matrix B = result.Q*result.R;
    std::cout << "QR = A = \n" << B << "\n\n";


return 0;}
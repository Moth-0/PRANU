#include<iostream>
#include<vector>
#include"matrix.h"

int main () {
    pp::vector a = {1.2, 3.2};
    std::cout << a << std::endl;
    pp::matrix M = {{1.0,0.0}, {0.0,2.0}};
    std::cout << M << std::endl;
    pp::vector U = M*a;
    std::cout << U << std::endl;
return 0;}
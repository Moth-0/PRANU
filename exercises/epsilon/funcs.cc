#include<cmath>
#include"funcs.h"

namespace funcs{
bool approx(double a, double b, double acc, double eps) {
    double diff = std::abs(a-b);
    double max_ab = std::max(std::abs(a), std::abs(b));
    if(diff <= acc) {return true;}
    else if(diff/max_ab <= eps) {return true;}
    else {return false;}
}
}
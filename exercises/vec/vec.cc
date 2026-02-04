#include"vec.h"
#include<cmath>
#include<iostream>

namespace pp{
std::ostream& operator<<(std::ostream& os, const vec& v){
    os << "{ " << v.x << ", " << v.y << ", " << v.z << " } ";
    return os;
    }
}

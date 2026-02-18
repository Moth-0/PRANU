#pragma once
#include<iostream>
#include<string>

namespace pp{
template <typename T>
struct vec{
    T x,y,z; 
    // Constructor
    vec(T a, T b, T c){x=a; y=b; z=c;}
    vec() : vec(0, 0, 0) {}

    vec(const vec&) = default;  // Copy vec a = b
    vec(vec&&) = default;       // Move vec a=b+c
    ~vec() = default;           // Destructor

    // assignment
    vec& operator=(const vec&) = default; // copy assignment a=b
    vec& operator=(vec&&) = default;      // move assignment a=b+c

    //operation
    //vec& operator+=(const vec&);
    vec& operator+=(const vec& other){
        x+=other.x;
        y+=other.y;
        z+=other.z;
        return (*this);
    }
    
    //vec& operator-=(const vec&);
    vec& operator-=(const vec& other){
        x-=other.x;
        y-=other.y;
        z-=other.z;
        return (*this);
    }

    //vec& operator*=(T);
    vec& operator*=(T n){
        x*=n;
        y*=n;
        z*=n;
        return (*this);
    }
    //vec& operator/=(T);
    vec& operator/=(T n){
        x/=n;
        y/=n;
        z/=n;
        return (*this);
    }

    // utility
    void set(T a, T b, T c) { x = a; y = b; z = c; }
    //void print(const std::string& s = "") const;    // for debugging
    void print(const std::string& s) const {
        std::cout << s << x << " " << y << " " << z << std::endl;
	}

    // stream output
    friend std::ostream& operator<<(std::ostream& os, const vec<T>& v) {
        os << "{ " << v.x << ", " << v.y << ", " << v.z << " }";
        return os;
    }
};
// non-members
// vec operator-(const vec);
template <typename T>
vec<T> operator-(vec<T> a){
	vec<T> r=a;
	a.x=-a.x;
	a.y=-a.y;
	a.z=-a.z;
	return a;
}
// vec operator+(vec, const vec&); 
template <typename T>
vec<T> operator+(vec<T> a, const vec<T>& b){
	a += b;
	return a;
}
// vec operator-(vec, const vec&);
template <typename T>
vec<T> operator-(vec<T> a, const vec<T>& b){
	a -= b;
	return a;
}
// vec operator*(vec, T); 
template <typename T>
vec<T> operator*(vec<T> a, T n){
	a *= n;
	return a;
}
// vec operator*(T, vec);
template <typename T>
vec<T> operator*(T n, vec<T> a){
	a *= n;
	return a;
}
// vec operator/(const vec&, T);
template <typename T>
vec<T> operator/(T n, vec<T> a){
	a /= n;
	return a;
}
 
}
#pragma once
#include<iostream>
#include<string>
namespace pp{
struct vec{
    double x,y,z; 
    // Constructor
    vec(double a, double b, double c){x=a; y=b; z=b;}
    vec() : vec(0, 0, 0) {}

    vec(const vec&) = default;  // Copy vec a = b
    vec(vec&&) = default;       // Move vec a=b+c
    ~vec() = default;           // Destructor

    // assignment
    vec& operator=(const vec&) = default; // copy assignment a=b
    vec& operator=(vec&&) = default;      // move assignment a=b+c

    //operation
    vec& operator+=(double);
    vec& operator-=(double);
    vec& operator*=(double);
    vec& operator/=(double);

    // utility
    void set(double a, double b, double c) { x = a; y = b; z = c; }
    void print(const std::string& s = "") const;    // for debugging

    // stream output
    friend std::ostream& operator<<(std::ostream&, const vec&);
};
// non-members
vec operator-(const vec);
vec operator+(const vec&, const vec&); 
vec operator-(const vec&, const vec&);
vec operator*(const vec&, double); 
vec operator*(double, const vec&);
vec operator/(const vec&, double);
 
}
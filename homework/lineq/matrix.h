#pragma once 

// What is this?
#ifndef HAVE_MATRIX_H
#define HAVE_MATRIX_H

#include<string>
#include<vector>
#include<initializer_list>
#include<functional>
#include<cmath>
#include<cassert>
#include<iostream>
#include<cstdio>
#include<iomanip>
#include<ostream>

// Definitions
#define FORV(i,v) for(int i=0;i<v.size();i++)
#define FOR_COLS(i,A) for(int i=0;i<A.size2();i++)
#define SELF (*this)

namespace pp{
struct vector {
	std::vector<double> data;
	vector(std::size_t n) : data(n) {}

	vector(std::initializer_list<double> list) :
		data(list.begin(),list.end()) {}

	vector()			                =default;
	vector(const vector&)		        =default;
	vector(vector&&)		            =default;
    ~vector()                           =default;
	vector& operator=(const vector&)    =default;
	vector& operator=(vector&&)	        =default;

    // For sizing
	int size() const {return data.size();}
	void resize(int n) {data.resize(n);}

    // For indexing
	double& operator[](int i) {return data[i];}
	const double& operator[](int i) const {return data[i];}

	vector& operator+=(const vector& other) {
        FORV(i, SELF) data[i]+=other.data[i];
        return SELF;}

	vector& operator-=(const vector& other) {
        FORV(i, SELF) data[i]-=other.data[i];
        return SELF;}

	vector& operator*=(double x) {
        FORV(i, SELF) data[i] *= x;
        return SELF;}

	vector& operator/=(double x) {
        FORV(i, SELF) data[i] /= x; 
        return SELF;}

	vector& add(double x) {
        data.push_back(x);
        return SELF;} 

	vector& push_back(double x) {
        data.push_back(x); 
        return SELF;} 

	double norm() const {
        double s = 0; 
        FORV(i,SELF)s+=SELF[i]*SELF[i];
	    return std::sqrt(s);}

	void print(std::string s="") const {
        std::cout << s;
	    FORV(i,SELF)printf("%9.3g ",(double)SELF[i]);
	    printf("\n");}

	vector map(std::function<double(double)> f) const {
        vector r=SELF;
	    for(int i=0;i<r.size();i++)r[i]=f(r[i]);
	    return r;}
};

vector operator+(const vector& v, const vector& u) {
    vector r = v; 
    r += u; 
    return r; }

vector operator-(const vector& v) {
    vector r = v; 
    FORV(i, r) r[i] = -r[i];
    return r;}

vector operator-(const vector& v, const vector& u) {
    vector r = v; 
    r -= u; 
    return r; }

vector operator*(const vector& v, double a) {
    vector r = v; 
    r *= a; 
    return r;} 

vector operator*(double a, const vector& v) {
    vector r = v; 
    r *= a; 
    return r; }

vector operator/(const vector& v, double a) {
    vector r = v; 
    r /= a; 
    return r;}

vector pow(const vector& v, int x) {
    vector r = v;
    FORV(i, r) r[i] = std::pow(v[i], x);
    return r;}

double dot(vector& v, vector& u) {
        double sum = 0;
        FORV(i, v) sum+= v[i] * u[i];
        return sum;
    }

bool approx(double x,double y,double acc=1e-6,double eps=1e-6){
	if(std::fabs(x-y) < acc)return true;
	if(std::fabs(x-y) < eps*(std::fabs(x)+std::fabs(y)))return true;
	return false;}

bool approx(const vector& v, const vector& u, double acc=1e-6, double eps=1e-6){
	if(u.size()!=v.size())return false;
	for(int i=0;i<u.size();i++)if(!approx(u[i],v[i],acc,eps))return false;
	return true;}

std::ostream& operator<<(std::ostream& os, const vector& v) {
    os << "(";
    for(int i=0;i<v.size()-1;i++) os << v[i] <<", ";
    int l = v.size()-1;
    os << v[l] << ")";
    return os;}


// Matrix object 
struct matrix {
	std::vector<vector> cols;
    matrix(std::size_t n,std::size_t m) {
        cols.resize(m);
        for(std::size_t i=0;i<m;i++) cols[i].resize(n);
    }
    
    matrix(std::initializer_list<std::initializer_list<double>> list) {
        for(auto c : list) {
            cols.push_back(vector(c));
        }
    }

	matrix()                    =default;
	matrix(const matrix& other) =default;
	matrix(matrix&& other)      =default;
    ~matrix()                   =default;


	matrix& operator=(const matrix& other)  =default;
	matrix& operator=(matrix&& other)       =default;

    // Sizing
	int size1() const {return cols.empty() ? 0 : cols[0].size(); }
	int size2() const {return cols.size();}
	void resize(int n, int m){
	    cols.resize(m);
	    for(int i=0;i<m;++i)cols[i].resize(n);
	}
	void setid(){
        assert(size1()==size2());
	    for(int i=0;i<size1();i++){
            SELF(i,i)=1;
            for(int j=i+1;j<size1();j++)SELF(i, j)=SELF(j, i)=0;
            }
	    }

    // Indexing
	double get (int i, int j) {return cols[j][i];}
	void set(int i, int j, double value){cols[j][i] = value;}
	double& operator()(int i, int j){return cols[j][i];}
    const double& operator()(int i, int j) const {return cols[j][i];}
	//double& operator[](int i, int j){return cols[j][i];}
	//const double& operator[](int i, int j) const {return cols[j][i];}
	vector& operator[](int i){return cols[i];}
	const vector& operator[](int i) const {return cols[i];}
//	vector get_col(int j);
//	void set_col(int j,vector& cj);

    // Transpose
	matrix transpose() const {
        matrix R(size2(),size1());
        for(int i=0;i<R.size1();i++)
            for(int j=0;j<R.size2();j++) R(i, j)=SELF(j, i);
        return R;
        }

	matrix T() const{return SELF.transpose();}

	matrix& operator+=(const matrix& other) {
        FOR_COLS(i, SELF) SELF[i] += other[i];
        return SELF;}

	matrix& operator-=(const matrix& other) {
        FOR_COLS(i, SELF) SELF[i] -= other[i];
        return SELF;} 

    // Rewrite
	// matrix& operator*=(const matrix& other) {
    //     FOR_COLS(i, SELF) SELF[i] *= other[i];
    //     return SELF;}

	matrix& operator*=(const double x) {
        FOR_COLS(i, SELF) SELF[i] *= x;
        return SELF;}

	matrix& operator/=(const double x) {
        FOR_COLS(i, SELF) SELF[i] /= x;
        return SELF;} 


	void print(std::string s="") const{
        std::cout << s << std::endl;
        for(int i=0;i<size1();i++){
            for(int j=0;j<size2();j++)printf("%9.3g ",(double)SELF(i, j));
            printf("\n");
            }
        printf("\n");
        }
};

matrix operator/(const matrix& A,double x){
	matrix R=A;
	R/=x;
	return R; }

matrix operator*(const matrix& A,double x){
	matrix R=A;
	R*=x;
	return R; }

matrix operator*(double x,const matrix& A){
	return A*x; }

matrix operator+(const matrix& A, const matrix& B){
	matrix R=A;
	R+=B;
	return R; }

matrix operator-(const matrix& A, const matrix& B){
	matrix R=A;
	R-=B;
	return R; }

vector operator*(const matrix& M, const vector& v){
	vector r; r.resize(M.size1());
	for(int i=0;i<r.size();i++){
		double sum=0;
		for(int j=0;j<v.size();j++)sum+=M(i, j)*v[j];
		r[i]=sum;
		}
	return r;
	}

matrix operator*(const matrix& A, const matrix& B){
	matrix R(A.size1(),B.size2());
	for(int k=0;k<A.size2();k++)
	for(int j=0;j<B.size2();j++)
		{
		for(int i=0;i<A.size1();i++)R(i, j)+=A(i,k)*B(k,j);
		}
	return R;
	}

matrix pow (const matrix& M, int x) {
    matrix R = M;
    FOR_COLS(i, R) R[i] = pow(M[i], x);
    return R;}

std::ostream& operator<<(std::ostream& os, const matrix& M) {
    int rows = M.size1();
    int cols = M.size2();
    os << std::fixed << std::setprecision(3);
    for (int i = 0; i < rows; ++i) {
        os << "[";
        for (int j = 0; j < cols; ++j) {
            // Using m(i,j) for row i, column j
            os << std::setw(6) << std::right << M(i, j);
            if (j < cols - 1) os << " ";
        }
        if (i != rows - 1) os << "]\n";
    }
    os << "]";
    return os;}

}
#endif

#include"matrix.h"
#include<string>
#include<algorithm>
#include<cmath>
#include<iostream>
#include<cassert>
#include<stdio.h>
#define SELF (*this)
#define FORV(i,v) for(int i=0;i<v.size();i++)
#define FOR_COLS(i,A) for(int i=0;i<A.size2();i++)
namespace pp{

bool approx(double x,double y,double acc=1e-6,double eps=1e-6){
	if(std::fabs(x-y) < acc)return true;
	if(std::fabs(x-y) < eps*(std::fabs(x)+std::fabs(y)))return true;
	return false;
}

bool approx(const vector& u,const vector& v,double acc,double eps){
	if(u.size()!=v.size())return false;
	for(int i=0;i<u.size();i++)if(!approx(u[i],v[i],acc,eps))return false;
	return true;
}

vector& vector::operator+=(const vector& other) {
	FORV(i,SELF) data[i]+=other.data[i];
	return SELF; }

vector& vector::operator-=(const vector& other) {
	FORV(i,SELF) data[i]-=other.data[i];
	return SELF; }

vector& vector::operator*=(double x) {
	FORV(i,SELF) data[i]*=x;
	return SELF; }

vector& vector::operator/=(double x) {
	FORV(i,SELF) data[i]/=x;
	return SELF; }

vector& vector::add(double x){
	data.push_back(x);
	return SELF;}

vector& vector::push_back(double x){
	data.push_back(x);
	return SELF;}

double vector::norm() const {
	double s=0;
	FORV(i,SELF)s+=SELF[i]*SELF[i];
	return std::sqrt(s);
	}

vector vector::map(std::function<double(double)> f) const{
	vector r=SELF;
	for(int i=0;i<r.size();i++)r[i]=f(r[i]);
	return r;
	}

void vector::print(std::string s) const {
	std::cout << s;
	FORV(i,SELF)printf("%9.3g ",(double)SELF[i]);
	printf("\n");
	}

vector operator/(const vector& v, double x){
	vector r=v;
	r/=x;
	return r; }

vector operator*(const vector& v, double x){
	vector r=v;
	r*=x;
	return r; }

vector operator*(double x,const vector& a){ return a*x; }

vector operator+(const vector& a, const vector& b){
	vector r=a;
	r+=b;
	return r; }

vector operator-(const vector& a){
	vector r=a;
	for(int i=0;i<r.size();i++)r[i]=-r[i];
	return r; }

vector operator-(const vector& a, const vector& b){
	vector r=a;
	r-=b;
	return r; }

void matrix::resize(int n, int m){
	cols.resize(m);
	for(int i=0;i<m;++i)cols[i].resize(n);
	}

matrix& matrix::operator+=(const matrix& other) {
	FOR_COLS(i,SELF) SELF[i]+=other[i];
	return SELF; }

matrix& matrix::operator-=(const matrix& other) {
	FOR_COLS(i,SELF) SELF[i]-=other[i];
	return SELF; }

matrix& matrix::operator*=(double x) {
	FOR_COLS(i,SELF) SELF[i]*=x;
	return SELF; }

matrix& matrix::operator/=(double x) {
	FOR_COLS(i,SELF) SELF[i]/=x;
	return SELF; }

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
		for(int j=0;j<v.size();j++)sum+=M[i,j]*v[j];
		r[i]=sum;
		}
	return r;
	}

matrix operator*(const matrix& A, const matrix& B){
	matrix R(A.size1(),B.size2());
	for(int k=0;k<A.size2();k++)
	for(int j=0;j<B.size2();j++)
		{
		for(int i=0;i<A.size1();i++)R[i,j]+=A[i,k]*B[k,j];
		}
	return R;
	}
/*
vector matrix::get_col(int j){
	vector cj=SELF[j];
	return cj;
	}

void matrix::set_col(int j,vector& cj){
	SELF[i]=cj;
	}
*/

void matrix::setid(){
	assert(size1()==size2());
	for(int i=0;i<size1();i++){
		SELF[i,i]=1;
		for(int j=i+1;j<size1();j++)SELF[i,j]=SELF[j,i]=0;
		}
	}

matrix matrix::transpose() const {
	matrix R(size2(),size1());
	for(int i=0;i<R.size1();i++)
		for(int j=0;j<R.size2();j++) R[i,j]=SELF[j,i];
	return R;
	}

matrix matrix::T() const {return SELF.transpose();}

void matrix::print(std::string s) const {
	std::cout << s << std::endl;
	for(int i=0;i<size1();i++){
		for(int j=0;j<size2();j++)printf("%9.3g ",(double)SELF[i,j]);
		printf("\n");
		}
	printf("\n");
	}
}//pp

#pragma once
#include "matrix.h"

#define SELF (*this)

// Jacobi diagonalization of symmetric matrix M.
// Iteratively zeroes off-diagonal elements via Givens rotations.
// Returns eigenvalues (diagonal of rotated M) and eigenvector matrix V.
namespace pp {

struct EVD{
    matrix D;
    matrix V; 

    EVD(const matrix& M) {
        D = M;
        size_t n = D.size1();

        // Identity matrix
        V.resize(n, n);
        for (size_t i = 0; i < n; ++i) V(i, i) = 1.0;
        jacobi(D, V);
    }

    EVD() = default;
    ~EVD() = default;

    static void jacobi(matrix& D, matrix& V) {
        size_t n = D.size1();
        bool changed;
        do{
            changed=false;
            for(size_t p=0;p<n-1;p++)
            for(size_t q=p+1;q<n;q++){
                double apq=D(p,q), app=D(p,p), aqq=D(q,q);
                double theta=0.5*std::atan2(2*apq,aqq-app);
                double c=std::cos(theta),s=std::sin(theta);
                double new_app=c*c*app-2*s*c*apq+s*s*aqq;
                double new_aqq=s*s*app+2*s*c*apq+c*c*aqq;
                if(new_app!=app || new_aqq!=aqq) // do rotation
                    {
                    changed=true;
                    timesJ(D,p,q, theta); // A←A*J 
                    Jtimes(D,p,q,-theta); // A←JT*D 
                    timesJ(V,p,q, theta); // V←V*J
                    }
            }
        }while(changed);
    }

    static void timesJ(matrix& D, size_t p, size_t q, double theta){
        double c=std::cos(theta), s=std::sin(theta);
        for(size_t i=0;i<D.size1();i++){
            double aip=D(i,p),aiq=D(i,q);
            D(i,p)=c*aip-s*aiq;
            D(i,q)=s*aip+c*aiq;
        }
    }

    static void Jtimes(matrix& D, size_t p, size_t q, double theta){
        double c=std::cos(theta),s=std::sin(theta);
        for(size_t j=0;j<D.size1();j++){
            double apj=D(p,j),aqj=D(q,j);
            D(p,j)= c*apj+s*aqj;
            D(q,j)=-s*apj+c*aqj;
        }
    }
};

}
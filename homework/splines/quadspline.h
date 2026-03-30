#pragma once

#include <vector>
#include <assert.h>

using vector = std::vector<double>;

struct qspline {
    const int n; 
    vector x,y,b,c;
    qspline(const vector& x_in, const vector& y_in) 
        : n(x_in.size()), x(x_in), y(y_in), b(n-1), c(n-1) 
    {
        // Calculate dy and dx
        vector dy(n-1), dx(n-1);
        for(int i=0; i<n-1; i++) {
            dy[i] = y[i+1] - y[i]; 
            dx[i] = x[i+1] - x[i];
        }

        // Calculate c
        vector p(n-1);
        p[0] = dy[0]/dx[0];
        c[0] = 0.0;
        for (int i=0; i<n-2; i++) { // Up
            p[i+1] = dy[i+1]/dx[i+1];
            c[i+1] = (p[i+1]-p[i]-c[i]*dx[i])/dx[i+1];
        } 
        c[n-2] /= 2.0;
        for (int i=n-3; i>=0; i--) { // Down
            c[i] = (p[i+1]-p[i]-c[i+1]*dx[i+1])/dx[i];
        }

        // Calculate b
        for (int i=0; i<n-1; i++) b[i] = p[i]-c[i]*dx[i];
    }

    qspline() = default;
    ~qspline() = default;

    double eval(const double& z) {
        int i = binsearch(x, z);
        return y[i] + b[i]*(z-x[i]) + c[i]*(z-x[i])*(z-x[i]);
    }

    double deriv(const double& z) {
        int i = binsearch(x, z);
        double dx = z-x[i];
        return b[i] + 2.0 * c[i]*dx;
    }

    double integ(const double& z) {
        int i = binsearch(x, z);
        double res = 0.0; 

        // Integral up to before point
        for (int k=0; k<i; k++) {
            double dx = x[k+1] - x[k];
            res += y[k]*dx + 0.5*b[k] * dx*dx + c[k]/3.0 * dx*dx*dx;
        }

        // up to point
        double dxz = z - x[i];
        res += y[i]*dxz + 0.5*b[i] * dxz*dxz + c[i]/3.0 * dxz*dxz*dxz;

        return res;
    }

    int binsearch(const vector& x, double z)
        {/* locates the interval for z by bisection */ 
        assert(z>=x[0] && z<=x[x.size()-1]);
        int i=0, j=x.size()-1;
        while(j-i>1){
            int mid=(i+j)/2;
            if(z>x[mid]) i=mid; else j=mid;
        }
        return i;
    }
};
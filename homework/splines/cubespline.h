#pragma once

#include <vector>
#include <assert.h>

using vector = std::vector<double>;

struct cspline {
    const int n; 
    vector x,y,b,c,d;
    cspline(const vector& x_in, const vector& y_in) 
        : n(x_in.size()), x(x_in), y(y_in), b(n), c(n-1), d(n-1) 
    {
        // Calculate dx and p
        vector dx(n-1), p(n-1);
        for(int i=0; i<n-1; i++) {
            dx[i] = x[i+1] - x[i];
            p[i] = (y[i+1] - y[i]) / dx[i]; 
        }

        // Calculate D, Q and B
        vector D(n), Q(n-1), B(n);

        D[0] = 2.0; D[n-1] = 2.0; 
        Q[0] = 1.0;
        B[0] = 3*p[0]; B[n-1] = 3*p[n-2];

        for (int i=0; i<n-2; i++) {
            double frac = dx[i]/dx[i+1];
            D[i+1] = 2 * frac + 2;
            Q[i+1] = frac;
            B[i+1] = 3 * (p[i] + p[i+1] * frac);
        }

        // Gauss elimination
        for(int i=1; i<n; i++) {
            D[i] -= Q[i-1] / D[i-1];
            B[i] -= B[i-1] / D[i-1];
        }

        // Back substitution
        b[n-1] = B[n-1]/D[n-1];
        for(int i=n-2; i>=0; i--) {
            b[i] = (B[i] - Q[i] * b[i+1]) / D[i];
        }

        for(int i=0; i<n-1; i++) {
            c[i] = (-2*b[i] - b[i+1] + 3*p[i]) / dx[i];
            d[i] = (b[i] + b[i+1] - 2*p[i]) / dx[i] / dx[i];
        }
    }

    cspline() = default;
    ~cspline() = default;

    double eval(const double& z) {
        int i = binsearch(x, z);
        double dx = z - x[i];
        return y[i] + dx * (b[i] + dx * (c[i] + dx * d[i]));
    }

    double deriv(const double& z) {
        int i = binsearch(x, z);
        double dx = z - x[i];
        return b[i] + dx * (2.0*c[i] + 3.0*d[i] * dx);
    }

    double deriv2(const double&z) {
        int i = binsearch(x, z);
        double dx = z - x[i];
        return 2.0*c[i] + 6.0*d[i]*dx;
    }

    double integ(const double& z) {
        int i = binsearch(x, z);
        double res = 0.0; 

        // Integral up to before point
        for (int k=0; k<i; k++) {
            double dx = x[k+1] - x[k];
            res += dx * (y[k] + dx * (b[k]*0.5 + dx * (c[k]/3.0 + dx * (d[k]*0.25))));
        }

        // up to point
        double dxz = z - x[i];
        res += dxz * (y[i] + dxz * (b[i]*0.5 + dxz * (c[i]/3.0 + dxz * (d[i]*0.25))));

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
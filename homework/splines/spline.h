#include <vector>
#include <assert.h>

using vector = std::vector<double>;

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

double linterp(vector x, vector y, double z){
	int i=binsearch(x,z);
  	double dx=x[i+1]-x[i];
  	assert(dx>0);
  	double dy=y[i+1]-y[i];
  	return y[i]+dy/dx*(z-x[i]);
}

double linterpInter(vector x, vector y, double z) {
    // Using trapesoid area
    // A = (x_1 - x_0) * 1/2 (y_1+ y_0)

    double i = binsearch(x, z);

    double A = 0; 
    for (int j = 0; j<i; j++) A += 0.5 * (x[j+1]-x[j]) * (y[j+1]+y[j]);

    double y_z = linterp(x, y, z);

    A += 0.5 * (z-x[i]) * (y_z + y[i]);

    return A;
}
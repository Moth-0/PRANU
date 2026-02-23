#include"matrix.h"
#include<tuple>


namespace pp{
struct QR {
    matrix Q;
    matrix R;

    QR(matrix& A) {
        int rows =A.size1(), cols = A.size2();
        Q = A;
        R.resize(rows, cols);
        
        // Fill Q with normalized vectors of input matrix
        for(int i=0;i<cols;i++) {
            double v = Q[i].norm();
            R(i,i) = v;
            Q[i] /= v;

            for(int j=i+1;j<cols;j++){
                R(i, j) = dot(Q[i], Q[j]);
                Q[j] -= R(i, j) * Q[i];
            }
        }
    }
};
}
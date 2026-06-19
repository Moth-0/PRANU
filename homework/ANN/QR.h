#include"matrix.h"
#include<tuple>


namespace pp{
struct QR {
    matrix Q;
    matrix R;

    QR(matrix& A) {
        size_t rows =A.size1(), cols = A.size2();
        Q = A;
        R.resize(rows, cols);
        
        // Fill Q with normalized vectors of input matrix
        for(size_t i=0;i<cols;i++) {
            double v = Q[i].norm();
            R(i,i) = v;
            Q[i] /= v;

            for(size_t j=i+1;j<cols;j++){
                R(i, j) = dot(Q[i], Q[j]);
                Q[j] -= R(i, j) * Q[i];
            }
        }
    }

    vector solve(vector b) {
        size_t n = b.size();
        vector y = Q.T()*b;
        vector x(n);

        for (size_t i=n; i>0; ){
            i -= 1;
            long double sum = 0;
            for (size_t j=i+1; j<n; j++) {
                sum += R(i, j) * x[j];
            }
            x[i] = (y[i] - sum)/R(i, i);
        }

        return x;
    }

    double long det() {
        double long out = 1;
        for (size_t i=0; i<R.size1(); i++) {
            out *= R(i, i);
        }
    
        return out;
    }

    matrix inverse() {
        size_t n = R.size1();
        matrix A_inv(n, n);

        for (size_t i=0; i<n; i++) {
            vector e(n); // initialize 0 vector
            e[i] = 1.0;   // identity vector
            vector col = solve(e);
            std::cout << col << std::endl;
            A_inv[i] = col;
        }

    return A_inv;
    }
};

}
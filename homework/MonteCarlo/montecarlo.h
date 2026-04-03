#include <cmath>
#include <vector>
#include <functional>
#include <random>

using vector = std::vector<double>;
using tuple = std::tuple<double, double>;
using Func = std::function<double(vector)>;

// Random double between lo and hi
double randd(double lo, double hi) {
    static std::mt19937_64 rng(std::random_device{}());
    std::uniform_real_distribution<double> dist(lo, hi);
    return dist(rng);
}

struct Halton {
    int dim;
    std::vector<int> bases;

    Halton(int n) {
        dim = n; 
        bases = prime_numbers(dim);
    }

    vector random_doubles(int n) {
        vector out(dim);
        for(int i=0; i<dim; i++) {
            int b = bases[i]; 
            out[i] = corput(n, b);
        }
        return out;
    }

    double corput(int n, int b=2) {
        double q = 0.0; double bk = 1.0/b;
        while (n>0) {
            q += (n % b) * bk;
            n /= b; 
            bk /= b;
        }
        return q;
    }   

    std::vector<int> prime_numbers(int n) {
        std::vector<int> primes; int cand = 2; 
        while ((int)primes.size() < n) {
            bool is_prime = true;
            for (int i=0; i<(int)primes.size(); i++) {
                if (primes[i]*primes[i] > cand) break;
                if (cand%primes[i] == 0) {is_prime = false; break;}
            }
            if (is_prime) primes.push_back(cand);
            cand += 1;  
        }
        return primes;
    }

};

// Following book
tuple plainmc(Func f, vector& a, vector& b, int N) {
    int dim = a.size(); 
    double V=1.0; for(int i=0; i<dim; i++) V *= b[i]-a[i];
    double sum1=0.0; double sum2=0.0;

    for (int i=0; i<N; i++) {
        vector x(dim); for(int j=0; j<dim; j++) x[j] = randd(a[j], b[j]);
        double fx = f(x); sum1+=fx; sum2+=fx*fx;
    }

    // Mean and variance
    double mean = sum1/N; double var2 = sum2/N-mean*mean;
    return {mean*V, std::sqrt(var2/N)*V};
}


tuple quacimc(Func f, vector& a, vector& b, int N) {
    int dim = a.size();
    double V=1.0; for(int i=0; i<dim; i++) V *= b[i]-a[i];
    double sum1=0.0; double sum2=0.0;
    Halton halton(dim);

    // First sequence 
    for (int i=0; i<0.5*N; i++) {
        vector x_new = halton.random_doubles(i);
        vector x(dim);
        for (int j=0; j<dim; j++) {
            x[j] = a[j]+x_new[j]*(b[j]-a[j]);
        }
        sum1 += f(x);
    }

    // Second
    for (int i=0.5*N+1; i<N; i++) {
        vector x_new = halton.random_doubles(i);
        vector x(dim);
        for (int j=0; j<dim; j++) {
            x[j] = a[j]+x_new[j]*(b[j]-a[j]);
        }
        sum2 += f(x);
    }

    // Mean and variance
    double mean = (sum1+sum2)/N; double err = std::abs(sum1-sum2)/N;
    return {mean*V, err*V};
}

tuple stratifiedmc(Func f, vector& a, vector& b, int N, int nmin=100) {
    int dim = a.size();
    
    // Base Case: If N < nmin, return plain MC estimate
    if (N < nmin) {
        return plainmc(f, a, b, N);
    }

    // Sample nmin points
    std::vector<vector> pts(nmin, vector(dim));
    vector fvals(nmin);

    for (int i=0; i<nmin; i++) {
        for(int j=0; j<dim; j++) {
            pts[i][j] = randd(a[j], b[j]);
        }
        fvals[i] = f(pts[i]);
    }

    return {0.0,0.0};
}
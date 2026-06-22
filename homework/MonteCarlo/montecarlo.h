#include <cmath>
#include <vector>
#include <functional>
#include <random>

using vector = std::vector<double>;
using tuple = std::tuple<double, double>;
using Func = std::function<double(vector)>;


struct rng {
    uint32_t seed;
    uint32_t a;
    uint32_t c;
    uint64_t m;

    rng(uint seed_in, uint32_t a_in = 1664525, uint32_t c_in = 1013904223, uint64_t m_in = std::pow(2, 32)){
        seed = seed_in; a = a_in; c = c_in; m = m_in;
    }

    rng() = default;
    ~rng() = default;

    double uniform(double low=0, double high=1) {
        seed = (a * seed + c) % m;
        return (high-low) * (seed+1)/(m+1) + low;
    }
};

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
    if (N <= 0) return {0.0, 0.0};

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

tuple cppmc(Func f, vector& a, vector& b, int N) {
    int dim = a.size(); 
    double V=1.0; for(int i=0; i<dim; i++) V *= b[i]-a[i];
    double sum1=0.0; double sum2=0.0;
    rng rand(42);

    for (int i=0; i<N; i++) {
        vector x(dim); for(int j=0; j<dim; j++) x[j] = rand.uniform(a[j], b[j]);
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
    for (int i=1; i<=0.5*N; i++) {
        vector x_new = halton.random_doubles(i);
        vector x(dim);
        for (int j=0; j<dim; j++) {
            x[j] = a[j]+x_new[j]*(b[j]-a[j]);
        }
        sum1 += f(x);
    }

    // Second
    for (int i=0.5*N+1; i<=N; i++) {
        vector x_new = halton.random_doubles(i);
        vector x(dim);
        for (int j=0; j<dim; j++) {
            x[j] = a[j]+x_new[j]*(b[j]-a[j]);
        }
        sum2 += f(x);
    }

    // Mean and variance
    double mean = (sum1+sum2)/N; 
    double err = std::abs(sum1-sum2)/(0.5*N);

    return {mean*V, err*V};
}

tuple stratifiedmc(Func f, vector& a, vector& b, int N, int nmin=100) {
    int dim = a.size();
    
    // Base Case: If N <= nmin, return plain MC estimate
    if (N <= nmin) {
        return plainmc(f, a, b, N);
    }

    std::vector<vector> pts(nmin, vector(dim));
    vector fvals(nmin);

    for (int i = 0; i < nmin; i++) {
        for(int j = 0; j < dim; j++) {
            // Using your randd() so the RNG state doesn't reset on recursive calls
            pts[i][j] = randd(a[j], b[j]); 
        }
        fvals[i] = f(pts[i]);
    }

    // Find the dimension with the largest sub-variance
    int max_dim = 0;
    double max_var = -1.0;
    double best_varL = 0.0;
    double best_varR = 0.0;

    for (int d = 0; d < dim; d++) {
        double mid = (a[d] + b[d]) / 2.0;
        double sumL = 0, sum2L = 0, sumR = 0, sum2R = 0;
        int nL = 0, nR = 0;

        // Split the evaluated points into left and right sub-volumes for dimension 'd'
        for (int i = 0; i < nmin; i++) {
            if (pts[i][d] < mid) {
                sumL += fvals[i];
                sum2L += fvals[i] * fvals[i];
                nL++;
            } else {
                sumR += fvals[i];
                sum2R += fvals[i] * fvals[i];
                nR++;
            }
        }

        // Calculate variance for the left and right halves
        double varL = (nL > 0) ? (sum2L / nL - (sumL / nL) * (sumL / nL)) : 0.0;
        double varR = (nR > 0) ? (sum2R / nR - (sumR / nR) * (sumR / nR)) : 0.0;
        double total_var = varL + varR;

        if (total_var > max_var) {
            max_var = total_var;
            max_dim = d;
            best_varL = varL;
            best_varR = varR;
        }
    }

    // Subdivide the volume along this dimension
    vector b_left = b;
    b_left[max_dim] = (a[max_dim] + b[max_dim]) / 2.0;
    
    vector a_right = a;
    a_right[max_dim] = (a[max_dim] + b[max_dim]) / 2.0;

    // Divide the remaining points between the two sub-volumes proportional to sub-variances
    int remaining_points = N - nmin;
    int N_left, N_right;

    if (best_varL + best_varR == 0.0) {
        N_left = remaining_points / 2;
    } else {
        N_left = std::round(remaining_points * best_varL / (best_varL + best_varR));
    }
    N_right = remaining_points - N_left;

    // Recursive calls on the sub-volumes
    auto [I_L, E_L] = stratifiedmc(f, a, b_left, N_left, nmin);
    auto [I_R, E_R] = stratifiedmc(f, a_right, b, N_right, nmin);

    // Estimate the grand integral and grand error
    return {I_L + I_R, std::sqrt(E_L * E_L + E_R * E_R)};
}
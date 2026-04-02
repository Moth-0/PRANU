#include <cmath>
#include <vector>
#include <functional>
#include <random>

using vector = std::vector<double>;
using Func = std::function<double(vector)>;

// Random double between lo and hi
double randd(double lo, double hi) {
    static std::mt19937_64 rng(std::random_device{}());
    std::uniform_real_distribution<double> dist(lo, hi);
    return dist(rng);
}

// Following book
std::tuple<double, double> plainmc(Func f, vector& a, vector& b, int N) {
    int dim = a.size(); 
    double V=1; for(int i=0; i<dim; i++) V *= b[i]-a[i];
    double sum1=0; double sum2=0;

    for (int i=0; i<N; i++) {
        vector x(dim); for(int i=0; i<dim; i++) x[i] = randd(a[i], b[i]);
        double fx = f(x); sum1+=fx; sum2+=fx*fx;
    }

    // Mean and variance
    double mean = sum1/N; double var2 = sum2/N-mean*mean;
    return {mean*V, std::sqrt(var2/N)*V};
}
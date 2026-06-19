#include <iostream>
#include <vector>
#include <cmath>
#include <random>

int main(int argc, char* argv[]) {
    int num_samples = 100;
    if (argc > 1) num_samples = std::stoi(argv[1]); 

    // noise
    std::mt19937 gen(42); 
    std::normal_distribution<double> noise_dist(0.0, 0.05);

    std::cout << "# x y\n";
    // Generate the points
    for (int i = 0; i < num_samples; ++i) {
        double x = -1.0 + 2.0 * i / (num_samples - 1.0);
        
        double pure_y = std::cos(5.0 * x - 1.0) * std::exp(-x * x);
        double noisy_y = pure_y + noise_dist(gen);

        std::cout << x << " " << noisy_y << "\n";
        
    }    
    return 0;
}
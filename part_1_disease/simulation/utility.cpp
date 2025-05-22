#include "simulation.h"

/**
 * @brief Common random engine
 * @return std::default_random_engine& 
 */
std::default_random_engine& Utility::engine() {
    static std::default_random_engine gen(std::random_device{}());
    return gen;
}

/**
 * @brief Generates random number between 0.0 and 1.0
 * @return float
 */
float Utility::gen_random_num() {
    std::uniform_real_distribution<float> dis(0.0f,1.0f);
    return dis(engine());
}

/**
 * @brief Random indices for the given elements size
 * @param elements_size 
 * @return std::vector<int> 
 */
std::vector<int> Utility::randomized_indices(int elements_size, int sample_size) {
    std::vector<int> sample_indices;
    std::uniform_int_distribution<int> dis(0, elements_size - 1);
    for(int ind = 0; ind < sample_size; ++ind) {
        sample_indices.push_back(dis(engine()));
    }
    return sample_indices;
}

/**
 * @brief Shuffle indicies for the given elements size
 * @param elements_size 
 * @return std::vector<int> 
 */
std::vector<int> Utility::shuffled_indices(int elements_size) {
    std::vector<int> indices(elements_size);
    std::iota(indices.begin(), indices.end(), 0);
    std::shuffle(indices.begin(), indices.end(), Utility::engine());
    return indices;
}

/**
 * @brief Calculate mean and standard deviation
 * @param elements 
 * @return std::pair<double, double> 
 */
std::pair<double, double> Utility::calc_mean_stdev(
    const std::vector<int>& elements) {
    int size = elements.size();
    if (size == 0) return {0, 0};

    double sum = std::accumulate(elements.begin(), elements.end(), 0.0);
    double mean = sum / size;

    double square_sum = 0.0;
    for (double value : elements) {
        double diff = value - mean;
        square_sum += diff * diff;
    }

    double stdev = std::sqrt(square_sum / size);

    return {mean, stdev};
}

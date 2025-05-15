#include "simulation.h"
#include <random>

/**
 * @brief Generates random number between 0.0 and 1.0
 * @return float
 */
float Utility::gen_random_num() {
    std::random_device rd;
    std::default_random_engine gen(rd());
    std::uniform_real_distribution<float> dis(0.0f,1.0f);

    return dis(gen);
}

/**
 * @brief Randomizes indices of the given elements size
 * @return vector<int>
 */
 std::vector<int> Utility::randomized_indices(int elements_size) {
    std::vector<int> indices(elements_size);
    std::iota(indices.begin(), indices.end(), 0);
    
    for(int index = elements_size - 1; index > 0; index--) {
        int swap_index = gen_random_num() * (index + 1);
        std::swap(indices[index], indices[swap_index]);
    }

    return indices;
}

/**
 * @brief Randomizes fixed elements of size (sample_size) for the given indices
 * @return vector<int>
 */
std::vector<int> Utility::random_fixed_elements(std::vector<int> indices, int sample_size) {
    std::random_device rd;
    std::default_random_engine gen(rd());

    for (int i = 0; i < sample_size; ++i) {
        std::uniform_int_distribution<int> dis(i, indices.size() - 1);
        int swap_index = dis(gen);
        std::swap(indices[i], indices[swap_index]);
    }

    return std::vector<int>(indices.begin(), indices.begin() + sample_size);
}
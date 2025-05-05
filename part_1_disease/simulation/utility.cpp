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
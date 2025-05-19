#include "simulation.h"

/**
 * @brief Common random engine
 * @return reference of default_random_engine
 */
std::default_random_engine& Utility::engine() {
    static std::random_device rd;
    static std::default_random_engine gen(rd());
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
 * @brief Random index for the given elements size
 * @return int
 */
 int Utility::randomized_indices(int elements_size) {
    std::uniform_int_distribution<int> dis(0, elements_size - 1);
    return dis(engine());
}
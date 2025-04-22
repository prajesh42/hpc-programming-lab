#include "simulation.h"
#include <random>

/**
 * @brief Generates random number between 0.1 and 1.0
 * @return float
 */
float Utility::gen_random_num() {
    std::random_device rd;
    std::default_random_engine gen(rd());
    std::uniform_real_distribution<float> dis(0.1,1.0);

    return dis(gen);
}
#include "misc/RandomGenerator.hpp"

#include <cstdlib>

RandomGenerator::RandomGenerator() {

}

RandomGenerator::~RandomGenerator() {
    
}

float RandomGenerator::randFromRange(float lowerBound, float upperBound) {
    std::uniform_real_distribution<float> distribution(lowerBound, upperBound);
    return distribution(m_generator);   
}


int RandomGenerator::randomInt() {
    return std::rand();
}
#pragma once

#include <iostream>
#include <random>

class RandomGenerator {
public:
    RandomGenerator();
    ~RandomGenerator();

    float randFromRange(float lowerBound, float upperBound);
    int randomInt();
private:
    std::default_random_engine m_generator;
};
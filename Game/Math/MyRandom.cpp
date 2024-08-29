#include "MyRandom.h"

float RandomFloat(const float& min, const float& max) {
    std::uniform_real_distribution<float> distribution(min, max);
    float result = distribution(randomEngine);
    return result;
}

Vector3 RandomVector3(const float& min, const float& max) {
    std::uniform_real_distribution<float> distribution(min, max);
    float x = distribution(randomEngine);
    float y = distribution(randomEngine);
    float z = distribution(randomEngine);

    return Vector3{ x, y, z };
}

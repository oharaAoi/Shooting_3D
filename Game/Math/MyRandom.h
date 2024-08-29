#pragma once
#include <random>
#include "Vector3.h"

inline std::random_device seedGenerator;
inline std::mt19937 randomEngine(seedGenerator());

float RandomFloat(const float& min, const float& max);

Vector3 RandomVector3(const float& min, const float& max);
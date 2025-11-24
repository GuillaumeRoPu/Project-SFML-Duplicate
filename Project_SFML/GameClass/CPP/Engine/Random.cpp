#include "Engine/FryEngine.h"

using namespace fry;

void Random::Init()
{
    // Graine basée sur un nombre vraiment aléatoire (hardware)
    std::random_device rd;
    generator.seed(rd());
}

int Random::Range(int min, int max)
{
    std::uniform_int_distribution<int> dist(min, max);
    return dist(generator);
}

float Random::Range(float min, float max)
{
    std::uniform_real_distribution<float> dist(min, max);
    return dist(generator);
}

bool Random::Bool()
{
    std::bernoulli_distribution dist(0.5);
    return dist(generator);
}

float Random::Value()
{
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    return dist(generator);
}
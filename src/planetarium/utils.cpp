#include "utils.h"

float Utils::GetRandomNumber(float lowerLimit, float upperLimit)
{
    return lowerLimit + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (upperLimit - lowerLimit)));
}

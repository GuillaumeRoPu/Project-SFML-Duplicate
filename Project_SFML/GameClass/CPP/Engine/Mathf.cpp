#include "Engine\Mathf.h"

float Mathf::abs(float value)
{
	return value < 0 ? value * -1 : value;
}

float Mathf::sqrt(float value)
{
	return sqrtf(value);
}

float Mathf::clamp(float value, float a, float b)
{
	return value > b ? b : value < a ? a : value;
}

float Mathf::clamp01(float value)
{
	return value > 1 ? 1 : value < 0 ? 0 : value;
}

float Mathf::Pow(float a, float p)
{
	float pow = a * a;
	if (p > 2)
	{
		for (int i = 2; i < p; i++)
		{
			pow *= a;
		}
	}

	return pow;
}

float Mathf::Lerp(float a, float b, float t)
{
	float f = b - a;
	return a + f * t;
}

float Mathf::Max(float a, float b)
{
	return a > b ? a : b;
}

float Mathf::Min(float a, float b)
{
	return a < b ? a : b;
}

#pragma once
#include <iostream>

class Mathf
{
public:

	// Return the absolute value
	static float abs(float value);

	// Return the square root of a value
	static float sqrt(float value);

	// clamp value between min and max
	static float clamp(float value, float min, float max);

	// clamp value between 0 and 1
	static float clamp01(float value);

	// Return a power p
	static float Pow(float a, float p);

	// Return the linearly interpolate float between a and b by t
	static float Lerp(float a, float b, float t);

	// Return the max between a and b
	static float Max(float a, float b);

	// Return the min between a and b
	static float Min(float a, float b);

	// Return the angle in radian
	static float Deg2rad(float deg) { return deg * PI / 180; }

	// Return the angle in degrees
	static float Rad2deg(float rad) { return rad * 180 / PI; }

	// Return the cosinus of an angle in degrees
	static float cos(float deg) { return std::cos(Deg2rad(deg)); }

	// Return the sinus of an angle in degrees
	static float sin(float deg) { return std::sin(Deg2rad(deg)); }

	// Return the angle of a vector in degrees
	static float atan2(float y, float x) { return Mathf::Rad2deg(std::atan2f(y, x)); }

	static float PI;
};
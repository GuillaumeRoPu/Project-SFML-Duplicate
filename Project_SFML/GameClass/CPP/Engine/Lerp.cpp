#include "Engine/FryEngine.h"

using namespace fry;

sf::Color Lerp::Color(sf::Color a, sf::Color b, float t)
{

	t = std::clamp(t, 0.0f, 1.0f);

	float r = (float)(b.r - a.r);
	float g = (float)(b.g - a.g);
	float bl = (float)(b.b - a.b);
	float al = (float)(b.a - a.a);

	sf::Color newColor;
	newColor.r = a.r + (uint8_t)(r * t);
	newColor.g = a.g + (uint8_t)(g * t);
	newColor.b = a.b + (uint8_t)(bl * t);
	newColor.a = a.a + (uint8_t)(al * t);

	return newColor;
}

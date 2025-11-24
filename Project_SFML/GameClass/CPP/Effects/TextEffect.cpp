#include "../../HEADER/Effects/TextEffect.h"
//#include <iostream>

sf::String TextEffect::TypeWriter(sf::String stringToType, sf::Text typeText, float timeToSleep)
{
	sf::sleep(sf::milliseconds((int)(timeToSleep * 1000)));

	if (stringToType == typeText.getString())
		return typeText.getString();

	int itt = (int)typeText.getString().getSize();
	sf::String current = typeText.getString() + stringToType[itt];

	if (stringToType[itt] == ' ' && itt < stringToType.getSize())
	{
		current += stringToType[itt + 1];
	}

	return current;
}

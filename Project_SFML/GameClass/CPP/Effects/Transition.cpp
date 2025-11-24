#include "../../HEADER/Effects/Transition.h"
#include <iostream>

bool Transition::QuadFadeOut(sf::RenderWindow& window, int sizeX, int sizeY, int nbSquare, float transitionTime)
{
	int x = 0;
	int y = 0;

	for (int i = 0; i < quadTurn; i++)
	{
		sf::RectangleShape rect(sf::Vector2f((float)sizeX / nbSquare, (float)sizeY / nbSquare));
		rect.setFillColor(sf::Color::White);
		rect.setPosition(sf::Vector2f((float)sizeX / nbSquare * x, (float)sizeY / nbSquare * y));
		window.draw(rect);

		x += 2;
		if (x >= nbSquare)
		{
			y++;
			x = x % (nbSquare);
			if (y == nbSquare)
			{
				x = 1;
				y = 0;
			}
		}
	}

	if (quadTurn > 0)
		sf::sleep(sf::milliseconds((int)(transitionTime * 1000)));
	
	quadTurn++;

	//std::cout << quadTurn << std::endl;

	if (quadTurn > nbSquare * nbSquare + 1)
		return true;

	return false;
}

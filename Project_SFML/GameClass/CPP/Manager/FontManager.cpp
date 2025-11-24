#include "Manager/FontManager.h"
#include <iostream>

sf::Font FontManager::f_pixelify;

void FontManager::InitFonts()
{
	LoadFont(f_pixelify, "res/pixelify_regular.ttf");
}

void FontManager::LoadFont(sf::Font& font, std::filesystem::path path)
{
	if (!font.openFromFile(path))
		std::cout << "Erreur de chargement de la police" << std::endl;
}
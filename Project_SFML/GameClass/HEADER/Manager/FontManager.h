#pragma once
#include <SFML\Graphics\Font.hpp>
#include <filesystem>

class FontManager
{
public:

	static void InitFonts();

	// Fonts
	static sf::Font f_pixelify;

private:

	static void LoadFont(sf::Font& font, std::filesystem::path path);

};
#pragma once
#include "Scene.h"

class lascen : public Scene 
{
public :
	lascen();
	void PlayScene(sf::RenderWindow& window, InputSystem& input) override;
	

};
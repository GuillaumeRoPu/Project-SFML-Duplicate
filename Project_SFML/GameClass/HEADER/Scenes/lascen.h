#pragma once
#include "Scene.h"
#include "Objects/Enemy.h"

class lascen : public Scene 
{
public:
	Enemy* caca;
public:
	lascen();
	void PlayScene(sf::RenderWindow& window, InputSystem& input) override;
	

};
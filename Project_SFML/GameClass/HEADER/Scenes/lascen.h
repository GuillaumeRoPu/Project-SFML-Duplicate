#pragma once
#include "Scene.h"
#include "Objects/Player.h"
#include "Objects/Enemy.h"

class lascen : public Scene
{
public:
	Player* p;
	Enemy* caca;
public:
	lascen();
	void PlayScene(sf::RenderWindow& window, InputSystem& input) override;
	

};
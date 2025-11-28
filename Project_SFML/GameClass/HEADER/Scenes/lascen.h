#pragma once
#include "Scene.h"
#include "Objects/Player.h"
#include "Objects/Enemy.h"
#include "Objects/Blocker.h"

class lascen : public Scene
{
public:
	Enemy* caca;
	Player* p;
	Blocker* blocker;
public:
	lascen();
	void PlayScene(sf::RenderWindow& window, InputSystem& input) override;
	

};
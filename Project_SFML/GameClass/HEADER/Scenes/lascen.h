#pragma once
#include "Scene.h"
#include "Objects/Player.h"

class lascen : public Scene 
{
public :
	Player* p;
public:
	lascen();
	void PlayScene(sf::RenderWindow& window, InputSystem& input) override;
};
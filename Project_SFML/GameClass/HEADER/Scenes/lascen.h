#pragma once
#include "Scene.h"
#include "Objects/Player.h"
#include "Objects/EnemyManager.h"
#include "Objects/Blocker.h"
#include "Objects/BulletManager.h"

class lascen : public Scene
{
public:
	EnemyManager* enemyManager;
	BulletManager* bulletManager;
	Player* p;
	Blocker* blocker;
public:
	lascen();
	void PlayScene(sf::RenderWindow& window, InputSystem& input) override;
	

};
#pragma once
#include "Objects/GameObject.h"
#include "Objects/Enemy.h"
#include "Components/Physics/CircleCollider2D.h"
#include "Scripts/BulletController.h"
#include "Scripts/EnemyManagerController.h"

class EnemyManager : public GameObject
{
public:
	EnemyManagerController* _enemyManagerController;

public:
	EnemyManager();
};
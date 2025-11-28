#pragma once
#include "Scripts/BulletManagerController.h"

class BulletManager : public GameObject
{
public:
	BulletManagerController* _bulletManagerController;

public:
	BulletManager();
};

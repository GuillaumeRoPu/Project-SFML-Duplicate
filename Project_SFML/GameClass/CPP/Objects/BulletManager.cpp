#include "Objects/BulletManager.h"

BulletManager::BulletManager() {
	_bulletManagerController = AddComponent<BulletManagerController>();
}
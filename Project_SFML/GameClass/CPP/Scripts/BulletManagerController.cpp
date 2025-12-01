#include "Scripts/BulletManagerController.h"

void BulletManagerController::Start()
{
	_cooldown = 5;
	_timer = 0;
}

void BulletManagerController::Awake()
{
	_player = owner->scene->FindFirstObjectByType<Player>();
}

void BulletManagerController::SpawnBullet(Vector2 position)
{
	Bullet* newBullet = InstantiateBullet();

	Vector2 direction = Vector2::Edge(position, _player->transform->position);
	newBullet->transform->position.RotateAroundCenter(position, .5f, Vector2::Angle(Vector2::right(), direction.normalized()));
	newBullet->_controller->controller = this;
	_allBullets.push_back(newBullet);
}

void BulletManagerController::DestroyBullet(GameObject* bullet)
{
	auto it = std::find_if(_allBullets.begin(), _allBullets.end(),
		[&](auto& uptr) {return uptr == bullet; });

	if (it != _allBullets.end())
		_allBullets.erase(it);

	scene().DestroyObject(bullet);
}


Bullet* BulletManagerController::InstantiateBullet()
{
	return scene().CreateGameObject<Bullet>();
}

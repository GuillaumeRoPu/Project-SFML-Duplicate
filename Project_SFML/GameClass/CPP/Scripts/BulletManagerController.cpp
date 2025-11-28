#include "Scripts/BulletManagerController.h"

void BulletManagerController::Update()
{
	_timer += Time::DeltaTime();
	if (_cooldown <= _timer)
	{
		_timer = 0;
		//SpawnBullet();
	}
}

void BulletManagerController::Start()
{
	_cooldown = 5;
	_timer = 0;
}

void BulletManagerController::Awake()
{
	_player = owner->scene->FindFirstObjectByType<Player>();
}

void BulletManagerController::SpawnBullet()
{
	Bullet* newBullet = InstantiateBullet();
	newBullet->_controller->controller = this;
	if (newBullet != nullptr && _player != nullptr) {
		Vector2 position = Vector2(Random::Range(-100 / (int)pixelPerPoint, 100 / (int)pixelPerPoint), Random::Range(-100 / (int)pixelPerPoint, 100 / (int)pixelPerPoint));
		newBullet->transform->position = (position);
	}
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

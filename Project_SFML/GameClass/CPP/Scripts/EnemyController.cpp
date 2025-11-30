#include "Scripts/EnemyController.h"
#include "Scripts/EnemyManagerController.h"
#include "Objects/GameObject.h"
#include "Objects/Bullet.h"
#include "Objects/BulletManager.h"

//void EnemyController::FixedUpdate()
//{
//	//_rb->AddForce(Vector2(1, 1));
//	//if (_rb->transform().position.x() > 1000 || _rb->transform().position.x() < -1000)
//	//{
//		//_rb->transform().position = Vector2(0, 0);
//	//}
//	if (_player != nullptr)
//	{
//		Vector2 VecteurPlayerEnemy = Vector2(_player->transform->position.x() - transform().position.x(), _player->transform->position.y() - transform().position.y());
//		float magnitude = Mathf::sqrt(VecteurPlayerEnemy.x() * VecteurPlayerEnemy.x() + VecteurPlayerEnemy.y() * VecteurPlayerEnemy.y());
//		if (magnitude >= _player->transform->scale.x()) {
//			_rb->velocity += VecteurPlayerEnemy;
//		}
//	}
//	int i = 9;
//}

void EnemyController::Update() 
{
	timer += Time::DeltaTime();
	if (timer >= cooldown) 
	{
		timer = 0;
		SpawnBullet();
	}
}

void EnemyController::Start() 
{
	_rb = owner->GetComponent<Rigidbody2D>();
	_collider = owner->GetComponent<BoxCollider2D>();
	_player = scene().FindFirstObjectByType<Player>();
	cooldown = 0.4f;
	timer = 0;
}

void EnemyController::SpawnBullet()
{
	auto manager = scene().FindFirstObjectByType<BulletManager>();
	manager->GetComponent<BulletManagerController>()->SpawnBullet(transform().position);
}

void EnemyController::OnTriggerEnter2D(Collider2D* other) 
{
	PlayerController* _player = other->owner->GetComponent<PlayerController>();
	if (_player != nullptr) {
		owner->active = false;
		std::cout << "Touched Player" << std::endl;
		controller->DestroyEnemy(owner);
	}
}

Bullet* EnemyController::InstantiateBullet() 
{
	return scene().CreateGameObject<Bullet>();
}


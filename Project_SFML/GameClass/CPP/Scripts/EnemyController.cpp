#include "Scripts/EnemyController.h"
#include "Scripts/EnemyManagerController.h"
#include "Components/ParticleSystem.h"
#include "Objects/GameObject.h"
#include "Objects/Bullet.h"
#include "Objects/BulletManager.h"
#include "Manager/AudioManager.h"

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
	AudioManager::PlaySound(AudioManager::shoot);
	auto manager = scene().FindFirstObjectByType<BulletManager>();
	manager->GetComponent<BulletManagerController>()->SpawnBullet(transform().position);
}

void EnemyController::OnTriggerEnter2D(Collider2D* other) 
{
	PlayerController* _player = other->owner->GetComponent<PlayerController>();
	if (_player != nullptr) {
		_player->Heal(1);
		owner->active = false;
		std::cout << "Touched Player" << std::endl;

		SpawnDeathParticle();

		AudioManager::PlaySound(AudioManager::destroy);
		controller->DestroyEnemy(owner);
	}
}

void EnemyController::SpawnDeathParticle()
{
	GameObject* gm_particle = scene().CreateGameObject<GameObject>();
	gm_particle->transform->position = transform().position;
	ParticleSystem* particle = gm_particle->AddComponent<ParticleSystem>();
	particle->destroyWhenDie = true;
	particle->duration = .5f;
	particle->randomLifetime = true;
	particle->minLifetime = .5f;
	particle->maxLifetime = 1.4f;
	particle->radius = .1f;
	particle->speed = 10;
	particle->burst = true;
	particle->burstNumber = 1;
	particle->rateOverTime = 20;
	particle->burstCount = 20;
	particle->size = .05f;
	particle->burstTime = 0.f;
}

Bullet* EnemyController::InstantiateBullet() 
{
	return scene().CreateGameObject<Bullet>();
}


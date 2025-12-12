#include "Scripts/EnemyController.h"
#include "Scripts/EnemyManagerController.h"
#include "Components/ParticleSystem.h"
#include "Objects/GameObject.h"
#include "Objects/Bullet.h"
#include "Objects/BulletManager.h"
#include "Manager/AudioManager.h"

void EnemyController::Update() 
{
	_cooldownTimer += Time::DeltaTime();
	if (_type == EnemyType::Burst) 
	{
		_littleCooldownTimer += Time::DeltaTime();
	}

	if (_cooldownTimer >= _cooldown) 
	{
		switch (_type)
		{
		case EnemyType::Classic:
			SpawnBullet();
			_cooldownTimer = 0;
			break;
		case EnemyType::Burst:
			if (_littleCooldownTimer >= _littleCooldown && _currentNumberShot < _numberofBulletsInString)
			{
				_littleCooldownTimer = 0;
				SpawnBullet();
				_currentNumberShot++;
			}
			else if (_currentNumberShot == _numberofBulletsInString) 
			{
				_cooldownTimer = 0;
				_littleCooldownTimer = 0;
				_currentNumberShot = 0;
			}
			break;
		case EnemyType::Shotgun:
			for (int i = 0; i < _numberOfBullets; i++) {
				SpawnBullet();
			}
			_cooldownTimer = 0;
			break;
		}
		
	}
}

void EnemyController::Start() 
{
	_rb = owner->GetComponent<Rigidbody2D>();
	_collider = owner->GetComponent<CircleCollider2D>();
	_player = scene().FindFirstObjectByType<Player>();
	_health = owner->GetComponent<HealthComponent>();
	//cooldown = 0.4f;
	_cooldownTimer = 0;
	_health->SetMaxHealth(_basehealth);
	_rb->velocity = Vector2(0.1f,0.0f);
}

void EnemyController::SpawnBullet()
{
	AudioManager::PlaySound(AudioManager::shoot, 20, (float)Random::Range(19, 21) / 20);
	auto manager = scene().FindFirstObjectByType<BulletManager>();
	auto bullet = manager->GetComponent<BulletManagerController>();
	if (_type == EnemyType::Shotgun) 
	{
		bullet->SpawnBullet(transform().position, _bulletSpeed, _bulletAcceleration, _bulletDamage, _bulletScale, _bulletColor, _bulletCanPredictPlayerPos, _bulletCanTrack, _bulletmaxTrackDistance, _spread);
	}
	else 
	{
		bullet->SpawnBullet(transform().position, _bulletSpeed, _bulletAcceleration, _bulletDamage, _bulletScale, _bulletColor, _bulletCanPredictPlayerPos, _bulletCanTrack, _bulletmaxTrackDistance);
	}
}

void EnemyController::OnTriggerEnter2D(Collider2D* other)
{
	PlayerController* _player = other->owner->GetComponent<PlayerController>();
	if (_player != nullptr) {
		
		_health->TakeDamage(_player->speed/2);

		if (_health->IsDead())
		{
			/*Time::timeScale = 0.2f;*/

			owner->active = false;

			_player->Heal(5);
			_player->score++;

			SpawnDeathParticle();

			AudioManager::PlaySound(AudioManager::destroy);
			
			controller->DestroyEnemy(owner);
		}
		else {
			_rb->damping = 1.0f;
			_rb->velocity += _player->rb->velocity * 3;
			AudioManager::PlaySound(AudioManager::collision);
		}
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

void EnemyController::BounceOnBorder(float borderX, float borderY)
{
	if (transform().position.x() * pixelPerPoint - transform().scale.x() * pixelPerPoint < -borderX / 2) {
		if (_rb->velocity.x() <= 0)
		{
			_rb->velocity = Vector2(-_rb->velocity.x(), _rb->velocity.y());
			AudioManager::PlaySound(AudioManager::collision);
		}
	}
	if (transform().position.x() * pixelPerPoint + transform().scale.x() * pixelPerPoint > borderX / 2) {
		if (_rb->velocity.x() >= 0)
		{
			_rb->velocity = Vector2(-_rb->velocity.x(), _rb->velocity.y());
			AudioManager::PlaySound(AudioManager::collision);
		}
	}
	if (transform().position.y() * pixelPerPoint - transform().scale.x() * pixelPerPoint < -borderY / 2) {
		if (_rb->velocity.y() <= 0)
		{
			_rb->velocity = Vector2(_rb->velocity.x(), -_rb->velocity.y());
			AudioManager::PlaySound(AudioManager::collision);
		}
	}
	if (transform().position.y() * pixelPerPoint + transform().scale.x() * pixelPerPoint > borderY / 2) {
		if (_rb->velocity.y() >= 0)
		{
			_rb->velocity = Vector2(_rb->velocity.x(), -_rb->velocity.y());
			AudioManager::PlaySound(AudioManager::collision);
		}
	}
}

Bullet* EnemyController::InstantiateBullet() 
{
	return scene().CreateGameObject<Bullet>();
}

void EnemyController::SetupBulletProperties(float bulletSpeed, float bulletAcceleration, float scale, int damage, sf::Color color, bool canPredictPlayerPosition, bool canTrack, float maxTrackingDistance)
{
	_bulletSpeed = bulletSpeed;
	_bulletAcceleration = bulletAcceleration;
	_bulletScale = scale;
	_bulletColor = color;
	_bulletCanPredictPlayerPos = canPredictPlayerPosition;
	_bulletDamage = damage;
	_bulletCanTrack = canTrack;
	_bulletmaxTrackDistance = maxTrackingDistance;
}

void EnemyController::SetupEnemyProperties(int health, float scale, float cooldown, sf::Color color, EnemyType type)
{
	_basehealth = health;
	transform().setLocalScale(Vector2(scale, scale));
	_cooldown = cooldown;
	this->color = color;
	_type = type;
}


void EnemyController::SetupBurstProperties(float delayBetweenShots, int numberOfShots)
{
	_littleCooldown = delayBetweenShots;
	_numberofBulletsInString = numberOfShots;
}

void EnemyController::SetupShotgunProperties(float spread, int numberOfShots)
{
	_spread = spread;
	_numberOfBullets = numberOfShots;
}


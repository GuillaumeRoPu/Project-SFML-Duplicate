#include <Scripts/PlayerController.h>
#include "Scripts/BulletController.h"
#include "Objects/Blocker.h"
#include "Components\ParticleSystem.h"
#include "InputSystem.h"
#include "Manager\AudioManager.h"

void PlayerController::Awake()
{
	_rb = owner->GetComponent<Rigidbody2D>();
	_rb->damping = 1;
	_health = owner->GetComponent<HealthComponent>();
}

void PlayerController::Update()
{
	Blocker* blocker = scene().FindFirstObjectByType<Blocker>();
	blocker->GetComponent<PlayerHealthBlocker>()->SetHP(_health->GetHealth().GetRatio());
}

void PlayerController::Move(int verDir, int horDir)
{
	if (verDir == 1) {
		_rb->velocity -= Vector2(0.0f, acceleration * Time::DeltaTime());
	}
	if (verDir == -1) {
		_rb->velocity -= Vector2(0.0f, -acceleration * Time::DeltaTime());
	}
	if (horDir == 1) {
		_rb->velocity -= Vector2(-acceleration * Time::DeltaTime(), 0.0f);
	}
	if (horDir == -1) {
		_rb->velocity -= Vector2(acceleration * Time::DeltaTime(), 0.0f);
	}
}

void PlayerController::TakeDamage(int amount)
{
	AudioManager::PlaySound(AudioManager::touch);
	_health->TakeDamage(amount);
	if (_health->IsDead())
	{
		Time::timeScale = 0.01f;
		SpawnDeathParticle();
		AudioManager::SoloSounds({ AudioManager::destroy , AudioManager::defeat });
	}
}

void PlayerController::SpawnDeathParticle()
{
	GameObject* gm_particle = scene().CreateGameObject<GameObject>();
	gm_particle->transform->position = transform().position;
	ParticleSystem* particle = gm_particle->AddComponent<ParticleSystem>();
	particle->deltaTime = false;
	particle->destroyWhenDie = true;
	particle->duration = .5f;
	particle->randomLifetime = true;
	particle->minLifetime = .5f;
	particle->maxLifetime = 1.4f;
	particle->radius = .3f;
	particle->speed = 10;
	particle->burst = true;
	particle->burstNumber = 1;
	particle->rateOverTime = 20;
	particle->burstCount = 20;
	particle->size = .1f;
	particle->burstTime = 0.f;
}

void PlayerController::Heal(int amount)
{
	_health->Heal(amount);
}

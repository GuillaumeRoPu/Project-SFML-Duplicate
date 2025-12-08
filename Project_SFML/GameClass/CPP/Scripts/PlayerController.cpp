#include <Scripts/PlayerController.h>
#include "Scripts/BulletController.h"
#include "Scripts/EnemyController.h"
#include "Objects/Blocker.h"

void PlayerController::Awake()
{
	_rb = owner->GetComponent<Rigidbody2D>();
	_rb->damping = 1;
	_health = owner->GetComponent<HealthComponent>();
	_tDashTimer = _tDashCooldown;
}

void PlayerController::Update()
{
	Blocker* blocker = scene().FindFirstObjectByType<Blocker>();
	blocker->GetComponent<PlayerHealthBlocker>()->SetHP(_health->GetHealth().GetRatio());

	_tDashTimer += Time::DeltaTime();
	if (_isDashing && _tDashTimer > _tDashDuration) {
		_isDashing = false;
		_rb->AddForce(Vector2(-_dashDir.x() * _dashForce, -_dashDir.y() * -_dashForce), ForceMode2D::Impulse);
		_rb->velocity -= Vector2(_dashDir.x() * -_acceleration, _dashDir.y() * _acceleration);
	}
}

void PlayerController::OnCollisionEnter2D(Collider2D* other)
{
	if (other->owner->GetComponent<EnemyController>())
	{
		//std::cout << "ENEMY" << std::endl;
		//AudioManager::PlaySound(AudioManager::collision);
	}
}

void PlayerController::Move(int verDir, int horDir)

void PlayerController::PlayerInputTreatment(InputSystem& input)
{
	float x = 0.0f;
	float y = 0.0f;
	if (input.getButton().up) {
		y += 1;
	}
	if (input.getButton().down) {
		y -= 1;
	}
	if (input.getButton().right) {
		x += 1;
	}
	if (input.getButton().left) {
		x -= 1;
	}
	Vector2 dir = { x,y };
	dir.normalize();
	
	Move(dir);

	if (input.getButton().lshift) {
		Dash(dir);
	};
}

void PlayerController::Move(Vector2 dir)
{
	if (dir.x() != 0 || dir.y() != 0) {
		_rb->velocity -= Vector2(dir.x() * -_acceleration * Time::DeltaTime(), dir.y() * _acceleration * Time::DeltaTime());
	}
}

void PlayerController::Dash(Vector2 dir)
{
	if (_tDashTimer > _tDashCooldown) {
		_tDashTimer = 0;
		_isDashing = true;
		_dashDir = dir;
		_rb->velocity = Vector2(0.0f, 0.0f);
		_rb->AddForce(Vector2(_dashDir.x() * _dashForce, _dashDir.y() * -_dashForce), ForceMode2D::Impulse);
	}
}

void PlayerController::BounceOnBorder(float borderX, float borderY, int playerSize)
{
	if (transform().position.x() * pixelPerPoint - playerSize < -borderX / 2) {
		if (_rb->velocity.x() <= 0)
			_rb->velocity = Vector2(-_rb->velocity.x(), _rb->velocity.y());
	}
	if (transform().position.x() * pixelPerPoint + playerSize > borderX / 2) {
		if (_rb->velocity.x() >= 0)
			_rb->velocity = Vector2(-_rb->velocity.x(), _rb->velocity.y());
	}
	if (transform().position.y() * pixelPerPoint - playerSize < -borderY / 2) {
		if (_rb->velocity.y() <= 0)
			_rb->velocity = Vector2(_rb->velocity.x(), -_rb->velocity.y());
	}
	if (transform().position.y() * pixelPerPoint + playerSize > borderY / 2) {
		if (_rb->velocity.y() >= 0)
			_rb->velocity = Vector2(_rb->velocity.x(), -_rb->velocity.y());
	}
}

void PlayerController::TakeDamage(int amount)
{
	AudioManager::PlaySound(AudioManager::touch);
	_health->TakeDamage(amount);
	if (_health->IsDead())
	{
		Time::timeScale = 0.f;
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

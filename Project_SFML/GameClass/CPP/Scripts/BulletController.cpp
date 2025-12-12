#include "Scripts/BulletController.h"
#include "Objects/Player.h"
#include "Scripts/PlayerController.h"
#include "Scripts/PlayerHealthBlocker.h"
#include "Components/Physics/CircleCollider2D.h"
#include "Components/Physics/Rigidbody2D.h"


void BulletController::Awake() 
{
	_rb = owner->GetComponent<Rigidbody2D>();
	_rb->damping = 2;
	_collider = owner->GetComponent<CircleCollider2D>();
}

void BulletController::Start() 
{
	_player = owner->scene->FindFirstObjectByType<Player>();

	//direction = Vector2::Edge(transform().position, _player->transform->position);

	if (_predictsPlayerMovement) 
	{
		if (!TryGetInterceptDirection(transform().position, _player->transform->position, _player->_rb->velocity, _bulletSpeed))
		{
			direction = Vector2::Edge(transform().position, _player->transform->position).normalized();
			if (_spread != 0)
			{
				float theta = Mathf::atan2(direction.y(), direction.x());
				float offset = Random::Range((float)(-1), (float)1) * _spread;
				float angle = theta + offset;
				direction = Vector2(Mathf::cos(angle), Mathf::sin(angle));
			}
		}
	}
	else 
	{
		direction = Vector2::Edge(transform().position, _player->transform->position).normalized();
		if (_spread != 0)
		{
			float theta = Mathf::atan2(direction.y(), direction.x());
			float offset = Random::Range((float)(-1), (float)1) * _spread;
			float angle = theta + offset;
			direction = Vector2(Mathf::cos(angle), Mathf::sin(angle));
		}
	}

	
	_rb->velocity = direction * _bulletSpeed;
	_rb->damping = 0;
	timer = 0;
}

void BulletController::Update() 
{

}

void BulletController::FixedUpdate() 
{
	timer += Time::DeltaTime();
	Vector2 magnitudeWithPlayer = Vector2::Edge(transform().position, _player->transform->position);
	if (_isTracking && magnitudeWithPlayer.magnitude() >= _maxDistance)
	{
		direction = magnitudeWithPlayer.normalized();
		_rb->velocity = direction * (_rb->velocity.magnitude() + _acceleration * Time::FixedDeltaTime());
	}
	else if (_isTracking)
	{
		_isTracking = false;
	}
	if(!_isTracking)
	{
		_rb->velocity += direction * _acceleration * Time::FixedDeltaTime();
	}
}

void BulletController::OnTriggerEnter2D(Collider2D* other) 
{
	PlayerHealthBlocker* blocker = other->owner->GetComponent<PlayerHealthBlocker>();
	PlayerController* p = other->owner->GetComponent<PlayerController>();
	if (blocker || p)
	{
		controller->DestroyBullet(owner);
		if (p && timer >= 0.05f)
			p->TakeDamage(_damage);
	}
}

void BulletController::OnTriggerExit2D(Collider2D* other)
{
	if (other->owner->name == "Limit")
	{
		controller->DestroyBullet(owner);
	}
}

void BulletController::OnCollisionEnter2D(Collider2D* other) 
{

}

void BulletController::BulletCanTrack(float maxDistance) 
{
	_isTracking = true;
	_maxDistance = maxDistance;
}

void BulletController::PredictsPlayerMovement() 
{
	_predictsPlayerMovement = true;
}

bool BulletController::TryGetInterceptDirection(Vector2 shooterPos, Vector2 targetPos, Vector2 targetVel, float projectileSpeed)
{
	direction = Vector2(0, 0);

	Vector2 r = targetPos - shooterPos;
	float vv = Vector2::Dot(targetVel, targetVel);
	float ss = projectileSpeed * projectileSpeed;
	float a = vv - ss;
	float b = 2 * Vector2::Dot(r, targetVel);
	float c = Vector2::Dot(r, r);

	const float EPS = 1e-6f;
	float t;

	if (abs(a) < EPS)
	{
		if (abs(b) < EPS)
			return false;

		t = -c / b;
		if (t <= 0) return false;
	}
	else
	{
		float disc = b * b - 4 * a * c;
		if (disc < 0) return false;

		float sqrtDisc = sqrtf(disc);
		float t1 = (-b - sqrtDisc) / (2 * a);
		float t2 = (-b + sqrtDisc) / (2 * a);

		bool t1OK = t1 > 0;
		bool t2OK = t2 > 0;

		if (t1OK && t2OK)
			t = Mathf::Min(t1, t2);
		else
			t = t1OK ? t1 : (t2OK ? t2 : -1);

		if (t <= 0) return false;
	}

	Vector2 aimPoint = targetPos + targetVel * t;
	Vector2 dir = aimPoint - shooterPos;
	if (dir.sqrMagnitude() < EPS) return false;

	direction = dir.normalized();
	return true;
}

void BulletController::SetBulletSpeed(float bulletSpeed) 
{
	_bulletSpeed = bulletSpeed;
}

void BulletController::SetColor(sf::Color color) 
{
	_color = color;
}

void BulletController::SetDamage(int damage) 
{
	_damage = damage;
}

void BulletController::SetScale(float scale) 
{
	transform().setLocalScale(Vector2(scale,scale));
}

void BulletController::SetAcceleration(float accel) 
{
	_acceleration = accel;
}

void BulletController::SetSpread(float spread) 
{
	_spread = spread;
}


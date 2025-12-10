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

	direction = Vector2::Edge(transform().position, _player->transform->position);
	_rb->velocity = direction.normalized() * _bulletSpeed;
	_rb->damping = 0;
	timer = 0;
}

void BulletController::Update() 
{
	timer += Time::DeltaTime();
}

void BulletController::OnTriggerEnter2D(Collider2D* other) 
{
	PlayerHealthBlocker* blocker = other->owner->GetComponent<PlayerHealthBlocker>();
	PlayerController* p = other->owner->GetComponent<PlayerController>();
	if (blocker || p)
	{
		controller->DestroyBullet(owner);
		if (p && timer >= 0.05f)
			p->TakeDamage(1);
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

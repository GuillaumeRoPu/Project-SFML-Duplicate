#include "Scripts/BulletController.h"
#include "Objects/Player.h"
#include "Scripts/PlayerController.h"
#include "Components/Physics/CircleCollider2D.h"
#include "Components/Physics/Rigidbody2D.h"

void BulletController::FixedUpdate() 
{
	if (_player != nullptr)
	{
		Vector2 VecteurPlayerEnemy = Vector2(_player->transform->position.x() - transform().position.x(), _player->transform->position.y() - transform().position.y());
		float magnitude = Mathf::sqrt(VecteurPlayerEnemy.x() * VecteurPlayerEnemy.x() + VecteurPlayerEnemy.y() * VecteurPlayerEnemy.y());
		if (magnitude >= _player->transform->scale.x()) 
		{
			_rb->velocity += VecteurPlayerEnemy;
		}
	}
}

void BulletController::Awake() 
{
	_rb = owner->GetComponent<Rigidbody2D>();
	_collider = owner->GetComponent<CircleCollider2D>();
}

void BulletController::OnTriggerEnter2D(Collider2D* other) 
{
	PlayerController* p = other->owner->GetComponent<PlayerController>();
	if (p != nullptr) {
	}
}

void BulletController::OnCollisionEnter2D(Collider2D* other) 
{

}

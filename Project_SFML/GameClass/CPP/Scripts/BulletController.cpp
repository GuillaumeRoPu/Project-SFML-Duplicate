#include "Scripts/BulletController.h"

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
	else 
	{
		std::cout << "bah jsp quoi te dire frr" << std::endl;
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
		std::cout << "TOUCHE" << std::endl;
	}
}

void BulletController::GetPlayer(Player* player) {
	_player = player;
	if (_player != nullptr) {
		std::cout << "TROUVE" << std::endl;
	}
}
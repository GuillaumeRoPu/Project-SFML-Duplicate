#include "Scripts/EnemyController.h"

void EnemyController::FixedUpdate()
{
	//_rb->AddForce(Vector2(1, 1));
	//if (_rb->transform().position.x() > 1000 || _rb->transform().position.x() < -1000)
	//{
		//_rb->transform().position = Vector2(0, 0);
	//}
	if (_player != nullptr)
	{
		Vector2 VecteurPlayerEnemy = Vector2(_player->transform->position.x() - transform().position.x(), _player->transform->position.y() - transform().position.y());
		float magnitude = Mathf::sqrt(VecteurPlayerEnemy.x() * VecteurPlayerEnemy.x() + VecteurPlayerEnemy.y() * VecteurPlayerEnemy.y());
		if (magnitude >= _player->transform->scale.x()) {
			_rb->velocity += VecteurPlayerEnemy;
		}
	}
}

void EnemyController::Update() 
{

}

void EnemyController::Start() 
{
	_rb = owner->GetComponent<Rigidbody2D>();
	_collider = owner->GetComponent<BoxCollider2D>();
}

void EnemyController::GetPlayer(Scene* scene) {
	_player = scene->FindFirstObjectByType<Player>();
}


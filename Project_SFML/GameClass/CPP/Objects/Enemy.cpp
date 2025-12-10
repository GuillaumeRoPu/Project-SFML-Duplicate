#include "Objects/Enemy.h"

Enemy::Enemy()
{
	transform->scale = { 0.4f, 0.4f };

	_rb = AddComponent<Rigidbody2D>();
	_collider = AddComponent<BoxCollider2D>();
	_collider->isTrigger = true;
	_collider->includeLayers += LayerManager::GetLayerByName("Player").value;
	_controller = AddComponent<EnemyController>();
	_health = AddComponent<HealthComponent>();
	_health->SetMaxHealth(5, true);

	name = "Enemy";
	layer = LayerManager::GetLayerByName("Enemy");
}
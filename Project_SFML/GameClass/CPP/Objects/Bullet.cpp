#include "Objects/Bullet.h"
#include "Manager\LayerManager.h"

Bullet::Bullet() 
{
	transform->scale = { 0.4f, 0.4f };

	_rb = AddComponent<Rigidbody2D>();
	_collider = AddComponent<CircleCollider2D>();
	_collider->includeLayers += LayerManager::GetLayerByName("Player").value;
	_collider->includeLayers += LayerManager::GetLayerByName("Limit").value;

	_controller = AddComponent<BulletController>();
	name = "Bullet";
	_collider->isTrigger = true;

	layer = LayerManager::GetLayerByName("Bullet");
}
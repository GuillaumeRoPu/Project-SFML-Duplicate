#include "Objects/Blocker.h"

Blocker::Blocker()
{
	transform->scale = { 1.f, 0.2f };

	_bc = AddComponent<BoxCollider2D>();
	_phc = AddComponent<PlayerHealthBlocker>();

	layer = LayerManager::GetLayerByName("Player");
}

#include "Objects/Blocker.h"

Blocker::Blocker()
{
	_bc = AddComponent<BoxCollider2D>();
	_phc = AddComponent<PlayerHealthBlocker>();
}

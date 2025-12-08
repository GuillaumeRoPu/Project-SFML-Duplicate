#include "Scripts/PlayerHealthBlocker.h"
#include <Components/Physics/BoxCollider2D.h>
#include "Objects/GameObject.h"

void PlayerHealthBlocker::BlockerInputTreatment(InputSystem& input)
{
	int rotation = 0;
	if (input.getButton().rotateRight) {
		rotation += 1;
	}
	if (input.getButton().rotateLeft) {
		rotation -= 1;
	}
	Rotate(rotation);
}

void PlayerHealthBlocker::Rotate(int dir)
{
	angle += dir * _rotationSpeed * Time::DeltaTime();

	if (angle < 0) {
		angle += 360;
	}
	else if (angle > 360) {
		angle -= 360;
	}
}

void PlayerHealthBlocker::SetHP(float ratio)
{
	_healthSize = transform().scale.x() * ratio;
}

#include "Scripts/PlayerHealthBlocker.h"
#include <Components/Physics/BoxCollider2D.h>
#include "Objects/GameObject.h"
#include "InputSystem.h"

void PlayerHealthBlocker::Rotate(bool dir)
{
	if (dir) {
		angle += rotationSpeed * Time::DeltaTime();
	}
	else if (!dir) {
		angle -= rotationSpeed * Time::DeltaTime();
	}
	if (angle < 0) {
		angle = 360 + angle;
	}
	angle = angle % 360;
	rad = angle * (2 * Mathf::PI / 360);
	//std::cout << angle << " " << rad << std::endl;
}

void PlayerHealthBlocker::SetHP(float ratio)
{
	_healthSize = transform().scale.x() * ratio;
}

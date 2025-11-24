#include "Components/Camera.h"
#include "Object/GameObject.h"

void Camera::SetCameraView(sf::RenderWindow& window)
{
	window.setView({ {transform().position.x() * pixelPerPoint, transform().position.y() * pixelPerPoint}, {viewport.x(), viewport.y()}});
}

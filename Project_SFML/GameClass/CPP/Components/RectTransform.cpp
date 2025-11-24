#include "Components\RectTransform.h"
#include "Object\GameObject.h"

void RectTransform::SetParent(RectTransform* newParent, bool keepTransform)
{
	parent = newParent;

	if (parent == nullptr) return;

	if (keepTransform)
	{
		_localRectPosition = rectPosition - parent->rectPosition;

	}
	else
	{
		_localRectPosition = rectPosition;

		rectPosition = parent->rectPosition + _localRectPosition;
	}
}

void RectTransform::ApplyRectTransformToParent(sf::RenderWindow& window)
{
	if (!parent) return;

	rectPosition = parent->rectPosition + _localRectPosition;

	sf::Vector2f pos = window.mapPixelToCoords({ (int)rectPosition.x(), (int)rectPosition.y() });
	position = Vector2(pos.x, pos.y);
}

void RectTransform::setLocalRectPosition(Vector2 newPos)
{
	_localRectPosition = newPos;
	if (parent)
		rectPosition = parent->rectPosition + _localRectPosition;
}

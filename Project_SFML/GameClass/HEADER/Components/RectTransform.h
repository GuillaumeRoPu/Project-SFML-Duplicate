#pragma once
#include "Components/Transform.h"
#include <SFML\Graphics\RenderWindow.hpp>

class RectTransform : public Transform
{

public:
	Vector2 rectPosition = Vector2(0, 0);

	RectTransform* parent = nullptr;

	void SetParent(RectTransform* newParent, bool keepTransform);

	void ApplyRectTransformToParent(sf::RenderWindow& window);

	Vector2 localRectPosition() { return _localRectPosition; }

	void setLocalRectPosition(Vector2 newLocalPosition);

private:
	Vector2 _localRectPosition = Vector2(0, 0);
};
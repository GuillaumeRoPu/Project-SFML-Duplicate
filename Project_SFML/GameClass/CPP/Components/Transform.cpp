#include "Components\Transform.h"
#include "Objects\GameObject.h"

void Transform::SetParent(Transform* newParent, bool keepTransform)
{
	parent = newParent;

	if (parent == nullptr) return;

	if (keepTransform)
	{
		_localPosition = position - parent->position;
		_localScale = scale / parent->scale;

	}
	else
	{
		_localPosition = position;
		_localScale = scale;

		position = parent->position + _localPosition;
		scale = parent->scale * _localScale;
	}
}

void Transform::ApplyTransformToParent()
{
	if (!parent) return;

	position = parent->position + _localPosition;
	scale = parent->scale * _localScale;
}

void Transform::setLocalPosition(Vector2 newPos)
{
	_localPosition = newPos;
	if (parent)
		position = parent->position + _localPosition;
}

void Transform::setLocalScale(Vector2 newScale)
{
	_localScale = newScale;
	if (parent)
		scale = parent->scale * _localScale;
}

#pragma once
#include "Components/Component.h"

using namespace fry;

class Transform : public Component
{
public:
	Vector2 position = Vector2(0, 0);
	Vector2 scale = Vector2(1, 1);
	float rotation = 0;


	Transform* parent = nullptr;

	void Translate(Vector2 translation)
	{
		position += translation;
	}

	void SetParent(Transform* newParent, bool keepTransform);

	void ApplyTransformToParent();

	Vector2 localPosition(){ return _localPosition; }
	Vector2 localScale(){ return _localScale; }

	void setLocalPosition(Vector2 newLocalPosition);
	void setLocalScale(Vector2 newLocalScale);

private:
	Vector2 _localPosition = Vector2(0, 0);
	Vector2 _localScale = Vector2(1, 1);
};
#pragma once
#include <SFML/Graphics.hpp>
#include "Objects/GameObject.h"
#include "Engine/FryEngine.h"
#include "Scripts/PlayerHealthBlocker.h"
#include <Components/Physics/BoxCollider2D.h>

class Blocker : public GameObject
{
public:
	Blocker();
	void Transformation(sf::Vector2f s, Vector2 pos, float rot);
public:
	//BoxCollider2D* _bc;
	PlayerHealthBlocker* _phc;
	sf::Vector2f size;
	Vector2 position;
	float rotation;
};
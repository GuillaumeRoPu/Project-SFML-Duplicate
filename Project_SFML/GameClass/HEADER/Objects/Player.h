#pragma once
#include <SFML/Graphics.hpp>
#include "Objects/GameObject.h"
#include "Engine/FryEngine.h"
#include "Scripts/PlayerController.h"
#include <Components/Physics/CircleCollider2D.h>

class Player : public GameObject
{
public:
	Player();
public:
	Rigidbody2D* _rb;
	CircleCollider2D* _cc;
	PlayerController* _pc;

};
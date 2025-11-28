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
public:
	BoxCollider2D* _bc;
	PlayerHealthBlocker* _phc;
};
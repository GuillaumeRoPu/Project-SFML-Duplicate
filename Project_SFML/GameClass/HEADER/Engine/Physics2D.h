#pragma once
#include <SFML/Graphics.hpp>
#include "Engine\FryEngine.h"

using namespace fry;


struct RaycastHit
{
    bool hit = false;
    Vector2 point;
    Vector2 normal;
    float distance = 0.f;
};

inline float gravityForce = 9.81f;

enum class ForceMode2D
{
    Force, Impulse
};

class Physics2D
{
public:
    static RaycastHit RaycastAABB(Vector2 start, Vector2 end, Rect box);
};

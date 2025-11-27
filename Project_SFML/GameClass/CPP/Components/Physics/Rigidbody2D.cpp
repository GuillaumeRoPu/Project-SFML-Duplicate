#include <Components/Physics/Rigidbody2D.h>
#include "Objects/GameObject.h"
#include "Engine/FryEngine.h"

void Rigidbody2D::Awake()
{
    if (owner->GetComponent<Collider2D>() == nullptr)
        owner->AddComponent<Collider2D>();

    Vector2 size = transform().scale;
    mass = autoMass ? size.x() * size.y() : mass;
}

void Rigidbody2D::FixedUpdate() 
{

    if (gravity)
        AddForce(Vector2(0.f, gravityForce * mass));

    (*owner).transform.position += (appliedForce + velocity) * Time::fixedDeltaTime;

    velocity -= velocity * damping * Time::DeltaTime();

    if (Mathf::abs(velocity.x()) < 0.01f) velocity = { 0.f, velocity.y() };
    if (Mathf::abs(velocity.y()) < 0.01f) velocity = { velocity.x(), 0.f };

    appliedForce = Vector2(0, 0);
}

void Rigidbody2D::AddForce(Vector2 force, ForceMode2D type)
{
    if (mass <= 0) return;

    switch (type)
    {
    case ForceMode2D::Force:
        appliedForce += force / mass * Time::DeltaTime();
        break;

    case ForceMode2D::Impulse:
        appliedForce += force / mass;
        break;
    }}

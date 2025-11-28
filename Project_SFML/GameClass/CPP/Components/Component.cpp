#include "Components/Component.h"
#include "Components/Transform.h"
#include "Objects/GameObject.h"
#include "Scene.h"

Transform& Component::transform()
{
    return *(owner->transform);
}

Scene& Component::scene()
{
    return *(owner->scene);
}

// Collision methods
void Component::OnCollisionEnter2D(Collider2D* other)
{
}

void Component::OnCollisionStay2D(Collider2D* other)
{
}

void Component::OnCollisionExit2D(Collider2D* other)
{
}

// Trigger methods
void Component::OnTriggerEnter2D(Collider2D* other)
{
}

void Component::OnTriggerStay2D(Collider2D* other)
{
}

void Component::OnTriggerExit2D(Collider2D* other)
{
}

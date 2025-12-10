#include "Objects\GameObject.h"
#include "Scene.h"

void GameObject::InitID()
{
    if (_ID == -1)
    {
        _ID = scene->ObjectCount();
    }
}

void GameObject::Awake()
{
    if (!active) return;

    for (auto& comp : components)
        comp->Awake();
}

void GameObject::Start()
{
    if (!active || _start) return;

    _start = true;
    for (auto& comp : components)
        comp->Start();
}

void GameObject::Update()
{
    if (!active) return;

    for (auto& comp : components)
        comp->Update();
}

void GameObject::FixedUpdate()
{
    if (!active) return;

    for (auto& comp : components)
        comp->FixedUpdate();
}

void GameObject::OnCollisionEnter2D(Collider2D* other)
{
    if (!active) return;

    for (auto& comp : components)
        comp->OnCollisionEnter2D(other);
}

void GameObject::OnCollisionStay2D(Collider2D* other)
{
    if (!active) return;

    for (auto& comp : components)
        comp->OnCollisionStay2D(other);
}

void GameObject::OnCollisionExit2D(Collider2D* other)
{
    if (!active) return;

    for (auto& comp : components)
        comp->OnCollisionExit2D(other);
}

void GameObject::OnTriggerEnter2D(Collider2D* other)
{
    if (!active) return;

    for (auto& comp : components)
        comp->OnTriggerEnter2D(other);
}

void GameObject::OnTriggerStay2D(Collider2D* other)
{
    if (!active) return;

    for (auto& comp : components)
        comp->OnTriggerStay2D(other);
}

void GameObject::OnTriggerExit2D(Collider2D* other)
{
    if (!active) return;

    for (auto& comp : components)
        comp->OnTriggerExit2D(other);
}
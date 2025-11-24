#pragma once
#include "Components/Component.h"

#pragma region COMPONENTS INCLUDE
#include "Components/Transform.h"
#include "Components/Physics/Rigidbody2D.h"
#include "Components/Physics/Collider2D.h"
#include "Components/Physics/BoxCollider2D.h"
#include "Components/Physics/CircleCollider2D.h"
#include "Components/Physics/PolygonCollider2D.h"
#include "Components/ParticleSystem.h"
#include "Components/SpriteRenderer.h"
#include "Components/Camera.h"
#include "Components/Scripting.h"

#pragma endregion

#include <iostream>
#include <vector>

class GameObject
{
protected:

    std::vector<std::unique_ptr<Component>> components;
    int _hashID;


public:
    bool active = true;

    std::string name = "GameObject";

    // Tag of the game object
    std::string tag = "Untagged";

    Transform& transform;

    GameObject() : transform(*AddComponent<Transform>()) {}
    GameObject(std::string name) : transform(*AddComponent<Transform>()) { this->name = name; }

    void Awake()
    {
        if (!active) return;
        
        for (auto& comp : components)
            comp->Awake();
    }

    void Start()
    {
        if (!active) return;

        for (auto& comp : components)
            comp->Start();
    }

    void Update()
    {
        if (!active) return;

        for (auto& comp : components)
            comp->Update();
    }

    void FixedUpdate()
    {
        if (!active) return;

        for (auto& comp : components)
            comp->FixedUpdate();
    }


    virtual ~GameObject() = default;

    // Add a component to your game object
    template <typename T, typename... Args>
    T* AddComponent(Args&&... args) {
        static_assert(std::is_base_of<Component, T>::value, "T must inherit Component");
        auto comp = std::make_unique<T>(std::forward<Args>(args)...);
        comp->owner = this;

        T* ptr = comp.get();
        components.push_back(std::move(comp));
        return ptr;
    }

    // Get a component to your game object
    template <typename T>
    T* GetComponent() {
        static_assert(std::is_base_of<Component, T>::value, "T must inherit Component");
        for (auto& comp : components) {
            if (auto casted = dynamic_cast<T*>(comp.get())) {
                return casted;
            }
        }
        return nullptr;
    }

    // Destroy a component to your gameObject
    void DestroyComponent(Component* T)
    {
        auto it = std::remove_if(components.begin(), components.end(),
            [&](const std::unique_ptr<Component>& comp) {
                return comp.get() == T;
            });

        if (it != components.end())
            components.erase(it, components.end());
    }

    // Collisions & Triggers
    void OnCollisionEnter2D(Collider2D* other)
    {
        if (!active) return;

        for (auto& comp : components)
            comp->OnCollisionEnter2D(other);
    }

    void OnCollisionStay2D(Collider2D* other)
    {
        if (!active) return;

        for (auto& comp : components)
            comp->OnCollisionStay2D(other);
    }

    void OnCollisionExit2D(Collider2D* other)
    {
        if (!active) return;

        for (auto& comp : components)
            comp->OnCollisionExit2D(other);
    }

    void OnTriggerEnter2D(Collider2D* other)
    {
        if (!active) return;

        for (auto& comp : components)
            comp->OnTriggerEnter2D(other);
    }

    void OnTriggerStay2D(Collider2D* other)
    {
        if (!active) return;

        for (auto& comp : components)
            comp->OnTriggerStay2D(other);
    }

    void OnTriggerExit2D(Collider2D* other)
    {
        if (!active) return;

        for (auto& comp : components)
            comp->OnTriggerExit2D(other);
    }

};
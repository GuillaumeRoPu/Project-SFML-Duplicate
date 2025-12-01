#pragma once
#include "Components/Component.h"

#include "Components/Transform.h"
#include "Components/Physics/Collider2D.h"

#include <vector>

class Scene;

class GameObject
{
protected:

    std::vector<std::unique_ptr<Component>> components;
    int _hashID;

private:
    bool _start = false;

public:
    bool active = true;

    std::string name = "GameObject";

    // Tag of the game object
    std::string tag = "Untagged";

    Scene* scene;

    Transform* transform;

    GameObject() : transform(AddComponent<Transform>()) {}
    GameObject(std::string name) : transform(AddComponent<Transform>()) { this->name = name; }

    bool HasStart() const { return _start; }

    void Awake();

    void Start();

    void Update();

    void FixedUpdate();


    virtual ~GameObject() = default;

    // Add a component to your game object
    template <typename T>
    T* AddComponent()
    {
        static_assert(std::is_base_of<Component, T>::value, "T must inherit Component");
        auto comp = std::make_unique<T>();
        comp->owner = this;

        T* ptr = comp.get();
        components.push_back(std::move(comp));
        return ptr;
    }

    // Get a component to your game object
    template <typename T>
    T* GetComponent()
    {
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
    void OnCollisionEnter2D(Collider2D* other);

    void OnCollisionStay2D(Collider2D* other);

    void OnCollisionExit2D(Collider2D* other);

    void OnTriggerEnter2D(Collider2D* other);

    void OnTriggerStay2D(Collider2D* other);

    void OnTriggerExit2D(Collider2D* other);

};
#pragma once
#include "Engine/FryEngine.h"
#include "Engine/Mathf.h"
#include "Engine/Time.h"
#include "Engine/Physics2D.h"
#include <SFML/Graphics.hpp>

class GameObject;
class Scene;
class Transform;
class Collider2D;

class Component
{
public:
	virtual ~Component() = default;

	virtual void Awake() {}
	virtual void Start() {}
	virtual void Update() {}
	virtual void FixedUpdate() {}

	GameObject* owner = nullptr;

	Transform& transform();
	Scene& scene();

	virtual void OnCollisionEnter2D(Collider2D* other);
	virtual void OnCollisionStay2D(Collider2D* other);
	virtual void OnCollisionExit2D(Collider2D* other);

	virtual void OnTriggerEnter2D(Collider2D* other);
	virtual void OnTriggerStay2D(Collider2D* other);
	virtual void OnTriggerExit2D(Collider2D* other);
};
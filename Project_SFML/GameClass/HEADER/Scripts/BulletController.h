#pragma once
#include "Scene.h"
#include "Objects/Player.h"
#include "Components/Scripting.h"
#include "Scripts/BulletManagerController.h"

class Scene;
class Rigidbody2D;
class CircleCollider2D;
class Collider2D;
class BulletManagerController;

class BulletController : public Scripting
{
	void Awake() override;
	void Start() override;
	void Update() override;
	void FixedUpdate() override;
	void OnTriggerEnter2D(Collider2D* other) override;
	void OnTriggerExit2D(Collider2D* other) override;
	void OnCollisionEnter2D(Collider2D* other) override;
	bool TryGetInterceptDirection(Vector2 shooterPos, Vector2 targetPos, Vector2 targetVel, float projectileSpeed);

public:
	void BulletCanTrack(float maxDistance);
	void PredictsPlayerMovement();
	void SetBulletSpeed(float bulletSpeed);
	void SetColor(sf::Color color);
	void SetDamage(int damage);
	void SetScale(float scale);
	void SetAcceleration(float accel);
	void SetSpread(float spread);

public:
	BulletManagerController* controller;
	Rigidbody2D* _rb;
	CircleCollider2D* _collider;
	Player* _player;
	sf::Color _color = sf::Color::Yellow;

private:
	float _bulletSpeed = 1;
	Vector2 direction;
	float timer;
	bool _isTracking = false;
	bool _predictsPlayerMovement = false;
	float _maxDistance = 3;
	int _damage = 1;
	float _acceleration = 0;
	float _totalAccel = 0;

	//Spread Values
	float _spread;
};
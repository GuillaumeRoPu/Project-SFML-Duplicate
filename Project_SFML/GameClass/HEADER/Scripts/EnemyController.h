#pragma once

#include "Components/Scripting.h"
#include "Components/Physics/BoxCollider2D.h"
#include "Scripts/HealthComponent.h"
#include "Objects/Player.h"
#include "Scene.h"
#include <list>

class Bullet;
class EnemyManagerController;

class EnemyController : public Scripting
{
    //void FixedUpdate() override;
    void Update() override;
    void Start() override;
    void OnTriggerEnter2D(Collider2D* other) override;
private:
    void SpawnDeathParticle();
    
public:
    void Setup(float damage, float cooldown, float bulletSpeed);
    void SpawnBullet();
    Bullet* InstantiateBullet();
    void BounceOnBorder(float borderX, float borderY);

public:

    EnemyManagerController* controller;
    Rigidbody2D* _rb;
    CircleCollider2D* _collider;
    HealthComponent* _health;
    Player* _player;
    Bullet* _bullet;
    Scene* _scene;
    float cooldown;
    float timer;
    int baseHealth = 70; // player go at 70 speed at almost maxCap
};


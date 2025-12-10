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
    void SetupBullet(float bulletSpeed, float bulletAcceleration, float scale, int damage, sf::Color color, bool canPredictPlayerPosition, bool canTrack, float maxTrackingDistance);
    void SpawnBullet();
    Bullet* InstantiateBullet();

public:

    EnemyManagerController* controller;
    Rigidbody2D* _rb;
    BoxCollider2D* _collider;
    HealthComponent* _health;
    Player* _player;
    Bullet* _bullet;
    Scene* _scene;
    float cooldown;
    float timer;

private:
    //Bullet Data
    float _bulletScale;
    float _bulletSpeed;
    float _bulletAcceleration;
    int _bulletDamage;
    sf::Color _bulletColor;
    bool _bulletCanPredictPlayerPos;
    bool _bulletCanTrack;
    float _bulletmaxTrackDistance;
};


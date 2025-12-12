#pragma once

#include "Components/Scripting.h"
#include "Components/Physics/BoxCollider2D.h"
#include "Scripts/HealthComponent.h"
#include "Objects/Player.h"
#include "Scene.h"
#include <list>

class Bullet;
class EnemyManagerController;
enum class EnemyType;

class EnemyController : public Scripting
{
    //void FixedUpdate() override;
    void Update() override;
    void Start() override;
    void OnTriggerEnter2D(Collider2D* other) override;
private:
    void SpawnDeathParticle();
    
public:
    void SetupBulletProperties(float bulletSpeed, float bulletAcceleration, float scale, int damage, sf::Color color, bool canPredictPlayerPosition, bool canTrack, float maxTrackingDistance);
    void SetupEnemyProperties(int health, float scale, float cooldown, sf::Color color, EnemyType type);
    void SpawnBullet();
    Bullet* InstantiateBullet();
    void BounceOnBorder(float borderX, float borderY);
    void SetupBurstProperties(float delayBetweenShots, int numberOfShots);
    void SetupShotgunProperties(float spread, int numberOfShots);

public:

    EnemyManagerController* controller;
    Rigidbody2D* _rb;
    CircleCollider2D* _collider;
    HealthComponent* _health;
    Player* _player;
    Bullet* _bullet;
    Scene* _scene;
    sf::Color color;

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

    //Enemy Data
    int _basehealth; // player go at 70 speed at almost maxCap
    float _cooldown;
    float _cooldownTimer;

    EnemyType _type;

    //Burst Type Data
    float _littleCooldown;
    float _littleCooldownTimer;
    int _numberofBulletsInString;
    int _currentNumberShot;

    //Shotgun Type Data
    int _numberOfBullets;
    float _spread;
};

enum class EnemyType
{
    Classic,
    Burst,
    Shotgun,
};

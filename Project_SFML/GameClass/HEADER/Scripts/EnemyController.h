#pragma once
#include "Objects/GameObject.h"
#include "Objects/Bullet.h"
#include "Components/Scripting.h"
#include "Components/Physics/BoxCollider2D.h"
#include "Objects/Player.h"
#include "Scene.h"
#include <list>

class Bullet;

class EnemyController : public Scripting
{
    //void FixedUpdate() override;
    void Update() override;
    void Start() override;
    void OnTriggerEnter2D(Collider2D* other) override;
public:
    void GetPlayer(Player* player);
    void GetScene(Scene* scene);
    void SpawnBullet();
    Bullet* InstantiateBullet();

public:
    Rigidbody2D* _rb;
    BoxCollider2D* _collider;
    Player* _player;
    std::list<Bullet*> _bullets;
    Bullet* _bullet;
    Scene* _scene;
    float cooldown;
    float timer;
};


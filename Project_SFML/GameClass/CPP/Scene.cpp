#include "Scene.h"
#include "Engine/Time.h"

Scene::Scene()
{
    CreateGameObject<GameObject>("MainCamera")->tag = "MainCamera";
    FindFirstObjectWithTag("MainCamera")->AddComponent<Camera>();
}

void Scene::PlayScene(sf::RenderWindow& window, InputSystem& input)
{
    static sf::Clock fixedClock;
    static float accumulator = 0.0f;


    if (!start)
    {
        for (auto& obj : SceneObjects)
            obj->Start();
        
        start = true;
    }


    Time::Update(); // met à jour deltaTime et FPS
    float frameTime = Time::DeltaTime();
    accumulator += frameTime;

    // Fixed Update : Physics
    while (accumulator >= Time::fixedDeltaTime)
    {
        for (auto& obj : SceneObjects)
            obj->FixedUpdate();

        accumulator -= Time::fixedDeltaTime;
    }

    //// Crée la collision entre un game object ayant un rigidbody et tout les colliders de la scène
    //// Dessine tout les game object ayant un sprite renderer
    //// Joue tout les particules de la scène
    // Update

    for (auto& obj : SceneObjects)
    {
        if (!obj.get()->active) continue;

        // Update : Logic
        obj->Update();

        auto& transform = obj.get()->transform;

        //transform.ApplyTransformToParent();

        // Rigidbody
        auto rb = obj.get()->GetComponent<Rigidbody2D>();
        if (rb != nullptr) // Regarde si l'objet à un rigidbody
        {
            auto* target = obj.get()->GetComponent<Collider2D>();

            for (auto& coll : SceneObjects) // Boucle sur tout les autres objets
            {

                if (obj == coll || !coll.get()->active)
                    continue;

                auto* collider = coll.get()->GetComponent<Collider2D>();

                if (collider != nullptr) // Regarder s'il a un collider
                {
                    target->UpdateCollision(coll); // Créé la collision
                }
            }
        }

        // Particles
        auto p = obj.get()->GetComponent<ParticleSystem>();
        if (p != nullptr)
        {
            p->PlayParticleSystem();
            p->UpdateParticleSystem();
        }
    }

    // Late Update
    for (auto& obj : SceneObjects)
    {
        auto& transform = obj.get()->transform;

        transform.ApplyTransformToParent();

        // Camera
        auto cam = obj.get()->GetComponent<Camera>();
        if (cam)
        {
            cam->SetCameraView(window);
        }
    }

    return;
}
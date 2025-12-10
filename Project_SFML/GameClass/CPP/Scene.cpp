#include "Scene.h"
#include "Components\Camera.h"
#include "Components\ParticleSystem.h"
#include "Components\SpriteRenderer.h"
#include "Components/Physics/Rigidbody2D.h"
#include "Components/Physics/Collider2D.h"
#include "Components\UI\Text.h"
#include "Engine/Time.h"

Scene::Scene(std::string n)
{
    name = n;
    MainCamera = CreateGameObject<GameObject>();
    MainCamera->name = "MainCamera";
    MainCamera->tag = "MainCamera";
    MainCamera->AddComponent<Camera>();
}

void Scene::PlayScene(sf::RenderWindow& window, InputSystem& input)
{
    static float accumulator = 0.0f;

    // Start
    for (auto& obj : SceneObjects)
        obj->Start();


    Time::Update(); // met à jour deltaTime et FPS
    float frameTime = Time::DeltaTime();
    accumulator += frameTime;

    // Fixed Update : Physics
    while (accumulator >= Time::FixedDeltaTime())
    {
        int collisionCalcul = 0;
        int nbObject = 0;
        for (auto& obj : SceneObjects)
        {
            if (!obj.get()->active) continue;

            nbObject++;

            obj->FixedUpdate();

            auto& transform = *(obj.get()->transform);

            transform.ApplyTransformToParent();

            // Rigidbody
            auto rb = obj.get()->GetComponent<Rigidbody2D>();
            if (rb != nullptr) // Regarde si l'objet à un rigidbody
            {
                auto* target = obj.get()->GetComponent<Collider2D>();
                if (target)
                {

                    for (auto& coll : SceneObjects) // Boucle sur tout les autres objets
                    {

                        if (obj == coll || !coll.get()->active)
                            continue;

                        auto* collider = coll.get()->GetComponent<Collider2D>();

                        if (target->includeLayers.value != 0)
                        {
                            if ((target->includeLayers.value & coll.get()->layer.value) == 0)
                            {
                                continue;
                            }
                        }

                        target->ApplyRotation();

                        if (collider != nullptr) // Regarder s'il a un collider
                        {
                            collider->ApplyRotation();
                            target->UpdateCollision(coll.get()); // Créé la collision
                            collisionCalcul++;
                        }
                    }
                }
                
            }
        }

        std::cout << "Nb Collision : " << collisionCalcul << std::endl;
        std::cout << "Nb Objects : " << nbObject << std::endl;
        accumulator -= Time::UnscaledFixedDeltaTime();

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

        auto& transform = *(obj.get()->transform);

        transform.ApplyTransformToParent();

        // Camera
        auto cam = obj.get()->GetComponent<Camera>();
        if (cam)
        {
            window.clear(cam->backgroundColor);
            cam->SetCameraView(window);
        }

        // Particles
        auto p = obj.get()->GetComponent<ParticleSystem>();
        if (p != nullptr)
        {
            p->PlayParticleSystem();
            p->UpdateParticleSystem();
            p->DrawParticleSystem(window);
        }
    }

    // Late Update
    for (auto& obj : SceneObjects)
    {
        if (!obj.get()->active) continue;

        auto& transform = *(obj.get()->transform);

        transform.ApplyTransformToParent();

        //UI

        //Text
        auto txt = obj.get()->GetComponent<Text>();
        if (txt)
        {
            sf::Text fps;
            fps.setFont(txt->font);
            fps.setFillColor(txt->color);
            fps.setString(txt->text);
        }

        // Sprites
        auto renderer = obj.get()->GetComponent<SpriteRenderer>();
        if (renderer != nullptr)
        {
            sf::Texture tex(renderer->GetTexture());
            sf::Sprite sprite(tex);
            sprite.setOrigin({ (float)tex.getSize().x / 2, (float)tex.getSize().y / 2 });
            sprite.setPosition({ transform.position.x() * pixelPerPoint, transform.position.y() * pixelPerPoint });
            sprite.setScale({ transform.scale.x(), transform.scale.y() });
            sprite.scale({ pixelPerPoint / (float)tex.getSize().y , pixelPerPoint / (float)tex.getSize().y });
            sprite.setRotation(transform.rotation);
            sprite.setColor(renderer->color);
            window.draw(sprite);
        }
    }

    bool makeAwake = false;
    int nb_awake = 0;
    for (auto& obj : PendingAdd)
    {
        makeAwake = true;
        nb_awake++;
        SceneObjects.push_back(std::move(obj));
    }
    PendingAdd.clear();

    for (auto* ptr : PendingRemove)
    {
        auto it = std::find_if(SceneObjects.begin(), SceneObjects.end(),
            [&](auto& uptr) { return uptr.get() == ptr; });

        if (it != SceneObjects.end())
            SceneObjects.erase(it);
    }

    PendingRemove.clear();

    if (makeAwake)
    {
        int id = 0;
        for (auto& obj : SceneObjects)
        {
            if (id >= SceneObjects.size() - nb_awake)
            {
                obj.get()->scene = this;
                obj.get()->InitID();
                obj.get()->Awake();
            }

            id++;
        }
    }

    return;
}
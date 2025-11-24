#include "Game.h"

#include "Entity/Player.h"
#include "Entity/AI.h"

#include "Engine/FryEngine.h"

Game::Game()
{
    Player& player = *CreateGameObject<Player>(100.0f, 11.f, 13.f, 2.f, 2.f);
    AI& enemy = *CreateGameObject<AI>(20.0f, 0.f, 0.f, 1.f, 1.f, 1.0f);

    GameObject& wall = *CreateGameObject<GameObject>();
    wall.AddComponent<BoxCollider2D>();
    auto* wallRenderer = wall.AddComponent<SpriteRenderer>();
    wallRenderer->SetTexture("res/Engine/Sprites/square.png");
    wallRenderer->color = sf::Color::Magenta;
    wall.transform.scale = Vector2(3, 3);
    wall.transform.position = Vector2(10, 6);

    GameObject& wall2 = *CreateGameObject<GameObject>();
    wall2.AddComponent<CircleCollider2D>();
    auto* wallRenderer2 = wall2.AddComponent<SpriteRenderer>();
    wallRenderer2->SetTexture("res/Engine/Sprites/circle.png");
    wallRenderer2->color = sf::Color::Blue;
    wall2.transform.scale = Vector2(2.5f, 2.5f);
    wall2.transform.position = Vector2(3, 6);

    GameObject& wall3 = *CreateGameObject<GameObject>();
    wall3.AddComponent<PolygonCollider2D>();
    auto* wallRenderer3 = wall3.AddComponent<SpriteRenderer>();
    wallRenderer3->SetTexture("res/Engine/Sprites/shape0.png");
    wallRenderer3->color = sf::Color::Yellow;
    wall3.transform.scale = Vector2(4.f, 4.f);
    wall3.transform.position = Vector2(7.f, 11.5f);
    wall3.GetComponent<PolygonCollider2D>()->vertices = { 
        Vector2(-.5f, .5f) * wall3.transform.scale, 
        Vector2(.5f, .5f) * wall3.transform.scale, 
        Vector2(.5f, -.25f) * wall3.transform.scale,
        Vector2(0.f, -.5f) * wall3.transform.scale,
        Vector2(-.5f, -.25f) * wall3.transform.scale 
    };

    FindFirstObjectWithTag("MainCamera")->transform.SetParent(&player.transform, false);

    player.speed = 6.f;
    enemy.speed = 3.f;
}

void Game::PlayScene(sf::RenderWindow& window, InputSystem& input)
{

    Player& player = *FindFirstObjectByType<Player>();
    AI& enemy = *FindFirstObjectByType<AI>();
    GameObject& wall = *FindFirstObjectWithTag("wall");

    window.clear(sf::Color::Green);
    Scene::PlayScene(window, input);

    //------------------------PLAYER------------------------

    // Player Movement

    float x, y = 0;
    y = input.getButton().up ? -1.0f :
        input.getButton().down ? 1.0f : 0.0f;

    x = input.getButton().right ? 1.0f :
        input.getButton().left ? -1.0f : 0.0f;

    Vector2 dir = Vector2(x, y);

    if (dir.x() != 0 || dir.y() != 0)
        dir = dir.normalized() * player.speed;

    (*player.rigidbody).velocity = 
        dir;

    //------------------------ENEMIES------------------------
    
    
    enemy.SetTargetPosition(player.transform.position);

    (*enemy.rigidbody).velocity *= enemy.speed;
}
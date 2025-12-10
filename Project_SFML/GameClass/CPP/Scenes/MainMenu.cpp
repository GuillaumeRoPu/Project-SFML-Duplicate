#include "Scenes/MainMenu.h"
#include "Manager/FontManager.h"
#include "Manager/SceneManager.h"
#include "Scenes/lascen.h"
#include "Engine/Time.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <cstdlib>


MainMenu::MainMenu()
{
}

void MainMenu::PlayScene(sf::RenderWindow& window, InputSystem& input)
{
    Scene::PlayScene(window, input);

    // ====== TIMERS ANIMATIONS ======
    static float animTime = 0.0f; // bounce du titre
    static float blinkTime = 0.0f; // clignotement texte
    animTime += Time::DeltaTime() * 3.f;
    blinkTime += Time::DeltaTime() * 4.f;

    float titleScale = 1.0f + std::sin(animTime) * 0.05f;

    // ====== ETAT DU MENU / TRANSITION ======
    static int   selectedIndex = 0;    // 0 = JOUER, 1 = QUITTER
    static bool  startTransition = false;
    static float flashTimer = 0.f;
    static float fadeAlpha = 0.f;

    const float FLASH_DURATION = 0.20f;  // durée du flash blanc

    static bool prevUp = false;
    static bool prevDown = false;
    static bool prevEnter = false;

    bool up = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
    bool down = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
    bool enter = sf::Keyboard::isKeyPressed(sf::Keyboard::Enter);

    // ====== CALCUL DES POSITIONS EN FONCTION DE LA VIEW ======
    sf::Vector2u winSize = window.getSize();

    sf::Vector2f worldTopLeft = window.mapPixelToCoords(sf::Vector2i(0, 0));
    sf::Vector2f worldBottomRight = window.mapPixelToCoords(
        sf::Vector2i(static_cast<int>(winSize.x), static_cast<int>(winSize.y))
    );
    sf::Vector2f worldSize(
        worldBottomRight.x - worldTopLeft.x,
        worldBottomRight.y - worldTopLeft.y
    );

    // Positions écran -> monde
    sf::Vector2f worldTitle = window.mapPixelToCoords(
        sf::Vector2i(static_cast<int>(winSize.x / 2), static_cast<int>(winSize.y / 3))
    );
    sf::Vector2f worldPlay = window.mapPixelToCoords(
        sf::Vector2i(static_cast<int>(winSize.x / 2), static_cast<int>(winSize.y / 2))
    );
    sf::Vector2f worldQuit = window.mapPixelToCoords(
        sf::Vector2i(static_cast<int>(winSize.x / 2), static_cast<int>(winSize.y / 2 + 50))
    );
    sf::Vector2f worldHint = window.mapPixelToCoords(
        sf::Vector2i(static_cast<int>(winSize.x / 2), static_cast<int>(winSize.y * 0.8f))
    );

    // ====== TITRE ======
    sf::Text title;
    title.setFont(FontManager::f_pixelify);
    title.setString("2D Game");
    title.setCharacterSize(64);
    title.setFillColor(sf::Color::White);
    title.setOrigin(title.getLocalBounds().width / 2, title.getLocalBounds().height / 2);
    title.setPosition(worldTitle);
    title.setScale(titleScale, titleScale);

    // Ombre / glow
    sf::Text shadow = title;
    shadow.setFillColor(sf::Color(0, 0, 0, 150));
    shadow.setScale(titleScale * 1.02f, titleScale * 1.02f);
    shadow.setPosition(worldTitle + sf::Vector2f(4.f, 4.f));

    // ====== OPTIONS DU MENU ======
    sf::Text playText;
    playText.setFont(FontManager::f_pixelify);
    playText.setString("JOUER");
    playText.setCharacterSize(32);
    playText.setOrigin(playText.getLocalBounds().width / 2, playText.getLocalBounds().height / 2);
    playText.setPosition(worldPlay);

    sf::Text quitText;
    quitText.setFont(FontManager::f_pixelify);
    quitText.setString("QUITTER");
    quitText.setCharacterSize(32);
    quitText.setOrigin(quitText.getLocalBounds().width / 2, quitText.getLocalBounds().height / 2);
    quitText.setPosition(worldQuit);

    if (selectedIndex == 0)
    {
        playText.setFillColor(sf::Color::White);
        playText.setScale(1.1f, 1.1f);
        quitText.setFillColor(sf::Color(150, 150, 150));
        quitText.setScale(1.f, 1.f);
    }
    else
    {
        playText.setFillColor(sf::Color(150, 150, 150));
        playText.setScale(1.f, 1.f);
        quitText.setFillColor(sf::Color::White);
        quitText.setScale(1.1f, 1.1f);
    }

    // Sélecteur ">"
    sf::Text selector;
    selector.setFont(FontManager::f_pixelify);
    selector.setString(">");
    selector.setCharacterSize(32);
    selector.setFillColor(sf::Color::White);
    if (selectedIndex == 0)
        selector.setPosition(worldPlay + sf::Vector2f(-80.f, -10.f));
    else
        selector.setPosition(worldQuit + sf::Vector2f(-80.f, -10.f));

    // ====== TEXTE QUI CLIGNOTE ("ENTREE pour valider") ======
    float blinkValue = (std::sin(blinkTime) + 1.f) * 0.5f; // 0  1
    sf::Uint8 blinkAlpha = static_cast<sf::Uint8>(80.f + blinkValue * 175.f);

    sf::Text hint;
    hint.setFont(FontManager::f_pixelify);
    hint.setString("ENTREE pour valider");
    hint.setCharacterSize(20);
    hint.setOrigin(hint.getLocalBounds().width / 2, hint.getLocalBounds().height / 2);
    hint.setPosition(worldHint);
    hint.setFillColor(sf::Color(200, 200, 200, blinkAlpha));

    // ====== FOND : PIXELS QUI BOUGENT ======
    struct Pixel
    {
        sf::Vector2f pixPos; // en coordonnées pixels écran
        float speed;
    };
    static std::vector<Pixel> pixels;

    // Initialisation des pixels une seule fois
    if (pixels.empty())
    {
        const int PIXEL_COUNT = 60;
        pixels.reserve(PIXEL_COUNT);
        for (int i = 0; i < PIXEL_COUNT; ++i)
        {
            Pixel p;
            p.pixPos.x = static_cast<float>(std::rand() % winSize.x);
            p.pixPos.y = static_cast<float>(std::rand() % winSize.y);
            p.speed = 30.f + static_cast<float>(std::rand() % 40); // 30 à 70 px/s
            pixels.push_back(p);
        }
    }

    // Mise à jour des pixels (en coordonnées écran)
    for (auto& px : pixels)
    {
        px.pixPos.y += px.speed * Time::DeltaTime();
        if (px.pixPos.y > static_cast<float>(winSize.y) + 2.f)
        {
            px.pixPos.y = -2.f;
            px.pixPos.x = static_cast<float>(std::rand() % winSize.x);
        }
    }

    // ====== NAVIGATION MENU (si pas en transition) ======
    if (!startTransition)
    {
        if (up && !prevUp)
        {
            selectedIndex--;
            if (selectedIndex < 0) selectedIndex = 1;
        }
        if (down && !prevDown)
        {
            selectedIndex++;
            if (selectedIndex > 1) selectedIndex = 0;
        }

        if (enter && !prevEnter)
        {
            if (selectedIndex == 0)
            {
                // Lancer la transition vers le jeu
                startTransition = true;
                flashTimer = 0.f;
                fadeAlpha = 0.f;
            }
            else
            {
                window.close();
            }
        }
    }

    prevUp = up;
    prevDown = down;
    prevEnter = enter;

    // ====== DESSIN : fond de pixels d'abord ======
    for (const auto& px : pixels)
    {
        sf::Vector2f worldPos = window.mapPixelToCoords(
            sf::Vector2i(static_cast<int>(px.pixPos.x), static_cast<int>(px.pixPos.y))
        );

        sf::RectangleShape dot;
        dot.setSize(sf::Vector2f(2.f, 2.f));
        dot.setPosition(worldPos);
        dot.setFillColor(sf::Color(255, 255, 255, 120));
        window.draw(dot);
    }

    // Ensuite le titre + menu
    window.draw(shadow);
    window.draw(title);
    window.draw(playText);
    window.draw(quitText);
    if (!startTransition)
    {
        window.draw(selector);
        window.draw(hint);   // texte qui clignote
    }

    // ====== TRANSITION : FLASH BLANC PUIS FADE NOIR ======

    // 1) Flash blanc style arcade
    if (startTransition && flashTimer < FLASH_DURATION)
    {
        flashTimer += Time::DeltaTime();
        float t = flashTimer / FLASH_DURATION;
        if (t > 1.f) t = 1.f;

        // courbe en cloche 
        float x = 2.f * t - 1.f;           // -1  1
        float intensity = 1.f - x * x;     // max au milieu
        sf::Uint8 alpha = static_cast<sf::Uint8>(255.f * intensity);

        sf::RectangleShape flashRect;
        flashRect.setSize(worldSize);
        flashRect.setPosition(worldTopLeft);
        flashRect.setFillColor(sf::Color(255, 255, 255, alpha));
        window.draw(flashRect);

        return; // on n'enchaîne le fade noir qu'après la fin du flash
    }

    // 2) Fade noir vers la scène de jeu
    if (startTransition && flashTimer >= FLASH_DURATION)
    {
        fadeAlpha += 200.f * Time::DeltaTime();
        if (fadeAlpha >= 255.f)
        {
            fadeAlpha = 255.f;
            // Si tu as donné le nom "lascen" à ta scène :
            // SceneManager::LoadScene("lascen");
            // Sinon, par index :
            SceneManager::LoadScene(1);
        }

        sf::RectangleShape fadeRect;
        fadeRect.setSize(worldSize);
        fadeRect.setPosition(worldTopLeft);
        fadeRect.setFillColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(fadeAlpha)));
        window.draw(fadeRect);
    }
}

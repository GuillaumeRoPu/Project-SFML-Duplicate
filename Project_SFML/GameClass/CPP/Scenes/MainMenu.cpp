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

    static float animTime = 0.0f;
    static float blinkTime = 0.0f;
    static float sparkleTime = 0.0f;
    animTime += Time::DeltaTime() * 3.f;
    blinkTime += Time::DeltaTime() * 4.f;
    sparkleTime += Time::DeltaTime();

    float titleScale = 1.0f + std::sin(animTime) * 0.05f;

    static int   selectedIndex = 0;
    static bool  startTransition = false;
    static float flashTimer = 0.f;
    static float fadeAlpha = 0.f;

    const float FLASH_DURATION = 0.20f;

    static bool prevUp = false;
    static bool prevDown = false;
    static bool prevEnter = false;

    bool up = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
    bool down = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
    bool enter = sf::Keyboard::isKeyPressed(sf::Keyboard::Enter);

    sf::Vector2u winSize = window.getSize();

    sf::Vector2f worldTopLeft = window.mapPixelToCoords(sf::Vector2i(0, 0));
    sf::Vector2f worldBottomRight = window.mapPixelToCoords(
        sf::Vector2i(static_cast<int>(winSize.x), static_cast<int>(winSize.y))
    );
    sf::Vector2f worldSize(
        worldBottomRight.x - worldTopLeft.x,
        worldBottomRight.y - worldTopLeft.y
    );

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

    sf::Color matrixNeon(0, 255, 100);
    sf::Vector2f snappedTitlePos(
        std::round(worldTitle.x),
        std::round(worldTitle.y)
    );

    sf::Text title;
    title.setFont(FontManager::f_pixelify);
    title.setString("2D Game");
    title.setCharacterSize(68);
    title.setOrigin(title.getLocalBounds().width / 2, title.getLocalBounds().height / 2);
    title.setPosition(snappedTitlePos);
    title.setScale(titleScale, titleScale);

    sf::Text outline = title;
    outline.setFillColor(sf::Color(0, 60, 0));

    std::vector<sf::Vector2f> offsets = {
        { -2.f,  0.f }, {  2.f,  0.f }, {  0.f, -2.f }, {  0.f,  2.f },
        { -2.f, -2.f }, {  2.f, -2.f }, { -2.f,  2.f }, {  2.f,  2.f },
    };

    std::vector<sf::Text> outlineCopies;
    for (auto& off : offsets)
    {
        sf::Text t = outline;
        t.move(off);
        outlineCopies.push_back(t);
    }

    std::vector<sf::Text> glowLayers;
    for (int i = 1; i <= 4; ++i)
    {
        sf::Text g = title;
        float s = 1.0f + i * 0.06f;
        g.setScale(titleScale * s, titleScale * s);
        sf::Uint8 a = static_cast<sf::Uint8>(80 / i);
        g.setFillColor(sf::Color(matrixNeon.r, matrixNeon.g, matrixNeon.b, a));
        glowLayers.push_back(g);
    }

    float colorAnim = (std::sin(animTime * 1.5f) + 1.f) * 0.5f;
    sf::Color animatedNeon(
        0,
        static_cast<sf::Uint8>(180 + 75 * colorAnim),
        static_cast<sf::Uint8>(80 + 50 * colorAnim)
    );
    title.setFillColor(animatedNeon);

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
        quitText.setFillColor(sf::Color(50, 180, 50));
    }
    else
    {
        playText.setFillColor(sf::Color(50, 180, 50));
        quitText.setFillColor(sf::Color::White);
        quitText.setScale(1.1f, 1.1f);
    }

    sf::Text selector;
    selector.setFont(FontManager::f_pixelify);
    selector.setString(">");
    selector.setCharacterSize(32);
    selector.setFillColor(sf::Color(0, 255, 100));
    selector.setPosition(
        selectedIndex == 0 ? worldPlay + sf::Vector2f(-80.f, -10.f)
        : worldQuit + sf::Vector2f(-80.f, -10.f)
    );

    float blinkValue = (std::sin(blinkTime) + 1.f) * 0.5f;
    sf::Uint8 blinkAlpha = static_cast<sf::Uint8>(80.f + blinkValue * 175.f);

    sf::Text hint;
    hint.setFont(FontManager::f_pixelify);
    hint.setString("ENTREE pour valider");
    hint.setCharacterSize(20);
    hint.setOrigin(hint.getLocalBounds().width / 2, hint.getLocalBounds().height / 2);
    hint.setPosition(worldHint);
    hint.setFillColor(sf::Color(0, 255, 100, blinkAlpha));

    struct Pixel { sf::Vector2f pixPos; float speed; };
    static std::vector<Pixel> pixels;

    if (pixels.empty())
    {
        const int PIXEL_COUNT = 80;
        pixels.reserve(PIXEL_COUNT);
        for (int i = 0; i < PIXEL_COUNT; ++i)
        {
            Pixel p;
            p.pixPos.x = static_cast<float>(std::rand() % winSize.x);
            p.pixPos.y = static_cast<float>(std::rand() % winSize.y);
            p.speed = 80.f + static_cast<float>(std::rand() % 60);
            pixels.push_back(p);
        }
    }

    for (auto& px : pixels)
    {
        px.pixPos.y += px.speed * Time::DeltaTime();
        if (px.pixPos.y > static_cast<float>(winSize.y))
        {
            px.pixPos.y = 0.f;
            px.pixPos.x = static_cast<float>(std::rand() % winSize.x);
        }
    }

    if (!startTransition)
    {
        if (up && !prevUp) selectedIndex = selectedIndex == 0 ? 1 : 0;
        if (down && !prevDown) selectedIndex = selectedIndex == 0 ? 1 : 0;
        if (enter && !prevEnter)
        {
            if (selectedIndex == 0)
            {
                startTransition = true;
                flashTimer = 0.f;
                fadeAlpha = 0.f;
            }
            else window.close();
        }
    }

    prevUp = up;
    prevDown = down;
    prevEnter = enter;

    for (const auto& px : pixels)
    {
        sf::Vector2f worldPos = window.mapPixelToCoords(
            sf::Vector2i(static_cast<int>(px.pixPos.x), static_cast<int>(px.pixPos.y))
        );
        sf::RectangleShape dot;
        dot.setSize(sf::Vector2f(2.f, 2.f));
        dot.setPosition(worldPos);
        dot.setFillColor(sf::Color(0, 255, 100, 150));
        window.draw(dot);
    }

    for (auto& g : glowLayers) window.draw(g);
    for (auto& t : outlineCopies) window.draw(t);
    window.draw(title);
    window.draw(playText);
    window.draw(quitText);
    if (!startTransition)
    {
        window.draw(selector);
        window.draw(hint);
    }

    if (startTransition && flashTimer < FLASH_DURATION)
    {
        flashTimer += Time::DeltaTime();
        float t = flashTimer / FLASH_DURATION;
        float x = 2.f * t - 1.f;
        float intensity = 1.f - x * x;
        sf::Uint8 alpha = static_cast<sf::Uint8>(255.f * intensity);

        sf::RectangleShape flashRect;
        flashRect.setSize(worldSize);
        flashRect.setPosition(worldTopLeft);
        flashRect.setFillColor(sf::Color(0, 255, 100, alpha));
        window.draw(flashRect);
        return;
    }

    if (startTransition && flashTimer >= FLASH_DURATION)
    {
        fadeAlpha += 200.f * Time::DeltaTime();
        if (fadeAlpha >= 255.f)
        {
            fadeAlpha = 255.f;
            SceneManager::LoadScene(1);
        }

        sf::RectangleShape fadeRect;
        fadeRect.setSize(worldSize);
        fadeRect.setPosition(worldTopLeft);
        fadeRect.setFillColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(fadeAlpha)));
        window.draw(fadeRect);
    }
}
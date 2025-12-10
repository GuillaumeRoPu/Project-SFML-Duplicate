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
    static float scanTime = 0.0f;

    animTime += Time::DeltaTime() * 3.f;
    blinkTime += Time::DeltaTime() * 4.f;
    sparkleTime += Time::DeltaTime();
    scanTime += Time::DeltaTime();

    float titleScale = 1.0f + std::sin(animTime) * 0.05f;

    static int   selectedIndex = 0;
    static bool  startTransition = false;
    static float flashTimer = 0.f;
    static float fadeAlpha = 0.f;

    const float FLASH_DURATION = 0.20f;

    static bool prevUp = false;
    static bool prevDown = false;
    static bool prevEnter = false;

    bool up = input.getButton().up;
    bool down = input.getButton().down;
    bool enter = input.getButton().enter;

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
    outlineCopies.reserve(offsets.size());
    for (auto& off : offsets)
    {
        sf::Text t = outline;
        t.move(off);
        outlineCopies.push_back(t);
    }

    struct TitleState
    {
        sf::Vector2f pos;
        float scale;
    };
    static std::vector<TitleState> trail;
    static float trailTimer = 0.f;

    const std::size_t trailSize = 4;
    const float trailSampleInterval = 0.06f;

    trailTimer += Time::DeltaTime();

    if (trail.empty())
    {
        trail.resize(trailSize);
        for (std::size_t i = 0; i < trailSize; ++i)
        {
            trail[i].pos = snappedTitlePos;
            trail[i].scale = titleScale;
        }
    }

    if (trailTimer >= trailSampleInterval)
    {
        trailTimer -= trailSampleInterval;
        trail.insert(trail.begin(), TitleState{ snappedTitlePos, titleScale });
        if (trail.size() > trailSize)
            trail.pop_back();
    }

    std::vector<sf::Text> glowLayers;
    glowLayers.reserve(trail.size());
    for (std::size_t i = 0; i < trail.size(); ++i)
    {
        const TitleState& st = trail[i];
        sf::Text g = title;
        g.setPosition(st.pos);
        float s = st.scale * (1.0f + static_cast<float>(i + 1) * 0.03f);
        g.setScale(s, s);
        float falloff = 1.f - static_cast<float>(i) / static_cast<float>(trail.size());
        if (falloff < 0.f) falloff = 0.f;
        sf::Uint8 a = static_cast<sf::Uint8>(20.f + 60.f * falloff);
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
        quitText.setScale(1.f, 1.f);
    }
    else
    {
        playText.setFillColor(sf::Color(50, 180, 50));
        playText.setScale(1.f, 1.f);
        quitText.setFillColor(sf::Color::White);
        quitText.setScale(1.1f, 1.1f);
    }

    sf::Text selector;
    selector.setFont(FontManager::f_pixelify);
    selector.setString(">");
    selector.setCharacterSize(32);
    selector.setFillColor(sf::Color(0, 255, 100));
    selector.setPosition(
        selectedIndex == 0 ? worldPlay + sf::Vector2f(-95.f, -10.f)
        : worldQuit + sf::Vector2f(-95.f, -10.f)
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

    struct Pixel
    {
        sf::Vector2f pixPos;
        float speed;
    };
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
            else
            {
                window.close();
            }
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

    for (auto& g : glowLayers)
        window.draw(g);

    for (auto& t : outlineCopies)
        window.draw(t);

    static const int sparkleCount = 8;
    for (int i = 0; i < sparkleCount; ++i)
    {
        float angle = sparkleTime * 2.f + i * 0.8f;
        float radius = 40.f + 5.f * std::sin(sparkleTime * 3.f + i);
        sf::Vector2f offset(
            std::cos(angle) * radius,
            std::sin(angle) * radius * 0.5f
        );

        sf::Vector2f pixPos = snappedTitlePos + offset;
        sf::Vector2f worldPixPos(
            std::round(pixPos.x),
            std::round(pixPos.y)
        );

        sf::RectangleShape px(sf::Vector2f(3.f, 3.f));
        px.setPosition(worldPixPos);
        px.setFillColor(sf::Color(0, 255, 100, 220));
        window.draw(px);
    }

    sf::Text rgb1 = title;
    sf::Text rgb2 = title;
    sf::Text rgb3 = title;

    float vibX1 = std::sin(animTime * 10.f) * 1.2f;
    float vibY1 = std::cos(animTime * 9.f) * 0.8f;
    float vibX2 = std::sin(animTime * 11.f + 1.5f) * 1.0f;
    float vibY2 = std::cos(animTime * 8.f + 0.7f) * 0.8f;
    float vibX3 = std::sin(animTime * 9.f + 2.3f) * 0.8f;
    float vibY3 = std::cos(animTime * 10.f + 1.1f) * 0.6f;

    rgb1.setFillColor(sf::Color(0, 255, 160, 90));
    rgb2.setFillColor(sf::Color(0, 220, 100, 70));
    rgb3.setFillColor(sf::Color(80, 255, 120, 60));

    rgb1.move(vibX1, vibY1);
    rgb2.move(vibX2, vibY2);
    rgb3.move(vibX3, vibY3);

    static float scanPos = 0.f;
    const float scanSpeed = 80.f;
    const float bandHeight = 32.f;

    scanPos += scanSpeed * Time::DeltaTime();
    if (scanPos > worldSize.y + bandHeight)
        scanPos = -bandHeight;

    sf::RectangleShape scanBand;
    scanBand.setSize(sf::Vector2f(worldSize.x, bandHeight));
    scanBand.setPosition(sf::Vector2f(worldTopLeft.x, worldTopLeft.y + scanPos));
    scanBand.setFillColor(sf::Color(0, 255, 100, 18));
    window.draw(scanBand);

    sf::RectangleShape scanLine;
    scanLine.setSize(sf::Vector2f(worldSize.x, 2.f));
    scanLine.setPosition(sf::Vector2f(worldTopLeft.x, worldTopLeft.y + scanPos + bandHeight * 0.5f - 1.f));
    scanLine.setFillColor(sf::Color(150, 255, 180, 70));
    window.draw(scanLine);

    for (float y = worldTopLeft.y; y < worldBottomRight.y; y += 3.f)
    {
        sf::RectangleShape line;
        line.setSize(sf::Vector2f(worldSize.x, 1.f));
        line.setPosition(sf::Vector2f(worldTopLeft.x, y));
        line.setFillColor(sf::Color(0, 255, 100, 8));
        window.draw(line);
    }

    window.draw(rgb3);
    window.draw(rgb2);
    window.draw(rgb1);
    window.draw(title);
    window.draw(playText);
    window.draw(quitText);

    if (!startTransition)
    {
        window.draw(selector);
        window.draw(hint);
    }

    static std::vector<sf::Vector2f> noisePoints;
    if (noisePoints.empty())
    {
        const int NOISE_COUNT = 300;
        noisePoints.reserve(NOISE_COUNT);
        for (int i = 0; i < NOISE_COUNT; ++i)
        {
            sf::Vector2f p;
            p.x = static_cast<float>(std::rand() % winSize.x);
            p.y = static_cast<float>(std::rand() % winSize.y);
            noisePoints.push_back(p);
        }
    }

    for (const auto& np : noisePoints)
    {
        sf::Vector2f worldPos = window.mapPixelToCoords(
            sf::Vector2i(static_cast<int>(np.x), static_cast<int>(np.y))
        );

        float n = std::sin(animTime * 50.f + np.x * 0.15f + np.y * 0.21f);
        float v = (n + 1.f) * 0.5f;
        sf::Uint8 a = static_cast<sf::Uint8>(8.f + v * 30.f);

        sf::RectangleShape grain(sf::Vector2f(1.f, 1.f));
        grain.setPosition(worldPos);
        grain.setFillColor(sf::Color(0, 255, 100, a));
        window.draw(grain);
    }

    if (startTransition && flashTimer < FLASH_DURATION)
    {
        flashTimer += Time::DeltaTime();
        float t = flashTimer / FLASH_DURATION;
        if (t > 1.f) t = 1.f;

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

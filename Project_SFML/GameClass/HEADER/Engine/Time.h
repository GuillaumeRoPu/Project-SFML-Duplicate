#pragma once
#include <SFML/System.hpp>

class Time
{
public:
    static void Update()
    {
        deltaTime = clock.restart().asSeconds();
        fps = (deltaTime > 0) ? 1.0f / deltaTime : 0.0f;
        totalTime += deltaTime;
    }

    static float DeltaTime() { return deltaTime * timeScale; }
    static float FixedDeltaTime() { return fixedDeltaTime * timeScale; }
    static float UnscaledDeltaTime() { return deltaTime; }
    static float UnscaledFixedDeltaTime() { return fixedDeltaTime; }
    static float FPS() { return fps; }
    static float TotalTime() { return totalTime; }


    inline static float timeScale = 1.f;

private:
    inline static sf::Clock clock;
    inline static float deltaTime;
    inline static float totalTime;
    inline static float fps;
    inline static float fixedDeltaTime = 1.0f / 60.0f;
};
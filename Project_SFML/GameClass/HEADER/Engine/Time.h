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

    static float DeltaTime() { return deltaTime; }
    static float FPS() { return fps; }
    static float TotalTime() { return totalTime; }

    inline static float fixedDeltaTime = 1.0f / 60.0f;

private:
    inline static sf::Clock clock;
    inline static float deltaTime = 0.0f;
    inline static float totalTime = 0.0f;
    inline static float fps = 0.0f;
};
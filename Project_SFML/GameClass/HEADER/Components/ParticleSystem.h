#pragma once
#include <SFML/Graphics.hpp>
#include "Components/Component.h"
#include <list>
#include "Engine/FryEngine.h"

struct Particle
{
	float lifetime = 5;
	float time = 0;
	Vector2 direction;

	sf::CircleShape shape;
};

class ParticleSystem : public Component
{
public:

	//System
	bool loop = false;
	float duration = 5;
	float startDelay = 0;
	int maxParticles = 1000;

	// Particles spawn
	float rateOverTime = 10;

	bool burst = false;
	float burstTime = 0;
	int burstCount = 4;

	// particles life
	bool randomLifetime = false;
	float lifetime = 5;
	float minLifetime = 2;
	float maxLifetime = 5;

	// particle parameters
	float maxDegrees = 180;
	float radius = 5;
	float rotation = 0;
	float speed = 1;

	// particles size
	bool randomSize = false;
	float size = 1;
	float minSize = 1;
	float maxSize = 1;

	// Color
	sf::Color startColor = sf::Color::White;
	bool colorByLifetime = false;
	fry::Gradient gradient;

	void UpdateParticleSystem();
	void ClearParticleSystem();
	void DrawParticleSystem(sf::RenderWindow& window);

	void PlayParticleSystem();

	bool isPlaying() { return _play; }
	int particleCount() { return (int)_particles.size(); }

private:
	float _startTime = 0;
	float _rateTime = 0;
	float _burstChrono = 0;
	float _lifeTime = 0;

	bool _play;

	std::list<Particle> _particles;

	void AddParticleToSystem(float lifetime);
};
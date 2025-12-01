#include <Components/ParticleSystem.h>
#include "Objects/GameObject.h"
#include "Scene.h";

void ParticleSystem::AddParticleToSystem(float lifetime)
{
	Particle p;
	p.lifetime = lifetime;

	float rad = size;
	if (randomSize)
		rad = fry::Random::Range(minSize, maxSize);

	p.shape.setRadius(rad * fry::pixelPerPoint);
	p.shape.setOrigin(sf::Vector2f(p.shape.getRadius(), p.shape.getRadius()));
	p.shape.setFillColor(startColor);

	const float PI = 3.14159265f;
	float angle = fry::Random::Range(-PI*(maxDegrees - rotation)/180, PI * (maxDegrees + rotation) / 180);
	float distance = fry::Random::Range(0.0f, radius * fry::pixelPerPoint);
	float x = transform().position.x() + cos(angle) * distance;
	float y = transform().position.y() + sin(angle) * distance;

	p.direction = Vector2(cos(angle), sin(angle)) * radius;

	p.shape.setPosition(sf::Vector2f(transform().position.x() * fry::pixelPerPoint, transform().position.y() * fry::pixelPerPoint));

	_particles.push_back(p);
	
}

void ParticleSystem::UpdateParticleSystem()
{


	float time = deltaTime ? Time::DeltaTime() : Time::UnscaledDeltaTime();
	float fixedTime = deltaTime ? Time::FixedDeltaTime() : Time::UnscaledFixedDeltaTime();

	Vector2 pos = transform().position * fry::pixelPerPoint;

	int count = (int)_particles.size();
	std::list<Particle>::iterator p = _particles.begin();

	for (int i = 0; i < count; i++)
	{
		if ((*p).time > (*p).lifetime)
		{
			p = _particles.erase(p);
			continue;
		}

		float ratio = (*p).time / (*p).lifetime;

		// Move particles
		Vector2 dir = Vector2((pos.x() - (*p).shape.getPosition().x) * -1, (pos.y() - (*p).shape.getPosition().y) * -1);
		if (dir != Vector2(0, 0))
			dir = dir.normalized();
		(*p).shape.setPosition((*p).shape.getPosition() + sf::Vector2f{(*p).direction.x(), (*p).direction.y()} * speed * fixedTime * fry::pixelPerPoint);

		// Gradient particles
		if (colorByLifetime)
			(*p).shape.setFillColor(gradient.GetColor(ratio));

		if (sizeByLifeTime)
		{
			float finalSize = (*p).shape.getRadius() * Mathf::Lerp(size_zero, size_one, ratio);
			(*p).shape.setRadius(finalSize);
		}

		(*p).time += time;
		p++;
	}

	
}

void ParticleSystem::ClearParticleSystem()
{
	_particles.clear();
	_burstChrono = 0;
	_rateTime = 0;
}

void ParticleSystem::DrawParticleSystem(sf::RenderWindow& window)
{

	int count = (int)_particles.size();
	std::list<Particle>::iterator p = _particles.begin();

	for (int i = 0; i < count; i++)
	{
		window.draw((*p).shape);
		p++;
	}

}

void ParticleSystem::PlayParticleSystem()
{

	float time = deltaTime ? Time::DeltaTime() : Time::UnscaledDeltaTime();

	// Delay before play particles
	if (!_play && _loopPlay)
	{
		_startTime += time;
		if (_startTime > startDelay)
		{
			_startTime = 0;
			_play = true;
			_burstChrono = burstTime;

			_loopPlay = loop;
		}
	}

	// Duration of the system
	if (!loop)
	{
		_lifeTime += time;
		if (_lifeTime > duration)
		{
			_play = false;
			_lifeTime = 0;
			if (destroyWhenDie)
			{
				_waitForDie = true;
			}
		}
	}

	if (_waitForDie && particleCount() <= 0)
		scene().DestroyObject(owner);

	// Spawn x particle in 1 second
	if (rateOverTime > 0 && _play)
	{
		_rateTime += time;
		if (_rateTime > 1 / rateOverTime)
		{
			_rateTime = 0;

			if (_particles.size() < maxParticles)
			{
				float life = lifetime;
				if (randomLifetime)
					life = fry::Random::Range(minLifetime, maxLifetime);
				AddParticleToSystem(life);
			}
		}
	}

	// Spawn x particles all n seconds
	if (burst && _play && _canBurst)
	{
		_burstChrono += time;
		if (_burstChrono > burstTime)
		{
			if (burstNumber > 0)
			{
				_burstCount++;
				_canBurst = _burstCount < burstNumber;
			}
			_burstChrono = 0;
			for (int i = 0; i < burstCount; i++)
			{
				if (_particles.size() == maxParticles)
					break;

				float life = lifetime;
				if (randomLifetime)
					life = fry::Random::Range(minLifetime, maxLifetime);
				AddParticleToSystem(life);
			}
		}
	}
}
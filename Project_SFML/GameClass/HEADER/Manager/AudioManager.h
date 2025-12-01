#pragma once
#include "SFML/Audio.hpp"
#include <string>
#include <list>
#include <map>
#include <SFML\System\Time.hpp>

struct AudioParameter
{
	std::string path = "";
	float volume = 100;
	sf::Time start = sf::seconds(0.f);

	AudioParameter(std::string path, float volume, float start)
	{
		this->path = path;
		this->volume = volume;
		this->start = sf::seconds(start);
	}
};

class AudioManager
{
	static std::map<std::string, sf::SoundBuffer> buffers;
	static std::list<sf::Sound> sfx;

	static bool _canPlay;

public:
	// Sounds
	inline static const AudioParameter shoot = AudioParameter("res/Sounds/shoot.wav", 50, 0);
	inline static const AudioParameter destroy = AudioParameter("res/Sounds/destroy.wav", 50, 0.1f);
	inline static const AudioParameter touch = AudioParameter("res/Sounds/playerTouch.wav", 50, 0);
	inline static const AudioParameter defeat = AudioParameter("res/Sounds/defeat.wav", 50, 0);

public:
	static void Initialize();
	static void LoadBuffer(std::string path);
	static void PlaySound(AudioParameter audio);
	static void SoloSounds(std::list<AudioParameter> audios);
};

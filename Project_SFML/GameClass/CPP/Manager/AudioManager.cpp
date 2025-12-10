#include "Manager/AudioManager.h"
#include <iostream>

std::map<std::string, sf::SoundBuffer> AudioManager::buffers;
std::list<sf::Sound> AudioManager::sfx;
bool AudioManager::_canPlay = true;

void AudioManager::Initialize()
{
	LoadBuffer(shoot.path);
	LoadBuffer(destroy.path);
	LoadBuffer(touch.path);
	LoadBuffer(defeat.path);
	LoadBuffer(collision.path);
}

void AudioManager::LoadBuffer(std::string path)
{
	sf::SoundBuffer buffer;
	if (buffer.loadFromFile(path))
	{
		buffers[path] = buffer;
	}
	else
	{
		std::cout << "Votre son au chemin : " << path << "n'existe pas." << std::endl;
	}
}

void AudioManager::PlaySound(AudioParameter audio, float volume, float pitch)
{
	if (!_canPlay)
		return;

	float vol = volume >= 0 ? volume : audio.volume;
	float pit = pitch >= 0 ? pitch : audio.pitch;

	sf::Sound sound;
	sound.setBuffer(buffers[audio.path]);
	sfx.push_back(sound);
	sfx.back().setVolume(vol);
	sfx.back().setPlayingOffset(audio.start);
	sfx.back().setPitch(pit);
	sfx.back().play();

	for (auto it = sfx.begin(); it != sfx.end(); )
	{
		if (it->getStatus() == sf::Sound::Stopped)
			it = sfx.erase(it);
		else
			it++;
	}
}

void AudioManager::SoloSounds(std::list<AudioParameter> audios)
{
	_canPlay = false;

	sfx.clear();

	for (AudioParameter audio : audios)
	{
		sf::Sound sound;
		sound.setBuffer(buffers[audio.path]);
		sfx.push_back(sound);
		sfx.back().setVolume(audio.volume);
		sfx.back().setPlayingOffset(audio.start);
		sfx.back().setPitch(audio.pitch);
		sfx.back().play();
	}
}

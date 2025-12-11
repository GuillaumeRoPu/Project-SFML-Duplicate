#include "Scripts/MusicController.h"
#include "Manager\AudioManager.h"

void MusicController::Start()
{
	if (gameMusic)
		AudioManager::PlayMusic(AudioManager::game, true, 10);
	else
		AudioManager::PlayMusic(AudioManager::menu, true, 20);
}

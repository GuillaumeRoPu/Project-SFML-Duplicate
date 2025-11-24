#include "Manager/SceneManager.h"
#include "Scene.h"

Scene* SceneManager::_currentScene;
std::vector<std::unique_ptr<Scene>> SceneManager::_scenes;

void SceneManager::PlayCurrentScene(sf::RenderWindow& window, InputSystem& input)
{
	_currentScene->PlayScene(window, input);
}

void SceneManager::LoadScene(int sceneIndex)
{
	if (sceneIndex < _scenes.size())
		_currentScene = _scenes[sceneIndex].get();
}

void SceneManager::LoadScene(std::string sceneName)
{
	for (std::unique_ptr<Scene>& s : _scenes)
	{
		if (s->name == sceneName)
		{
			_currentScene = s.get();
			break;
		}
	}
}

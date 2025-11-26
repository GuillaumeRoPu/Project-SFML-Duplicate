#pragma once
#include <SFML\Graphics\RenderWindow.hpp>
#include "InputSystem.h"
#include <string>
#include <vector>

class Scene;

class SceneManager
{
public:


	template <typename T, typename... Args>
	static void BuildScene(Args&&... args)
	{
		static_assert(std::is_base_of<Scene, T>::value, "T must inherit Scene");
		auto obj = std::make_unique<T>(std::forward<Args>(args)...);

		_scenes.push_back(std::move(obj));

		LoadScene(0);
	}

	static void PlayCurrentScene(sf::RenderWindow& window, InputSystem& input);

	static void LoadScene(int sceneIndex);

	static void LoadScene(std::string sceneName);

private:
	static Scene* _currentScene;
	static std::vector<std::unique_ptr<Scene>> _scenes;
};
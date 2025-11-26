#pragma once
#include "Manager/FontManager.h"
#include "Objects/GameObject.h"
#include "InputSystem.h"

#include <SFML/Graphics.hpp>
#include <vector>

class Scene
{
public:

	Scene(std::string name = "Scene");

	std::string name;

	virtual void PlayScene(sf::RenderWindow& window, InputSystem& input);

	// Create an object to your scene
	template <typename T, typename... Args>
	T* CreateGameObject(Args&&... args) {
		static_assert(std::is_base_of<GameObject, T>::value, "T must inherit GameObject");
		auto obj = std::make_unique<T>(std::forward<Args>(args)...);
		obj->Awake();

		T* ptr = obj.get();
		SceneObjects.push_back(std::move(obj));
		return ptr;
	}

	//--------------------GET FROM TYPE--------------------

	// Get an object of type T to your scene
	template <typename T>
	T* FindFirstObjectByType()
	{
		static_assert(std::is_base_of<GameObject, T>::value, "T must inherit GameObject");
		for (auto& obj : SceneObjects) {
			if (auto casted = dynamic_cast<T*>(obj.get())) {
				return casted;
			}
		}
		return nullptr;
	}

	// Get all objects of type T to your scene
	template <typename T>
	std::vector<T*> FindObjectsByType()
	{
		std::vector<T*> list;
		static_assert(std::is_base_of<GameObject, T>::value, "T must inherit GameObject");
		for (auto& obj : SceneObjects) {
			if (auto casted = dynamic_cast<T*>(obj.get())) {
				list.push_back(casted);
			}
		}
		return list;
	}

	//--------------------GET FROM TAG--------------------

	// Get an object of give tag to your scene
	GameObject* FindFirstObjectWithTag(std::string tag)
	{
		for (auto& obj : SceneObjects) {
			if (obj.get()->tag == tag) {
				return obj.get();
			}
		}
		return nullptr;
	}

	// Get all objects of give tag to your scene
	std::vector<GameObject*> FindObjectsWithTag(std::string tag)
	{
		std::vector<GameObject*> list;
		for (auto& obj : SceneObjects) {
			if (obj.get()->tag == tag) {
				list.push_back(obj.get());
			}
		}
		return list;
	}

	//--------------------DESTROY--------------------
	void DestroyObject(GameObject* T)
	{
		auto it = std::remove_if(SceneObjects.begin(), SceneObjects.end(),
			[&](const std::unique_ptr<GameObject>& go) {
				return go.get() == T;
			});

		if (it != SceneObjects.end())
			SceneObjects.erase(it, SceneObjects.end());
	}

private:
	bool start = false;

protected:

	std::vector<std::unique_ptr<GameObject>> SceneObjects;

	bool HasStart() { return start; }
	
};
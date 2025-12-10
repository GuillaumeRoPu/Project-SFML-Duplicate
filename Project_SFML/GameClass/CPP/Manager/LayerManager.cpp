#include "Manager\LayerManager.h"

std::map<fry::LayerMask, std::string> LayerManager::layers = {};

fry::LayerMask LayerManager::GetLayerByName(std::string name)
{
	int value = 0;
	for (auto bit : layers)
	{
		if (bit.second == name)
			value = bit.first.value;
	}

	return fry::LayerMask(value);
}

fry::LayerMask LayerManager::GetLayerByIndex(int index)
{
	int value = 0;

	int i = 0;
	if (index < layers.size())
	{
		for (auto bit : layers)
		{
			if (i == index)
				value = bit.first.value;

			i++;
		}
	}
	return fry::LayerMask(value);
}

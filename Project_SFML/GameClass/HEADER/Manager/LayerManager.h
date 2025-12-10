#pragma once
#include "Engine\FryEngine.h"
#include <string>
#include <map>

class LayerManager
{
public:
	static std::map<fry::LayerMask, std::string> layers;

public:
	static fry::LayerMask GetLayerByName(std::string name);

	static fry::LayerMask GetLayerByIndex(int index);
};
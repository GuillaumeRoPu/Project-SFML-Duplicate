#include "Engine/FryEngine.h"
#include "Manager/LayerManager.h"
using namespace fry;

bool fry::LayerMask::operator==(int v)
{
	return value == v;
}

bool fry::LayerMask::operator==(LayerMask layer)
{
	return value == layer.value;
}

bool fry::LayerMask::operator<(int v)
{
	return value < v;
}

bool fry::LayerMask::operator<(const LayerMask& layer) const
{
	return value < layer.value;
}

bool fry::LayerMask::operator>(int v)
{
	return value > v;
}

bool fry::LayerMask::operator>(const LayerMask& layer) const
{
	return value > layer.value;
}

int LayerMask::operator+(int v)
{
	int n = value;

	if (v < Mathf::Pow(2, LayerManager::layers.size()) - 1)
		n |= v;

	return n;
}

int LayerMask::operator-(int v)
{
	int n = value;

	if (v < Mathf::Pow(2, LayerManager::layers.size()) - 1)
	{
		if ((n & v) > 0)
		{
			n ^= v;
		}
	}

	return n;
}

void fry::LayerMask::operator+=(int v)
{
	if (v < Mathf::Pow(2, LayerManager::layers.size()) - 1)
		value |= v;
}

void fry::LayerMask::operator-=(int v)
{
	if (v < Mathf::Pow(2, LayerManager::layers.size()) - 1)
	{
		if ((value & v) > 0)
		{
			value ^= v;
		}
	}
}

#include "Engine/FryEngine.h"

using namespace fry;

Gradient Gradient::AddColor(sf::Color color, int time)
{
	// Detect if a color doesn't exit at the given time
	std::map<int, sf::Color>::iterator found = _colors.find(time);
	if (found != _colors.end())
		_colors.erase(time);

	_colors[std::clamp(time, 0, 100)] = color;
	Gradient g;
	g.SetGradient(_colors);
	return g;
}

sf::Color Gradient::GetColor(float time)
{
	int _currentTime = (int)(std::clamp(time, 0.0f, 1.0f) * 100);
	std::map<int, sf::Color>::iterator found = _colors.find(_currentTime);
	sf::Color _currentColorInMap;
	std::map<int, sf::Color>::iterator _currentIt;
	std::map<int, sf::Color>::iterator _nextIt;

	if (found != _colors.end())
	{
		_currentColorInMap = (*found).second;
		_currentIt = found;
		found++;
		if (found != _colors.end())
			_nextIt = found;
		else
			_nextIt = _currentIt;
	}
	else
	{
		std::map<int, sf::Color>::iterator it = _colors.begin();
		while ((*it).first < _currentTime)
		{
			_currentColorInMap = (*it).second;
			_currentIt = it;
			it++;
			if (it != _colors.end())
				_nextIt = it;
			else
				_nextIt = _currentIt;
		}
	}

	float lerpTime = (float)(*_nextIt).first/100 - (float)(*_currentIt).first/100;

	time = (lerpTime > 0) ? (time - (float)(*_currentIt).first / 100) / lerpTime : 0.0f;

	sf::Color newColor = Lerp::Color((*_currentIt).second, (*_nextIt).second, time);

	return newColor;
}

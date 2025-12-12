#pragma once
#include "Components\Scripting.h"

class MusicController : public Scripting
{
	void Start() override;

public:
	bool gameMusic;
};
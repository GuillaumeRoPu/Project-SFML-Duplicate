#include "Components\UI\Text.h"
#include "Components\RectTransform.h"
#include "Objects\GameObject.h"

void Text::Start()
{
	owner->DestroyComponent(owner->transform);
	owner->transform = owner->AddComponent<RectTransform>();
}

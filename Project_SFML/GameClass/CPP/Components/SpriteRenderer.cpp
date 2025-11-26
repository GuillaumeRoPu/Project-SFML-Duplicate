#include "Components/SpriteRenderer.h"

SpriteRenderer::SpriteRenderer()
{
}

SpriteRenderer::SpriteRenderer(std::string path)
{
	SetTexture(path);
}

void SpriteRenderer::SetTexture(std::string path)
{
	if (!texture.loadFromFile(path))
	{
		std::cout << "Texture can't be load.\n";
	}

}

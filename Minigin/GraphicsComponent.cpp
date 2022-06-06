#include "MiniginPCH.h"
#include "GraphicsComponent.h"

#include <iomanip>

#include "ResourceManager.h"
#include "Renderer.h"
#include "Font.h"
#include "LocationComponent.h"
#include "Texture2D.h"
#include "GameObject.h"

dae::SpriteMapTextureComponent::SpriteMapTextureComponent(
	const std::shared_ptr<GameObject> go, const std::string& filename,
	int                               startX, int            startY, int width, int height, float scale)
	: Component(go), m_Texture(ResourceManager::GetInstance().LoadTexture(filename)),
	  x(startX), y(startY), w(width), h(height), scale(scale)
{
}

void dae::SpriteMapTextureComponent::Render(float)
{
	const auto& pos = GetEntity()->GetComponent<LocationComponent>()->GetPosition();
	Renderer::GetInstance().RenderTexture(
		*m_Texture, x, y, w, h,
		pos.x, pos.y, w * scale, h * scale);
}

dae::TextureComponent::TextureComponent(std::shared_ptr<GameObject> go, const std::string& filename)
	: Component(go), m_Texture(ResourceManager::GetInstance().LoadTexture(filename))
{
}

void dae::TextureComponent::Render(float)
{
	const auto& pos = GetEntity()->GetComponent<LocationComponent>()->GetPosition();
	Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);
}


dae::TextComponent::TextComponent(const std::shared_ptr<GameObject> go,
                                  const std::string&                text,
                                  const std::shared_ptr<Font>&      font)
	: Component(go), m_Text(text), m_Font(font), m_TextTexture(nullptr)
{
}

dae::TextComponent::TextComponent(const std::shared_ptr<GameObject> go,
                                  const std::string&                text,
                                  const std::shared_ptr<Font>&      font,
                                  const int                         wrapLength)
	: Component(go), m_Text(text), m_Font(font), m_TextTexture(nullptr), m_WrapLength(wrapLength)
{
}

void dae::TextComponent::Update(float)
{
	if ((m_NeedsUpdate || m_AlwaysUpdate))
	{
		if (m_TextLink != nullptr && m_TextLink)
			m_Text = *m_TextLink.get();

		// We can not render an empty text field.
		// We then set the text to a space to the text does get cleared

		if (m_Text.empty())
			m_Text = " ";

		const auto surf = TTF_RenderUTF8_Blended_Wrapped(m_Font->GetFont(), m_Text.c_str(), m_Color, m_WrapLength);
		if (surf == nullptr)
		{
			printf("Error : %s", TTF_GetError());
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr)
		{
			printf("Error : %s", SDL_GetError());
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_FreeSurface(surf);
		m_TextTexture = std::make_shared<Texture2D>(texture);
		m_NeedsUpdate = false;
	}
}

void dae::TextComponent::Render(float)
{
	if (m_TextTexture != nullptr)
	{
		const auto& pos = GetEntity()->GetComponent<LocationComponent>()->GetPosition();
		Renderer::GetInstance().RenderTexture(*m_TextTexture, pos.x, pos.y);
	}
}

// This implementation uses the "dirty flag" pattern
void dae::TextComponent::SetText(const std::string& text)
{
	m_Text.assign(text);
	m_NeedsUpdate = true;
}

void dae::TextComponent::SetColor(const SDL_Color color)
{
	m_Color       = color;
	m_NeedsUpdate = true;
}

void dae::TextComponent::SetTextLink(const std::shared_ptr<std::string> textRef)
{
	m_TextLink     = textRef;
	m_AlwaysUpdate = true;
}

#include "MiniginPCH.h"
#include "GraphicsComponent.h"

#include <iomanip>

#include "ResourceManager.h"
#include "Renderer.h"
#include "Font.h"
#include "LocationComponent.h"
#include "Texture2D.h"
#include "GameObject.h"

dae::TextureComponent::TextureComponent(std::shared_ptr<GameObject> go, const std::string& filename)
	: Component(go), m_Texture(ResourceManager::GetInstance().LoadTexture(filename))
{
}

void dae::TextureComponent::Render(float)
{
	const auto& pos = GetEntity()->GetComponent<LocationComponent>()->GetTransform().GetPosition();
	Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);
}


dae::TextComponent::TextComponent(const std::shared_ptr<GameObject> go, const std::string& text,
                                  const std::shared_ptr<Font>&      font)
	: Component(go), m_Text(text), m_Font(font), m_TextTexture(nullptr)
{
}

void dae::TextComponent::Update(float)
{
	if (m_NeedsUpdate)
	{
		const auto surf = TTF_RenderText_Blended(m_Font->GetFont(), m_Text.c_str(), m_Color);
		if (surf == nullptr)
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr)
		{
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
		const auto& pos = GetEntity()->GetComponent<LocationComponent>()->GetTransform().GetPosition();
		Renderer::GetInstance().RenderTexture(*m_TextTexture, pos.x, pos.y);
	}
}

// This implementation uses the "dirty flag" pattern
void dae::TextComponent::SetText(const std::string& text)
{
	m_Text        = text;
	m_NeedsUpdate = true;
}

dae::FPSComponent::FPSComponent(std::shared_ptr<GameObject> go) :
	TextComponent(go, "", ResourceManager::GetInstance().LoadFont("Lingua.otf", 20))
{
	m_Color = {255, 255, 0};
}

void dae::FPSComponent::Update(float dt)
{
	TextComponent::Update(dt);
}

void dae::FPSComponent::Render(float dt)
{
	const float fps = 1.f / dt;

	std::stringstream ss;
	ss << std::fixed << std::setprecision(1) << fps;
	const std::string fps_s = ss.str();

	SetText(fps_s + " FPS");

	TextComponent::Render(dt);
}

dae::FPSPhysicsComponent::FPSPhysicsComponent(std::shared_ptr<GameObject> go)
	: TextComponent(go, "", ResourceManager::GetInstance().LoadFont("Lingua.otf", 20))
{
	m_Color = {255, 55, 0};
}

void dae::FPSPhysicsComponent::Update(float dt)
{
	const float fps = 1.f / dt;

	std::stringstream ss;
	ss << std::fixed << std::setprecision(1) << fps;
	const std::string fps_s = ss.str();

	SetText(fps_s + " TPS (physics)");

	TextComponent::Update(dt);
}

void dae::FPSPhysicsComponent::Render(float dt)
{
	TextComponent::Render(dt);
}

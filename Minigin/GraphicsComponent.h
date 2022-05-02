#pragma once

#include "Component.h"

#include <string>
#include <memory>

namespace dae
{
	class GameObject;
	class Texture2D;

	class SpriteMapTextureComponent : public Component
	{
	public:
		SpriteMapTextureComponent() = default;
		SpriteMapTextureComponent(
			std::shared_ptr<GameObject> go, const std::string& filename,
			int                         startX, int            startY, int width, int height,
			float                       scale);

		void Update(float) override
		{
		}

		void Render(float dt) override;

	private:
		std::shared_ptr<Texture2D> m_Texture{};
		int                        x;
		int                        y;
		int                        w;
		int                        h;
		float                      scale;
	};

	class TextureComponent : public Component
	{
	public:
		TextureComponent() = default;
		TextureComponent(std::shared_ptr<GameObject> go, const std::string& filename);

		void Update(float) override
		{
		}

		void Render(float dt) override;

	private:
		std::shared_ptr<Texture2D> m_Texture{};
	};


	class Font;

	class TextComponent : public Component
	{
	public:
		TextComponent() = default;
		TextComponent(std::shared_ptr<GameObject> go, const std::string& text, const std::shared_ptr<Font>& font);

		void SetText(const std::string& text);

		void Update(float) override;
		void Render(float dt) override;

	protected:
		bool                       m_NeedsUpdate = true;
		std::string                m_Text;
		std::shared_ptr<Font>      m_Font;
		std::shared_ptr<Texture2D> m_TextTexture;

		SDL_Color m_Color = {255, 255, 255};
	};


	class FPSComponent : public TextComponent
	{
	public:
		FPSComponent(std::shared_ptr<GameObject> go);

		void Update(float dt) override;
		void Render(float dt) override;
	};


	class FPSPhysicsComponent : public TextComponent
	{
	public:
		FPSPhysicsComponent(std::shared_ptr<GameObject> go);

		void Update(float) override;
		void Render(float dt) override;
	};
}

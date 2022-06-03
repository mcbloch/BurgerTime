#include "MiniginPCH.h"
#include "Renderer.h"

#include <future>

#include "GameObject.h"
#include "SceneManager.h"
#include "Texture2D.h"


int GetOpenGLDriverIndex()
{
	auto       openglIndex = -1;
	const auto driverCount = SDL_GetNumRenderDrivers();
	for (auto i = 0; i < driverCount; i++)
	{
		SDL_RendererInfo info;
		if (!SDL_GetRenderDriverInfo(i, &info))
			if (!strcmp(info.name, "opengl"))
				openglIndex = i;
	}
	return openglIndex;
}


void dae::Renderer::Init(SDL_Window* window)
{
	m_Window = window;

	// Select render driver
	// - A render driver that supports HW acceleration is used when available
	// - Otherwise a render driver supporting software fallback is selected
	SDL_RendererInfo renderDriverInfo;
	uint32_t         rendererFlags   = SDL_RENDERER_PRESENTVSYNC;
	int32_t          nbRenderDrivers = SDL_GetNumRenderDrivers();

	// Render driver debug info
	// for (int i = 0; i < nbRenderDrivers; i++)
	// {
	// 	if (SDL_GetRenderDriverInfo(index, &renderDriverInfo) == 0)
	// 	{
	// 		std::cout << "RenderDriverInfo" << std::endl;
	// 		std::cout << "  Name: " << renderDriverInfo.name << std::endl;
	// 		std::cout << "  Max Texture Height: " << renderDriverInfo.max_texture_height << std::endl;
	// 		std::cout << "  Max Texture Width: " << renderDriverInfo.max_texture_width << std::endl;
	// 		std::cout << "  Amount of available texture formats: " << renderDriverInfo.num_texture_formats << std::endl;
	// 	}
	// }

	int renderDriverIndex = GetOpenGLDriverIndex();
	SDL_GetRenderDriverInfo(GetOpenGLDriverIndex(), &renderDriverInfo);

	if ((renderDriverInfo.flags & rendererFlags) == rendererFlags
		&& (renderDriverInfo.flags & SDL_RENDERER_ACCELERATED) == SDL_RENDERER_ACCELERATED)
	{
		// Using render driver with HW acceleration
		rendererFlags |= SDL_RENDERER_ACCELERATED;
		SDL_SetHint(SDL_HINT_RENDER_DRIVER, renderDriverInfo.name);
		std::cout << "Using Hardware accelerated renderer" << std::endl;
	}
	else
	{
		// Let SDL use the first render driver supporting software fallback
		std::cout << "Could not fine hardware accelerated renderer" << std::endl;
		std::cout << "Using (fallback) software renderer" << std::endl;
		for (int i = 0; i < nbRenderDrivers; i++)
		{
			SDL_GetRenderDriverInfo(i, &renderDriverInfo);
			if ((renderDriverInfo.flags & SDL_RENDERER_SOFTWARE) != 0)
			{
				renderDriverIndex = i;
				break;
			}
		}
		rendererFlags |= SDL_RENDERER_SOFTWARE;
	}

	std::cout << "Using renderer: " << renderDriverInfo.name << std::endl;
	m_Renderer = SDL_CreateRenderer(window, renderDriverIndex, rendererFlags);


	if (m_Renderer == nullptr)
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL(window, SDL_GL_GetCurrentContext());
	ImGui_ImplOpenGL2_Init();
}

void dae::Renderer::Render(float dt) const
{
	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_Renderer);

	SceneManager::GetInstance().Render(dt);

	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(m_Window);
	ImGui::NewFrame();
	// ImGui::ShowDemoWindow();

	// cacheExercise.Render();

	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	SDL_RenderPresent(m_Renderer);
}

void dae::Renderer::Destroy()
{
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	if (m_Renderer != nullptr)
	{
		SDL_DestroyRenderer(m_Renderer);
		m_Renderer = nullptr;
	}
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float width,
                                  const float      height) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}


void dae::Renderer::RenderTexture(
	const Texture2D& texture,
	const int        textureX, const int textureY, const int textureWidth, const int textureHeight,
	const float      x, const float      y) const
{
	SDL_Rect src{};
	src.x = textureX;
	src.y = textureY;
	src.w = textureWidth;
	src.h = textureHeight;
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), &src, &dst);
}


void dae::Renderer::RenderTexture(
	const Texture2D& texture,
	const int        textureX, const int textureY, const int textureWidth, const int textureHeight,
	const float      x, const float      y, const float      width, const float      height) const
{
	SDL_Rect src{};
	src.x = textureX;
	src.y = textureY;
	src.w = textureWidth;
	src.h = textureHeight;
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);

	// SDL_SetColorKey(texture.GetSDLTexture(), TRUE, 0);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), &src, &dst);
}

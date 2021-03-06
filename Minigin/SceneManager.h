#pragma once
#include "Singleton.h"

namespace dae
{
	class Scene;

	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name);

		void                                 Update(float deltaTime);
		void                                 Render(float dt);
		[[nodiscard]] std::shared_ptr<Scene> GetCurrentScene() const;
	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<std::shared_ptr<Scene>> m_Scenes;
	};
}

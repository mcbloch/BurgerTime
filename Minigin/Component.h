#pragma once

namespace dae
{
#define m_assert(expr, msg) assert(( (void)(msg), (expr) ))

	class GameObject;

	class Component
	{
	public:
		virtual void Init()
		{
		};
		virtual void Update(float dt) = 0;
		virtual void Render(float dt) = 0;


		void               MarkForRemoval() { m_Gravestone = true; }
		[[nodiscard]] bool GetGravestone() const { return m_Gravestone; }

		[[nodiscard]] std::shared_ptr<GameObject> GetEntity() const { return m_Entity; }
		void                                      SetEntity(std::shared_ptr<GameObject> gameObject);

		Component(std::shared_ptr<GameObject>);
		virtual ~Component() = default;

		Component(const Component& other)            = delete;
		Component(Component&& other)                 = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other)      = delete;

	private:
		bool                        m_Gravestone = false;
		std::shared_ptr<GameObject> m_Entity{};
	};
}

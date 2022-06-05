#pragma once
namespace dae
{
	class Transform final
	{
	public:
		[[nodiscard]] const glm::vec2& GetPosition() const { return m_Position; }
		void                           SetPosition(glm::vec2 pos);
	private:
		glm::vec2 m_Position{};
	};
}

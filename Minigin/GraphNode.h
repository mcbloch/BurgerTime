#pragma once

namespace dae
{
	class GraphNode
	{
	public:
		GraphNode(const int index, const glm::vec2& pos = {0, 0}) : m_Index(index), m_Position(pos)
		{
		}

		explicit GraphNode(const int idx) : m_Index(idx)
		{
		}

		~GraphNode() = default;

		[[nodiscard]] int GetIndex() const { return m_Index; }
		void              SetIndex(const int newIdx) { m_Index = newIdx; }

		[[nodiscard]] glm::vec2 GetPosition() const { return m_Position; }
		void                    SetPosition(const glm::vec2& newPos) { m_Position = newPos; }

		bool operator==(const GraphNode& rhs) const { return m_Index == rhs.m_Index; }

	protected:
		int       m_Index;
		glm::vec2 m_Position;
	};
}

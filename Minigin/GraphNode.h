#pragma once

#include "GraphEnums.h"
#include "Vector2.h"

namespace dae
{
	class GraphNode
	{
	public:
		GraphNode(const int index, const Vector2& pos = ZeroVector2) : m_Index(index), m_Position(pos)
		{
		}

		explicit GraphNode(const int idx) : m_Index(idx)
		{
		}

		~GraphNode() = default;

		int  GetIndex() const { return m_Index; }
		void SetIndex(const int newIdx) { m_Index = newIdx; }

		Vector2 GetPosition() const { return m_Position; }
		void    SetPosition(const Vector2& newPos) { m_Position = newPos; }

		bool operator==(const GraphNode& rhs) const { return m_Index == rhs.m_Index; }

	protected:
		int m_Index;
		Vector2 m_Position;
	};
}

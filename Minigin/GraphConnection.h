#pragma once

#include "GraphEnums.h"

namespace dae
{
	class GraphConnection final
	{
	public:
		explicit GraphConnection(int from = invalid_node_index, int to = invalid_node_index, float cost = 1.f);
		virtual  ~GraphConnection() = default;

		[[nodiscard]] int GetFrom() const { return m_From; }
		void              SetFrom(const int newFrom) { m_From = newFrom; }

		[[nodiscard]] int GetTo() const { return m_To; }
		void              SetTo(const int newTo) { m_To = newTo; }

		[[nodiscard]] float GetCost() const { return m_Cost; }
		void                SetCost(const float newCost) { m_Cost = newCost; }

		[[nodiscard]] bool IsValid() const { return (m_From != -1 && m_To != -1); }

		bool operator==(const GraphConnection& rhs) const;
		bool operator!=(const GraphConnection& rhs) const;

	protected:
		int m_From;
		int m_To;

		// the cost of traversing the edge
		float m_Cost;
	};
}

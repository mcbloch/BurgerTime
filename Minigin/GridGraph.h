#pragma once

#include "Graph.h"
#include "GraphConnection.h"
#include "GraphNode.h"

namespace dae
{
	template <class T_NodeType, class T_ConnectionType>
	class GridGraph : public Graph<T_NodeType, T_ConnectionType>
	{
	public:
		explicit GridGraph(bool isDirectional);
		GridGraph(int columns, int rows, int cellSize, bool isDirectionalGraph, bool isConnectedDiagonally,
		          float costStraight = 1.f, float costDiagonal = 1.5);
		void InitializeGrid(int   columns, int rows, int cellSize, bool isDirectionalGraph, bool isConnectedDiagonally,
		                    float costStraight = 1.f, float costDiagonal = 1.5);

		using Graph<T_NodeType, T_ConnectionType>::GetNode;
		T_NodeType* GetNode(const int col, const int row) const { return this->m_Nodes[GetIndex(col, row)]; }

		const typename Graph<T_NodeType, T_ConnectionType>::ConnectionList&
		GetConnections(const T_NodeType& node) const { return this->m_Connections[node.GetIndex()]; }

		const typename Graph<T_NodeType, T_ConnectionType>::ConnectionList& GetConnections(int idx) const
		{
			return this->m_Connections[idx];
		}

		[[nodiscard]] int GetRows() const { return m_NrOfRows; }
		[[nodiscard]] int GetColumns() const { return m_NrOfColumns; }

		[[nodiscard]] bool IsWithinBounds(int col, int row) const;
		[[nodiscard]] int  GetIndex(const int col, const int row) const { return row * m_NrOfColumns + col; }

		// returns the column and row of the node in a Vector2
		using Graph<T_NodeType, T_ConnectionType>::GetNodePos;
		virtual Vector2 GetNodePos(T_NodeType* pNode) const override;

		// returns the actual world position of the node
		using Graph<T_NodeType, T_ConnectionType>::GetNodeWorldPos;
		[[nodiscard]] Vector2 GetNodeWorldPos(int col, int row) const;
		[[nodiscard]] Vector2 GetNodeWorldPos(int idx) const override;

		[[nodiscard]] int GetNodeIdxAtWorldPos(const Vector2& pos) const override;

		void AddConnectionsToAdjacentCells(int col, int row);
		void AddConnectionsToAdjacentCells(int idx);
	private:
		int m_NrOfColumns;
		int m_NrOfRows;
		int m_CellSize;

		bool  m_IsConnectedDiagonally;
		float m_DefaultCostStraight;
		float m_DefaultCostDiagonal;

		const std::vector<Vector2> m_StraightDirections = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
		const std::vector<Vector2> m_DiagonalDirections = {{1, 1}, {-1, 1}, {-1, -1}, {1, -1}};

		// graph creation helper functions
		void AddConnectionsInDirections(int idx, int col, int row, std::vector<Vector2> directions);

		[[nodiscard]] float CalculateConnectionCost(int fromIdx, int toIdx) const;

		friend class GraphRenderer;
	};

	template <class T_NodeType, class T_ConnectionType>
	inline GridGraph<T_NodeType, T_ConnectionType>::GridGraph(bool isDirectional)
		: Graph<T_NodeType, T_ConnectionType>(isDirectional)
		  , m_NrOfColumns(0)
		  , m_NrOfRows(0)
		  , m_CellSize(5)
		  , m_IsConnectedDiagonally(true)
		  , m_DefaultCostStraight(1.f)
		  , m_DefaultCostDiagonal(1.5f)
	{
	}

	template <class T_NodeType, class T_ConnectionType>
	GridGraph<T_NodeType, T_ConnectionType>::GridGraph(
		const int   columns,
		const int   rows,
		const int   cellSize,
		bool        isDirectionalGraph,
		const bool  isConnectedDiagonally,
		const float costStraight /* = 1.f*/,
		const float costDiagonal /* = 1.5f */)
		: Graph<T_NodeType, T_ConnectionType>(isDirectionalGraph)
		  , m_NrOfColumns(columns)
		  , m_NrOfRows(rows)
		  , m_CellSize(cellSize)
		  , m_IsConnectedDiagonally(isConnectedDiagonally)
		  , m_DefaultCostStraight(costStraight)
		  , m_DefaultCostDiagonal(costDiagonal)
	{
		InitializeGrid(columns, rows, cellSize, isDirectionalGraph, isConnectedDiagonally, costStraight, costDiagonal);
	}

	template <class T_NodeType, class T_ConnectionType>
	inline void GridGraph<T_NodeType, T_ConnectionType>::InitializeGrid(
		const int   columns,
		const int   rows,
		const int   cellSize,
		bool        isDirectionalGraph,
		const bool  isConnectedDiagonally,
		const float costStraight /* = 1.f*/,
		const float costDiagonal /* = 1.5f */)
	{
		this->m_IsDirectionalGraph = isDirectionalGraph;
		m_NrOfColumns              = columns;
		m_NrOfRows                 = rows;
		m_CellSize                 = cellSize;
		m_IsConnectedDiagonally    = isConnectedDiagonally;
		m_DefaultCostStraight      = costStraight;
		m_DefaultCostDiagonal      = costDiagonal;

		// Create all nodes
		for (auto r = 0; r < m_NrOfRows; ++r)
		{
			for (auto c = 0; c < m_NrOfColumns; ++c)
			{
				int idx = GetIndex(c, r);
				this->AddNode(new T_NodeType(idx));
			}
		}

		// Create connections in each valid direction on each node
		for (auto r = 0; r < m_NrOfRows; ++r)
		{
			for (auto c = 0; c < m_NrOfColumns; ++c)
			{
				AddConnectionsToAdjacentCells(c, r);
			}
		}
	}

	template <class T_NodeType, class T_ConnectionType>
	bool GridGraph<T_NodeType, T_ConnectionType>::IsWithinBounds(const int col, const int row) const
	{
		return (col >= 0 && col < m_NrOfColumns && row >= 0 && row < m_NrOfRows);
	}


	template <class T_NodeType, class T_ConnectionType>
	void GridGraph<T_NodeType, T_ConnectionType>::AddConnectionsToAdjacentCells(const int col, const int row)
	{
		const int idx = GetIndex(col, row);

		// Add connections in all directions, taking into account the dimensions of the grid
		AddConnectionsInDirections(idx, col, row, m_StraightDirections);

		if (m_IsConnectedDiagonally)
		{
			AddConnectionsInDirections(idx, col, row, m_DiagonalDirections);
		}

		this->OnGraphModified(false, true);
	}

	template <class T_NodeType, class T_ConnectionType>
	inline void GridGraph<T_NodeType, T_ConnectionType>::AddConnectionsToAdjacentCells(int idx)
	{
		auto colRow = this->GetNodePos(idx);
		AddConnectionsToAdjacentCells(static_cast<int>(colRow.x), static_cast<int>(colRow.y));
	}

	template <class T_NodeType, class T_ConnectionType>
	void GridGraph<T_NodeType, T_ConnectionType>::AddConnectionsInDirections(
		int idx, const int col, const int row, const std::vector<Vector2> directions)
	{
		for (const auto d : directions)
		{
			const int neighborCol = col + static_cast<int>(d.x);
			const int neighborRow = row + static_cast<int>(d.y);

			if (IsWithinBounds(neighborCol, neighborRow))
			{
				int         neighborIdx    = neighborRow * m_NrOfColumns + neighborCol;
				const float connectionCost = CalculateConnectionCost(idx, neighborIdx);

				if (this->IsUniqueConnection(idx, neighborIdx)
					&& connectionCost < 100000) //Extra check for different terrain types
					this->AddConnection(new GraphConnection(idx, neighborIdx, connectionCost));
			}
		}
	}

	template <class T_NodeType, class T_ConnectionType>
	inline float GridGraph<T_NodeType, T_ConnectionType>::CalculateConnectionCost(int fromIdx, int toIdx) const
	{
		float cost = m_DefaultCostStraight;

		const Vector2 fromPos = this->GetNodePos(fromIdx);
		const Vector2 toPos   = this->GetNodePos(toIdx);

		if (static_cast<int>(fromPos.y) != static_cast<int>(toPos.y) &&
			static_cast<int>(fromPos.x) != static_cast<int>(toPos.x))
		{
			cost = m_DefaultCostDiagonal;
		}

		return cost;
	}

	template <class T_NodeType, class T_ConnectionType>
	Vector2 GridGraph<T_NodeType, T_ConnectionType>::GetNodePos(T_NodeType* pNode) const
	{
		auto col = pNode->GetIndex() % m_NrOfColumns;
		auto row = pNode->GetIndex() / m_NrOfColumns;

		return Vector2{static_cast<float>(col), static_cast<float>(row)};
	}

	template <class T_NodeType, class T_ConnectionType>
	Vector2 GridGraph<T_NodeType, T_ConnectionType>::GetNodeWorldPos(const int col, const int row) const
	{
		const Vector2 cellCenterOffset = {m_CellSize / 2.f, m_CellSize / 2.f};
		return Vector2{static_cast<float>(col) * m_CellSize, static_cast<float>(row) * m_CellSize} + cellCenterOffset;
	}

	template <class T_NodeType, class T_ConnectionType>
	Vector2 GridGraph<T_NodeType, T_ConnectionType>::GetNodeWorldPos(int idx) const
	{
		auto colRow = this->GetNodePos(idx);
		return this->GetNodeWorldPos(static_cast<int>(colRow.x), static_cast<int>(colRow.y));
	}

	template <class T_NodeType, class T_ConnectionType>
	inline int GridGraph<T_NodeType, T_ConnectionType>::GetNodeIdxAtWorldPos(const Vector2& pos) const
	{
		constexpr int idx = invalid_node_index;

		//Added extra check since  c = int(pos.x / m_CellSize); => doesn't work correctly when out of the lower bounds
		//TODO add grid start point
		if (pos.x < 0 || pos.y < 0)
		{
			return idx;
		}

		const int c = static_cast<int>(pos.x / m_CellSize);
		const int r = static_cast<int>(pos.y / m_CellSize);

		if (!IsWithinBounds(c, r))
			return idx;

		return GetIndex(c, r);
	}
}

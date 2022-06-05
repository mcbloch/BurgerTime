#pragma once

#include <vector>

#include "Graph.h"

namespace dae
{
	template <class T_NodeType, class T_ConnectionType>
	class BFS
	{
	public:
		BFS(Graph<T_NodeType, T_ConnectionType>* pGraph);

		std::vector<T_NodeType*> FindPath(T_NodeType* pStartNode, T_NodeType* pDestinationNode);
	private:
		Graph<T_NodeType, T_ConnectionType>* m_pGraph;
	};

	template <class T_NodeType, class T_ConnectionType>
	BFS<T_NodeType, T_ConnectionType>::BFS(Graph<T_NodeType, T_ConnectionType>* pGraph)
		: m_pGraph(pGraph)
	{
	}

	template <class T_NodeType, class T_ConnectionType>
	std::vector<T_NodeType*> BFS<T_NodeType, T_ConnectionType>::FindPath(
		T_NodeType* pStartNode, T_NodeType* pDestinationNode)
	{
		// const int nodeCount = m_pGraph->GetNrOfActiveNodes();
		auto queue = std::list<std::tuple<int, std::vector<T_NodeType*>>>();

		//visited[pStartNode->GetIndex()] = true;
		queue.push_back({pStartNode->GetIndex(), {pStartNode}});

		while (!queue.empty())
		{
			auto [v, path] = queue.front();

			queue.pop_front();

			for (const T_ConnectionType* con : m_pGraph->GetNodeConnections(v))
			{
				auto toIdx = con->GetTo();
				auto to    = m_pGraph->GetNode(con->GetTo());
				if (to == pDestinationNode)
				{
					path.push_back(to);
					return path;
				}
				if (std::find(path.begin(), path.end(), to) == path.end())
				{
					auto newPath = std::vector(path);
					newPath.push_back(to);
					queue.push_back({toIdx, newPath});
				}
			}
		}
		// TODO: Very bad performance 0_o
		// Return here should not be reached
		return {};
	}
};

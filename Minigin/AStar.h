#pragma once

#include <vector>
#include <algorithm>
#include <ranges>

#include "Graph.h"
#include "Navigation.h"

namespace dae
{
	template <class T_NodeType, class T_ConnectionType>
	class AStar
	{
	public:
		AStar(Graph<T_NodeType, T_ConnectionType>* pGraph, Heuristic hFunction);

		// stores the optimal connection to a node and its total costs related to the start and end node of the path
		struct NodeRecord
		{
			T_NodeType*       pNode = nullptr;
			T_ConnectionType* pConnection = nullptr;
			float             costSoFar = 0.f; // accumulated g-costs of all the connections leading up to this one
			float             estimatedTotalCost = 0.f; // f-cost (= costSoFar + h-cost)

			bool operator==(const NodeRecord& other) const
			{
				return pNode == other.pNode
					&& pConnection == other.pConnection
					&& costSoFar == other.costSoFar
					&& estimatedTotalCost == other.estimatedTotalCost;
			};

			// Compare using the F-scores
			bool operator<(const NodeRecord& other) const
			{
				return estimatedTotalCost < other.estimatedTotalCost;
			};
		};

		std::vector<T_NodeType*> FindPath(T_NodeType* pStartNode, T_NodeType* pGoalNode);

	private:
		float GetHeuristicCost(T_NodeType* pStartNode, T_NodeType* pEndNode) const;

		Graph<T_NodeType, T_ConnectionType>* m_pGraph;
		Heuristic                            m_HeuristicFunction;
	};

	template <class T_NodeType, class T_ConnectionType>
	AStar<T_NodeType, T_ConnectionType>::AStar(Graph<T_NodeType, T_ConnectionType>* pGraph, const Heuristic hFunction)
		: m_pGraph(pGraph)
		  , m_HeuristicFunction(hFunction)
	{
	}

	/*
	 * Using vectors is very performant, but we do a lot of lookups and can benefit from using hash tables.
	 *
	 * Using maps as data-structure for nodes has better performance when scaled to a larger grid.
	 * We can use maps because our lookups are always based on the node in the nodeRecord.
	 *
	 *	 Use an unordered_map that behaves like a hashmap for the closedList.
	 *		+ This gives us constant lookup, insertion and deletion times.
	 *	 Use a sorted vector for the openList.
	 *		+ We get constant lookup for the smallest element with the overhead of sorting.
	 *		- Linear lookup of other elements (maybe use a second unordered_map for this?)
	 *		/ logarithmic insertion and deletion.
	 *
	 * Interesting read: https://blog.quasardb.net/using-c-containers-efficiently
	 */
	template <class T_NodeType, class T_ConnectionType>
	std::vector<T_NodeType*> AStar<T_NodeType, T_ConnectionType>::FindPath(
		T_NodeType* pStartNode, T_NodeType* pGoalNode)
	{
		auto       path      = std::vector<T_NodeType*>(); // Final path
		auto       openMap   = std::vector<NodeRecord>(); // Connections to be checked
		auto       closedMap = std::unordered_map<T_NodeType*, NodeRecord>(); // Connections already checked
		NodeRecord currentRecord; // Holds the current to be evaluated connection

		NodeRecord startRecord = {pStartNode, nullptr, 0, GetHeuristicCost(pStartNode, pGoalNode)};
		openMap.push_back(startRecord);

		// if (static_cast<GridTerrainNode*>(pStartNode)->GetTerrainType() == TerrainType::Water ||
		// 	static_cast<GridTerrainNode*>(pGoalNode)->GetTerrainType() == TerrainType::Water)
		// {
		// 	std::cout << "Not possible to travel over water. Set the start and end destination somewhere reachable" <<
		// 		std::endl;
		// 	return path;
		// }

		while (!openMap.empty())
		{
			// 2 Continue searching for a connection that leads to the end node

			// 2.A Get the connection with the lowest F score
			sort(openMap.begin(), openMap.end());
			currentRecord = openMap.front();
			// currentRecord = (*std::min_element(openMap.begin(), openMap.end())).second;
			// std::cout << "Investigating " << currentRecord.ToString() << std::endl;

			// 2.B Check if this connection leads to the end node
			if (currentRecord.pNode == pGoalNode)
				break;

			// 2.D Check if any of these connections go to a node on the closed list
			const auto& nodeCons = m_pGraph->GetNodeConnections(currentRecord.pNode);
			for (auto con : nodeCons)
			{
				float       gCost  = currentRecord.costSoFar + con->GetCost();
				T_NodeType* toNode = m_pGraph->GetNode(con->GetTo());

				auto closedIt = closedMap.find(toNode);
				if (closedIt != closedMap.end())
				{
					if (auto [pNode, record] = (*closedIt); gCost < record.costSoFar)
						closedMap.erase(closedIt);
					else
						continue;
				}
				else
				{
					auto openIt = find_if(openMap.begin(), openMap.end(), [&](auto r) { return r.pNode == toNode; });
					if (openIt != openMap.end())
					{
						if (const auto& record = (*openIt); gCost < record.costSoFar)
							openMap.erase(openIt);
						else
							continue;
					}
				}

				// 2.F All expensive connections to the node we are investigating are now removed and we skipped this if we found a better closed or open record.
				// We add this connection to the openList
				openMap.push_back({toNode, con, gCost, gCost + GetHeuristicCost(toNode, pGoalNode)});
			}

			// 2.G Remove our nodeRecord from the openList and add it to the closedList
			openMap.erase(find_if(openMap.begin(), openMap.end(),
			                      [&](auto r) { return r.pNode == currentRecord.pNode; }));
			// std::cout << std::format("Adding {} to the endList", currentRecord.ToString()) << std::endl;
			closedMap.insert({currentRecord.pNode, currentRecord});
		}

		// 3 We now have all our needed nodes in the closedList. Reconstruct the path from the last connection to the startNode
		if (currentRecord.pNode != pGoalNode)
		{
			std::cout << "Could not find a path to the goalNode. Trying to get as close as possible." << std::endl;
			currentRecord = (*min_element(closedMap.begin(), closedMap.end(), [&](auto rTuple, auto rTuple2)
			{
				return GetHeuristicCost(rTuple.second.pNode, pGoalNode) < GetHeuristicCost(
					rTuple2.second.pNode, pGoalNode);
			})).second;
		}
		while (currentRecord.pNode != pStartNode)
		{
			path.push_back(currentRecord.pNode);
			// Seek the record leading to the currentNode and save it
			auto foundIt = closedMap.find(m_pGraph->GetNode(currentRecord.pConnection->GetFrom()));

			if (foundIt == closedMap.end())
			{
				std::cout << "Did not find a valid path. A node in the closedList does not have a parent." << std::endl;
				std::cout << currentRecord.pNode->GetIndex() << std::endl;
				std::cout << "Could not find a record for node with index: " <<
					currentRecord.pConnection->GetFrom() <<
					std::endl;
				break;
			}

			currentRecord = (*foundIt).second;
		}
		path.push_back(pStartNode);

		std::reverse(path.begin(), path.end());
		return path;
	}

	template <class T_NodeType, class T_ConnectionType>
	float AStar<T_NodeType, T_ConnectionType>::GetHeuristicCost(T_NodeType* pStartNode,
	                                                            T_NodeType* pEndNode) const
	{
		const glm::vec2 toDestination = m_pGraph->GetNodePos(pEndNode) - m_pGraph->GetNodePos(pStartNode);
		return m_HeuristicFunction(abs(toDestination.x), abs(toDestination.y));
	}
}

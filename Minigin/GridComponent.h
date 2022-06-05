#pragma once
#include "Component.h"

namespace dae
{
	enum class GridDirection
	{
		Top,
		Bottom,
		Left,
		Right,
	};

	/*
	 * A logical component representing the grid position of an object.
	 * An object that is more then halfway into a new grid position will move in the grid and launch an event.
	 * An object that arrives exactly onto a grid position will also launch an exact position event.
	 *
	 * Halfway is used for marking food elements
	 * Full is used to be able to climb ladders
	 */
	class GridComponent : public Component
	{
	public:
		GridComponent() = default;
		explicit GridComponent(std::shared_ptr<GameObject> go);

		void Update(float) override;
		void Render(float) override;

		std::pair<int, int> GetGridPos();
		bool                GetXFullyAligned() const;
		bool                GetYFullyAligned() const;

		// bool IsNeighbourInDirectionOf(GridDirection direction, GridComponent of);

		~GridComponent() override                            = default;
		GridComponent(const GridComponent& other)            = delete;
		GridComponent(GridComponent&& other)                 = delete;
		GridComponent& operator=(const GridComponent& other) = delete;
		GridComponent& operator=(GridComponent&& other)      = delete;


		// The base point of the grid. The grid positions are calculated relative to this.
		inline static int GridBaseX = 20;
		inline static int GridBaseY = 90;

		// Pixel size of one grid cell
		inline static int GridCellSizeX = 30;
		inline static int GridCellSizeY = 40;

	private:
		bool isXFullyAligned = false;
		bool isYFullyAligned = false;

		// The current grid position. Represented in columns and rows to differentiate between pixel coordinates.
		int m_GridR = -1;
		int m_GridC = -1;
	};
}

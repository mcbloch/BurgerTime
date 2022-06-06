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
	 * Overrides the Location of the LocationComponent when passed a desired GridPosition to start.
	 *
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
		GridComponent(std::shared_ptr<GameObject> go, glm::ivec2 gridPosStart);

		void Update(float) override;
		void Render(float) override;

		[[nodiscard]] glm::ivec2 GetGridPos() const;
		void                     UpdateGridPos();
		[[nodiscard]] glm::bvec2 GetFullyAligned() const;

		void ResetToStart() const;

		static glm::vec2  GetLocationFromGridPos(glm::ivec2);
		static glm::ivec2 GetGridPosFromLocation(glm::vec2);

		// bool IsNeighbourInDirectionOf(GridDirection direction, GridComponent of);

		~GridComponent() override                            = default;
		GridComponent(const GridComponent& other)            = delete;
		GridComponent(GridComponent&& other)                 = delete;
		GridComponent& operator=(const GridComponent& other) = delete;
		GridComponent& operator=(GridComponent&& other)      = delete;


		// The base point of the grid. The grid positions are calculated relative to this.
		inline static glm::ivec2 GridBase = {20, 90};

		// Pixel size of one grid cell
		inline static glm::ivec2 GridCellSize = {30, 40};

		// Maps are always the same size, Hardcode it atm
		inline static glm::ivec2 MaxGridSize = { 17, 10 };

	private:
		glm::bvec2 isFullyAligned{false, false};

		glm::ivec2 m_GridPosStart = {-1, -1};
		// The current grid position. Represented in columns and rows to differentiate between pixel coordinates.
		glm::ivec2 m_GridPos = {-1, -1};
	};
}

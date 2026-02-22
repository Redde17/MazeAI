#pragma once
#include <array>

#define NEIGHBOURS 4

//enum for the possible neghbours directions in a square maze
enum Direction {
	NORTH,
	SOUTH,
	EAST,
	WEST
};

class MazeNode {
private:
	std::array<MazeNode*, NEIGHBOURS> neighbours;

public:
	// \brief Empty constructor for node without neighbours
	MazeNode();

	// \brief Constructor for a node with all neighbours
	// 
	// The neighbours are pointers to other MazeNodes near the current one
	// 
	// \param northNeigbour	pointer to the neighbour above the current node
	// \param southNeigbour pointer to the neighbour below the current node
	// \param eastNeigbour	pointer to the neighbour right of the current node
	// \param westNeigbour	pointer to the neighbour left of the current node
	MazeNode(
		MazeNode* northNeigbour,
		MazeNode* southNeigbour,
		MazeNode* eastNeigbour,
		MazeNode* westNeigbour
	);

	// \brief Get a specific neighbour of the MazeNode
	// 
	// \param neighbour	Defines the neighbour to get from the node
	// 
	// \return MazeNode* pointer to the neighbouring node
	MazeNode* getNeighbour(Direction neighbour);

	// \brief Set a neighbour for the node
	// 
	// \param neighbour		Defines where the neighbour to set for the node is
	// \param nodeNeighbour		Defines the neighbour to set for the node
	void setNeighbour(Direction neighbour, MazeNode* nodeNeigbour);

	// \brief Set all the neighbours for the node
	// 
	// The neighbours are pointers to other MazeNodes near the current one
	// 
	// \param northNeigbour	pointer to the neighbour above the current node
	// \param southNeigbour	pointer to the neighbour below the current node
	// \param eastNeigbour	pointer to the neighbour right of the current node
	// \param westNeigbour	pointer to the neighbour left of the current node
	void setNeighbours(
		MazeNode* northNeigbour,
		MazeNode* southNeigbour,
		MazeNode* eastNeigbour,
		MazeNode* westNeigbour
	);

	// \brief Remove a neighbour from the node
	// 
	// \param neighbour	Defines the neighbour to remove from the node
	void removeNeighbour(Direction neighbour);

	// \brief Gets the amount of neighbours of the node
	// 
	// \return Integer for the amount of neighbours
	int getNeighboursAmount();

	// \brief Gets the opposite of a direction
	// \param Direction to invert
	// \return Direction inverted direction
	static Direction getOppositeDirection(Direction direction);
};
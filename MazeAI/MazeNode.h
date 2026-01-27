#pragma once
#include <array>

#define WALLS_AMOUNT 4

//enum for the possible walls in the node
enum NodeWall {
	NORTH,
	SOUTH,
	EAST,
	WEST
};

class MazeNode {
private:
	std::array<bool, WALLS_AMOUNT> walls;

public:
	// \brief Empty constructor for node without walls
	MazeNode();

	// \brief Constructor for a node with walls
	// 
	// Sets the walls for the MazeNode usiong bools
	// True = wall
	// False = no wall
	// 
	// \param northWall	bool for the north wall
	// \param southWall	bool for the south wall
	// \param eastWall	bool for the east wall
	// \param westWall	bool for the west wall
	MazeNode(
		const bool northWall, 
		const bool southWall, 
		const bool eastWall, 
		const bool westWall
	);

	// \brief Get tha status of a specific wall from the node
	// 
	// \param wall	Defines to wall to get from the node
	// 
	// \return bool value for the wall of the node
	bool getWall(NodeWall wall);
};


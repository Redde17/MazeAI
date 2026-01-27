#include "MazeNode.h"

// \brief Empty constructor for node without walls
MazeNode::MazeNode() {
	for (int i = 0; i < WALLS_AMOUNT; i++)
		walls[i] = false;
}

// \brief Constructor for a node with walls
// 
// Sets the walls for the MazeNode usiong bools
// True = wall
// False = no wall
// 
// \param northWall	bool for the north wall
// \param southWall bool for the south wall
// \param eastWall	bool for the east wall
// \param westWall	bool for the west wall
MazeNode::MazeNode(const bool northWall, const bool southWall, const bool eastWall, const bool westWall) {
	walls[NORTH] = northWall;
	walls[SOUTH] = southWall;
	walls[EAST] = eastWall;
	walls[WEST] = westWall;
}

// \brief Get tha status of a specific wall from the node
// 
// \param wall	Defines to wall to get from the node
// 
// \return bool value for the wall of the node
bool MazeNode::getWall(NodeWall wall) {
	return walls[wall];
}
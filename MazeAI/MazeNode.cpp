#include "MazeNode.h"

// \brief Empty constructor for node without neighbours
MazeNode::MazeNode() {
	for (int i = 0; i < NEIGHBOURS; i++)
		neighbours[i] = nullptr;
}

// \brief Constructor for a node with all neighbours
// 
// \param northNeigbour	pointer to the neighbour above the current node
// \param southNeigbour pointer to the neighbour below the current node
// \param eastNeigbour	pointer to the neighbour right of the current node
// \param westNeigbour	pointer to the neighbour left of the current node
MazeNode::MazeNode(MazeNode* northNeigbour, MazeNode* southNeigbour, MazeNode* eastNeigbour, MazeNode* westNeigbour) {
	this->setNeighbours(northNeigbour, southNeigbour, eastNeigbour, westNeigbour);
}

// \brief Get a specific neighbour of the MazeNode
// 
// \param neighbour	Defines the neighbour to get from the node
// 
// \return MazeNode* pointer to the neighbouring node
MazeNode* MazeNode::getNeighbour(Neighbour neighbour) {
	return neighbours[neighbour];
}

// \brief Set a neighbour for the node
// 
// \param neighbour		Defines where the neighbour to set for the node is
// \param nodeNeighbour		Defines the neighbour to set for the node
void MazeNode::setNeighbour(Neighbour neighbour, MazeNode* nodeNeigbour) {
	neighbours[neighbour] = nodeNeigbour;
}

// \brief Set all the neighbours for the node
// 
// The neighbours are pointers to other MazeNodes near the current one
// 
// \param northNeigbour	pointer to the neighbour above the current node
// \param southNeigbour pointer to the neighbour below the current node
// \param eastNeigbour	pointer to the neighbour right of the current node
// \param westNeigbour	pointer to the neighbour left of the current node
void MazeNode::setNeighbours(MazeNode* northNeigbour, MazeNode* southNeigbour, MazeNode* eastNeigbour, MazeNode* westNeigbour) {
	neighbours[NORTH] = northNeigbour;
	neighbours[SOUTH] = southNeigbour;
	neighbours[EAST] = eastNeigbour;
	neighbours[WEST] = westNeigbour;
}

// \brief Remove a neighbour from the node
// 
// \param neighbour	Defines the neighbour to remove from the node
void MazeNode::removeNeighbour(Neighbour neighbour) {
	neighbours[neighbour] = nullptr;
}

// \brief Gets the amount of neighbours of the node
// 
// \return Integer for the amount of neighbours
int MazeNode::getNeighboursAmount() {
	int amount = 0;

	for(MazeNode* neighbour : neighbours)
		if (neighbour)	//checks if neighbour is not nullptr
			amount++;

	return amount;
}
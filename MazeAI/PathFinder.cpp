#include "PathFinder.h"
#include <stack>
#include <map>
#include <iostream>
#include "MazeHandler.h"

///VisitedNode struct implementation//
// \brief VisitedNode constructor
PathFinder::VisitedNode::VisitedNode() {
	mazeNode = nullptr;
}

// \brief VisitedNode constructor
// \param *mazeNode: Pointer to a maze node
// \param position: Vector2 containing position of the maze node
PathFinder::VisitedNode::VisitedNode(MazeNode* mazeNode, const Vector2 position) {
	this->mazeNode = mazeNode;
	this->position = position;
}

// \brief Overload of operator =
void PathFinder::VisitedNode::operator=(const VisitedNode& other) {
	this->mazeNode = other.mazeNode;
	this->position = other.position;
}

// \brief Overload of operator ==
bool PathFinder::VisitedNode::operator==(const VisitedNode& rhs) {
	return position == rhs.position;
}

///PathFinder class implementation///
// \brief Find the path to a given maze
//
// \param pathFinderAlgorithm: Algorithm type to use for path search
// \param startPos: Starting position for the path finder
// \param finishPos: Finishing position for the path finder
// \param *mazePath: Pointer to the mazePath instance where to save the solution path
// \param *mazeMap: Pointer to the mazeMap instance containing the maze to solve
void PathFinder::findPath(const PathFinderAlgorithm pathFinderAlgorithm, const Vector2 startPos, const Vector2 finishPos, MazePath* mazePath, MazeNode* startingNode) {
	switch (pathFinderAlgorithm){
	case DEPTH_FIRST_SEARCH:
		findPathDFS(startPos, finishPos, mazePath, startingNode);
		break;
	default:
		break;
	}
}

// \brief Find the path to a given maze using random depth first search
//
// \param startPos: Starting position for the path finder
// \param finishPos: Finishing position for the path finder
// \param *mazePath: Pointer to the mazePath instance where to save the solution path
// \param *mazeMap: Pointer to the mazeMap instance containing the maze to solve
void PathFinder::findPathDFS(const Vector2 startPos, const Vector2 finishPos, MazePath* mazePath, MazeNode* startingNode) {
	std::stack<VisitedNode> recursionStack;
	
	//Choose the initial cell, mark it as path start and push it to the stack
	mazePath->setPathMapValue(startPos, MazePath::PATH);
	recursionStack.push(VisitedNode(startingNode, startPos));
	mazePath->setPathMapValue(Vector2(0, 0), MazePath::PATH);

	//While the stack is not empty
	VisitedNode currentNode, neighbourNode;
	while (!recursionStack.empty()) {
		//Pop a cell from the stack and make it a current cell
		currentNode = recursionStack.top();

		//If current cell is finish, stop path finding
		if (currentNode.position == finishPos)
			break;

		//If the current cell has any neighbours which have not been visited
		neighbourNode = getUnexploredNeighbour(currentNode, mazePath);
		//If the neighbourNode is equal to the currentNode, then currentNode has no available neighbours to explore
		if (neighbourNode == currentNode) {
			recursionStack.pop();
			mazePath->setPathMapValue(neighbourNode.position, MazePath::VISITED);
			continue;
		}

		//Mark neighbour as visited by adding it to the path map, and push it to the stack
		recursionStack.push(neighbourNode);
		mazePath->setPathMapValue(neighbourNode.position, MazePath::PATH);
	}
}

// \brief Get a non random unexplored neighbour from a given node
//
// \param mazeNode: Node from wich to get the neighbour
// \param *mazePath: Map with current path and visited nodes
//
// \return VisitedNode object rappresenting a neighbour to the passed node
PathFinder::VisitedNode PathFinder::getUnexploredNeighbour(const VisitedNode mazeNode, MazePath* mazePath) {
	//If there are no neighbours, returns the current mazeNode
	//if (!mazeNode.mazeNode->getNeighboursAmount())
	//	return mazeNode;

	//Get first available unvisited neighbour
	VisitedNode neighbourNode;
	if (neighbourNode.mazeNode = mazeNode.mazeNode->getNeighbour(NORTH)) {
		neighbourNode.position = Vector2(mazeNode.position.x, mazeNode.position.y - 1);
		//Check if node has been visited using the mazePath
		if (mazePath->getPathMapValue(neighbourNode.position) == MazePath::NOT_VISITED)
			return neighbourNode;
	} 
	if (neighbourNode.mazeNode = mazeNode.mazeNode->getNeighbour(SOUTH)) {
		neighbourNode.position = Vector2(mazeNode.position.x, mazeNode.position.y + 1);
		if (mazePath->getPathMapValue(neighbourNode.position) == MazePath::NOT_VISITED)
			return neighbourNode;
	}
	if (neighbourNode.mazeNode = mazeNode.mazeNode->getNeighbour(EAST)) {
		neighbourNode.position = Vector2(mazeNode.position.x + 1, mazeNode.position.y);
		if (mazePath->getPathMapValue(neighbourNode.position) == MazePath::NOT_VISITED)
			return neighbourNode;
	}
	if (neighbourNode.mazeNode = mazeNode.mazeNode->getNeighbour(WEST)) {
		neighbourNode.position = Vector2(mazeNode.position.x - 1, mazeNode.position.y);
		if (mazePath->getPathMapValue(neighbourNode.position) == MazePath::NOT_VISITED)
			return neighbourNode;
	}

	return mazeNode;
}
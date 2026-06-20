#include <stack>
#include <queue>
//#include <map>

#include "PathFinder.h"
#include "MazeHandler.h"
#include "DataCollector.h"
#include "searchable_priority_queue.h"


///VisitedNode struct implementation//
// \brief VisitedNode constructor
PathFinder::DFSNode::DFSNode() {
	mazeNode = nullptr;
}

// \brief VisitedNode constructor
// \param *mazeNode: Pointer to a maze node
// \param position: Vector2 containing position of the maze node
PathFinder::DFSNode::DFSNode(MazeNode* mazeNode, const Vector2 position) {
	this->mazeNode = mazeNode;
	this->position = position;
}

// \brief Overload of operator =
void PathFinder::DFSNode::operator=(const DFSNode& other) {
	this->mazeNode = other.mazeNode;
	this->position = other.position;
}

// \brief Overload of operator ==
bool PathFinder::DFSNode::operator==(const DFSNode& rhs) const {
	return position == rhs.position;
}

// \brief Overload of operator !=
bool PathFinder::DFSNode::operator!=(const DFSNode& rhs) const {
	return !(position == rhs.position);
}

// \brief Overload of operator <
bool PathFinder::DFSNode::operator<(const DFSNode& other) const {
	//return this->position < other.position;


	if (this->position.x != other.position.x)
		return this->position.x < other.position.x;
	return this->position.y < other.position.y;
}

// \brief Overload of operator >
bool PathFinder::DFSNode::operator>(const DFSNode& other) const {
	return !(*this < other);
}

// \brief BestFirstNode constructor
PathFinder::BestFirstNode::BestFirstNode()
{
	DFSNode();
	gScore = fScore = std::numeric_limits<int>::max(); 
}

// \brief BestFirstNode constructor
// \param dfsNode: instance of a DFSNode class to extend
// \param gScore: int with the value of gScore
// \param gScore: int with the value of fScore
PathFinder::BestFirstNode::BestFirstNode(DFSNode dfsNode, const int gScore, const int fScore) {
	BestFirstNode(dfsNode.mazeNode, dfsNode.position, gScore, fScore);
}

// \brief BestFirstNode constructor
// \param *mazeNode: Pointer to a maze node
// \param position: Vector2 containing position of the maze node
// \param gScore: int with the value of gScore
// \param gScore: int with the value of fScore
PathFinder::BestFirstNode::BestFirstNode(MazeNode* mazeNode, const Vector2 position, const int gScore, const int fScore) {
	this->mazeNode = mazeNode;
	this->position = position;
	this->gScore = gScore;
	this->fScore = fScore;
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
	DataCollector* DC = DataCollector::getInstance();
	DC->chronoTime(DataCollector::START, DataCollector::PATH_FINDING);
	
	switch (pathFinderAlgorithm){
	case NONE: //clear the maze path
		MazeHandler::getInstance()->getMazeMap()->clearMazePath();
		break;
	case DEPTH_FIRST_SEARCH:
		findPathDFS(startPos, finishPos, mazePath, startingNode);
		break;
	case BEST_FIRST_GREEDY:
		findPathBestFirst(startPos, finishPos, mazePath, startingNode, pathFinderAlgorithm);
		break;
	case A_STAR:
		findPathBestFirst(startPos, finishPos, mazePath, startingNode, pathFinderAlgorithm);
		break;
	default:
		break;
	}

	DC->chronoTime(DataCollector::STOP, DataCollector::PATH_FINDING);
}

// \brief Find the path to a given maze using random depth first search
//
// \param startPos: Starting position for the path finder
// \param finishPos: Finishing position for the path finder
// \param *mazePath: Pointer to the mazePath instance where to save the solution path
// \param *mazeMap: Pointer to the mazeMap instance containing the maze to solve
void PathFinder::findPathDFS(const Vector2 startPos, const Vector2 finishPos, MazePath* mazePath, MazeNode* startingNode) {
	std::stack<DFSNode> recursionStack;
	
	//Choose the initial cell, mark it as path start and push it to the stack
	mazePath->setPathMapValue(startPos, MazePath::PATH);
	recursionStack.push(DFSNode(startingNode, startPos));
	mazePath->setPathMapValue(Vector2(0, 0), MazePath::PATH);

	//While the stack is not empty
	while (!recursionStack.empty()) {
		//Pop a cell from the stack and make it a current cell
		DFSNode currentNode = recursionStack.top();

		//If current cell is finish, stop path finding
		if (currentNode.position == finishPos)
			break;

		//If the current cell has any neighbours which have not been visited
		DFSNode neighbourNode = getUnexploredNeighbour(currentNode, mazePath);
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

// \brief Find the path to a given maze using a Best First algorithm
//
// \param startPos: Starting position for the path finder
// \param finishPos: Finishing position for the path finder
// \param *mazePath: Pointer to the mazePath instance where to save the solution path
// \param *mazeMap: Pointer to the mazeMap instance containing the maze to solve
void PathFinder::findPathBestFirst(const Vector2 startPos, const Vector2 finishPos, MazePath* mazePath, MazeNode* startingNode, const PathFinderAlgorithm algorithm) {
	//Create pathMap for tracking path generation
	BestFirstNode startingBFNode = BestFirstNode(startingNode, startPos, 0, getManhattanDistance(startPos, finishPos));
	std::map<BestFirstNode, BestFirstNode> pathMap{ {startingBFNode, startingBFNode} };
	
	//Priority queue setup for frontier
	auto cmp = [](const BestFirstNode& left, const BestFirstNode& right) {return left.fScore > right.fScore; };
	searchable_priority_queue<BestFirstNode, std::vector<BestFirstNode>, decltype(cmp)> frontier(cmp);

	//Adds starting node to frontier
	frontier.push(startingBFNode);

	//while frontier is not empty
	while (!frontier.empty())
	{
		//get best element in open set
		BestFirstNode bestBFNode = frontier.top();

		//if goal trace the solution path onto the pathMap
		if (bestBFNode.position == finishPos) {
			traceSolutionPath(pathMap, mazePath, startPos, bestBFNode);
			return;
		}

		//if not remove node from open set
		frontier.pop();
		mazePath->setPathMapValue(bestBFNode.position, MazePath::VISITED);

		//Get valid neighbours
		for (auto neighbour : getAllUnexploredNeighbours(bestBFNode, mazePath)) {
			//If neighbour is already inside frontier, jump to next loop
			if (frontier.check(BestFirstNode(nullptr, neighbour.position, 0, 0)))
				continue;

			//Calculate gScore of neighboutBFNode based if the algo is running an A* or Greedy version
			int gScore = getGScore(pathMap, startPos, neighbour, algorithm);

			BestFirstNode neighbourBFNode = BestFirstNode(
				neighbour.mazeNode,
				neighbour.position,
				gScore,
				gScore + getManhattanDistance(neighbour.position, finishPos)
			);

			pathMap.emplace(neighbourBFNode, bestBFNode);
			frontier.push(neighbourBFNode);
		}
	}
}

// \brief Get a non random unexplored neighbour from a given node
//
// \param mazeNode: Node from wich to get the neighbour
// \param *mazePath: Map with current path and visited nodes
//
// \return VisitedNode object rappresenting a neighbour to the passed node
PathFinder::DFSNode PathFinder::getUnexploredNeighbour(const DFSNode mazeNode, MazePath* mazePath) {
	//If there are no neighbours, returns the current mazeNode
	//if (!mazeNode.mazeNode->getNeighboursAmount())
	//	return mazeNode;

	//Get first available unvisited neighbour
	DFSNode neighbourNode;
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

// \brief Get all unexplored neighbours from a given node
//
// \param mazeNode: Node from wich to get the neighbours
// \param *mazePath: Map with current path and visited nodes
//
// \return Vector of DFSNodes objects rappresenting the unexplored neighbours
std::vector<PathFinder::DFSNode> PathFinder::getAllUnexploredNeighbours(const DFSNode dfsNode, MazePath* mazePath) {
	std::vector<DFSNode> neighbours;
	for (int i = 0; i < 4; i++) {	//4 is the magic number for maximum number of possible neighbours
		//If there is no available neighbour on direction i, jump to next loop
		MazeNode* mazeNode = dfsNode.mazeNode->getNeighbour(static_cast<Direction>(i));
		if (mazeNode == nullptr)
			continue;

		//If the neighbour has already been visited, jump to next loop
		Vector2 neighbourPosition = getNeighbourPosition(dfsNode.position, static_cast<Direction>(i));
		if (mazePath->getPathMapValue(neighbourPosition) != MazePath::NOT_VISITED)
			continue;
		
		neighbours.push_back(DFSNode(mazeNode, neighbourPosition));
	}
	return neighbours;
}

// \brief Get the manhattan distance between two vectors
//
// \param pos1: Vector2 of the first position
// \param pos2: Vector2 of the second position
//
// \return returns the distance between the two points
int PathFinder::getManhattanDistance(const Vector2 pos1, const Vector2 pos2) {
	return (abs(pos1.x - pos2.x) + abs(pos1.y - pos2.y));
}

// \brief Get the g score for a certain node
//
// \param pathMap: Map with all the paths currently knows to the algorithm
// \param node: Node of wich to calculate the gScore
//
// \return int of the gScore
int PathFinder::getGScore(const std::map<BestFirstNode, BestFirstNode>& pathMap, const Vector2 startNode, const DFSNode node, const PathFinderAlgorithm algorithm) {
	if (algorithm != A_STAR)
		return 0;
	return countPathToStart(pathMap, startNode, BestFirstNode(node, 0, 0));
}

// \brief Get the position of a neighbouring node from the nodes position
//
// \param nodePosition: Position of the node from wich to get the neighbour position
// \param direction: Direction of the neighbour
//
// \return returns the position of the neighbour inside a Vector2, in case direction is unhandled return the nodePosition unaltered
Vector2 PathFinder::getNeighbourPosition(const Vector2 nodePosition, const Direction direction) {
	switch (direction) {
	case NORTH: //NORTH
		return nodePosition + Vector2(0, -1);
	case SOUTH: //SOUTH
		return nodePosition + Vector2(0, 1);
	case EAST: //EAST
		return nodePosition + Vector2(1, 0);
	case WEST: //WEST
		return nodePosition + Vector2(-1, 0);
	default:
		return nodePosition;
	}
}

// \brief Traces the path inside of the mazePath
//
// \param startNode: Position of the starting node
// \param pathMap: Map with all the paths currently knows to the algorithm
// \param *mazePath: Map with current path and visited nodes
void PathFinder::traceSolutionPath(const std::map<BestFirstNode, BestFirstNode>& pathMap, MazePath* mazePath, const Vector2 startNode, const BestFirstNode finishNode) {
	mazePath->setPathMapValue(finishNode.position, MazePath::PATH);
	BestFirstNode pathNode = pathMap.at(finishNode);
	while (pathNode.position != startNode) {
		mazePath->setPathMapValue(pathNode.position, MazePath::PATH);
		pathNode = pathMap.at(pathNode);
	}
	mazePath->setPathMapValue(pathNode.position, MazePath::PATH);
}

// \brief Counts the amount fo nodes from a certain node to the starting node
//
// \param startNode: Position of the starting node
// \param node: Node from wich to start counting
// \param pathMap: Map with all the paths currently knows to the algorithm
//
// \return returns the number of nodes needed to reach the start from the current node
int PathFinder::countPathToStart(const std::map<BestFirstNode, BestFirstNode>& pathMap, const Vector2 startNode, const BestFirstNode node) {
	int counter = 0;
	BestFirstNode pathNode = pathMap.at(node);
	while (pathNode.position != startNode) {
		counter++;
		pathNode = pathMap.at(pathNode);
	}
	return counter;
}
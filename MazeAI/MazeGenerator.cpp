#include <time.h>
#include <algorithm>
#include <random>
#include <stack>

#include "MazeGenerator.h"

//pseudo-random number generator
std::random_device rd;
std::mt19937 g(rd());

///Neighbour struct implementation

// \brief Neighbour struct empty constructor
MazeGenerator::Neighbour::Neighbour() {
	node = nullptr;
	position = Vector2(0, 0);
	directionFromNode = NORTH;
}

// \brief Neighbour struct constructor
// 
// \param *node: Pointer to a maze node
// \param position: Position of the maze node
// \param directionFromNode: Direction in relation to a node
MazeGenerator::Neighbour::Neighbour(MazeNode* node, Vector2 position, Direction positionFromNode) {
	this->node = node;
	this->position = position;
	this->directionFromNode = positionFromNode;
}

///VisitedNode struct implementation
// \brief VisistedNode constructor
// \param *node: Pointer to the maze node
// \param position: Position of the maze node
MazeGenerator::VisitedNode::VisitedNode(MazeNode* node, const Vector2 position) {
	this->node = node;
	this->position = position;
}

///MazeGenerator class implementation
// \brief Generates a maze from a given empty map using randomized depth-first search
// 
// \param mazeMap: Pointer to an empty map
// \param startPos: Starting position for the algorithm
//
// \return bool: True if generation has been completed False otherwise
void MazeGenerator::generateMazeDFS(MazeMap* mazeMap, const Vector2 startPos) {
	//create array to store visited nodes
	//0 = not visited
	//1 = visited
	Vector2 mapSize = mazeMap->getMapSize();
	std::vector<int> mazeFogMap(mapSize.x * mapSize.y);

	//Choose the initial cell, mark it as visited and push it to the stack
	std::stack<VisitedNode> recursionStack;
	recursionStack.push(VisitedNode(
		mazeMap->getMazeNode(startPos),
		startPos
		)
	);
	mazeFogMap[mazeMap->parsePosition(startPos)] = 1;

	//While the stack is not empty
	while (!recursionStack.empty()) {
		//Pop a cell from the stack and make it a current cell
		VisitedNode currentNode = recursionStack.top();
		
		
		//If the current cell has any neighbours which have not been visited
		Neighbour neighbour = getRandomExplorableNeighbour(
			mazeMap,
			&mazeFogMap,
			currentNode.position
		);
		if (!neighbour.node) {
			recursionStack.pop();
			continue;
		}

		//Remove the wall between the current cell and the chosen cell
		linkNodes(
			mazeMap,
			&currentNode.position,
			&neighbour
		);
		
		//Mark the chosen cell as visited and push it to the stack
		mazeFogMap[mazeMap->parsePosition(neighbour.position)] = 1;
		recursionStack.push(VisitedNode(
			neighbour.node,
			neighbour.position
		));
	}
}

// \brief Generates an empty map with a given size
// 
// \param *mazeMap: Pointer to the maze map
// \param mapSize: Vector2 with map size passed by value
void MazeGenerator::generateMazeEmpty(MazeMap* mazeMap, const Vector2 mapSize) {
	mazeMap->resize(mapSize);
}

// \brief Link two MazeNodes by setting each other as neighbours
//
// \param *mazeMap:	Pointer to the maze map
// \param *mazeNodePos:	Pointer to the position of the maze node to connect to the neighbour
// \param *neighbour: Pointer of the neighbour to connect to the maze node
void MazeGenerator::linkNodes(MazeMap* mazeMap, Vector2* mazeNodePos, Neighbour* neighbour) {
	MazeNode* currentNode = mazeMap->getMazeNode(*mazeNodePos);
	MazeNode* neighbourNode = mazeMap->getMazeNode(neighbour->position);

	currentNode->setNeighbour(neighbour->directionFromNode, neighbour->node);
	neighbourNode->setNeighbour(MazeNode::getOppositeDirection(neighbour->directionFromNode), currentNode);
}

// \brief Gets the available neighbours around a maze node
//
// \param *mazeMap: Pointer to the maze map
// \param *mazeFogMap: Pointer to the fog map tracking the visited nodes
// \param nodePosition: Position of the node currently beign explored 
std::vector<MazeGenerator::Neighbour> MazeGenerator::getAvailableNeighbours(MazeMap* mazeMap, std::vector<int>* mazeFogMap, BoundedVector2 nodePosition) {
	std::vector<Neighbour> availableNeighbours;
	Vector2* neighbourPos;
	
	//gets the available neighbours around the current node
	//check if there is a north neigbour and is not explored
	if (neighbourPos = nodePosition.moveTo(NORTH))
		if (!mazeFogMap->at(mazeMap->parsePosition(*neighbourPos)))
			availableNeighbours.push_back(Neighbour(mazeMap->getMazeNode(*neighbourPos), *neighbourPos, NORTH));

	//check if there is a south neigbour and is not explored
	if (neighbourPos = nodePosition.moveTo(SOUTH))
		if (!mazeFogMap->at(mazeMap->parsePosition(*neighbourPos)))
			availableNeighbours.push_back(Neighbour(mazeMap->getMazeNode(*neighbourPos), *neighbourPos, SOUTH));

	//check if there is a east neigbour and is not explored
	if (neighbourPos = nodePosition.moveTo(EAST))
		if (!mazeFogMap->at(mazeMap->parsePosition(*neighbourPos)))
			availableNeighbours.push_back(Neighbour(mazeMap->getMazeNode(*neighbourPos), *neighbourPos, EAST));

	//check if there is a west neigbour and is not explored
	if (neighbourPos = nodePosition.moveTo(WEST))
		if (!mazeFogMap->at(mazeMap->parsePosition(*neighbourPos)))
			availableNeighbours.push_back(Neighbour(mazeMap->getMazeNode(*neighbourPos), *neighbourPos, WEST));

	return availableNeighbours;
}

// \brief Gets a random explorable neighbour to a node
//
// \param *mazeMap: Pointer to the maze map
// \param *mazeFogMap: Pointer to the fog map tracking the visited nodes
// \param nodePosition: Position of the node currently beign explored 
//
// \return Neighbour: Returns the random neighbour 
MazeGenerator::Neighbour MazeGenerator::getRandomExplorableNeighbour(MazeMap* mazeMap, std::vector<int>* fogMap, const Vector2 nodePosition) {
	BoundedVector2 boundedNodePosition = BoundedVector2(nodePosition.x, nodePosition.y);
	boundedNodePosition.setBounds(
		Vector2(0, mazeMap->getMapSize().x),
		Vector2(0, mazeMap->getMapSize().y)
	);
	//retrieve all explorable neighbours
	std::vector<Neighbour> availableNeighbours = getAvailableNeighbours(mazeMap, fogMap, boundedNodePosition);

	//if there are no expolorable neighbours return an empty neighbour
	if (!availableNeighbours.size())
		return Neighbour();

	//if there are more than one neighbour shuffle the vector for random selection
	if (availableNeighbours.size() > 1)
		std::shuffle(availableNeighbours.begin(), availableNeighbours.end(), g);

	//return the last neighbour from the vector
	return availableNeighbours.back();
}
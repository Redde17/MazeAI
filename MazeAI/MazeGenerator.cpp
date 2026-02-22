#include "MazeGenerator.h"
#include <time.h>
#include <algorithm>
#include <random>

//pseudo-random number generator
std::random_device rd;
std::mt19937 g(rd());

// \brief Generates a maze from a given empty map using randomized depth-first search
// 
// \param mazeMap pointer to an empty map
//
// \return bool: true if generation has been completed false otherwise
void MazeGenerator::generateMazeDFS(MazeMap* mazeMap, const Vector2 startPos) {
	srand(time(0));
	
	//create array to store visited nodes
	//0 = not visited
	//1 = visited
	Vector2 mapSize = mazeMap->getMapSize();
	std::vector<int> mazeFogMap(mapSize.x * mapSize.y);

	//create bounded vector2 for bounded maze movement
	BoundedVector2 boundedStartPos(startPos.x, startPos.y);
	boundedStartPos.SetBounds(
		Vector2(0, mapSize.x),
		Vector2(0, mapSize.y)
	);

	//start recurisve procedure with starting node and ending node
	exploreNodeDFS(
		&mazeFogMap,
		mazeMap,
		boundedStartPos
	);

}

// \brief Recursive function for generateMazeDFS function
//
// \param *mazeFogMap:	Reference to the fog map tracking the visited nodes
// \param *mazeMap:	Reference to the maze map
// \param currentNodePos: Position of the node currently beign explored 
void MazeGenerator::exploreNodeDFS(std::vector<int>* mazeFogMap, MazeMap* mazeMap, BoundedVector2 currentNodePos) {
	//set current node as visited
	mazeFogMap->at(mazeMap->parsePosition(currentNodePos)) = 1;

	//gets the available neighbours around the current node
	std::vector<Neighbour> availableNeighbours = getAvailableNeighbours(
		mazeMap,
		mazeFogMap,
		&availableNeighbours,
		&currentNodePos
	);

	//if no valid neighbours, return
	if (!availableNeighbours.size())
		return;

	//select a random neighbour node
	//random neighbour selection is made trough vector shuffle and pop
	std::shuffle(availableNeighbours.begin(), availableNeighbours.end(), g);

	//while there are available neighbours
	while (availableNeighbours.size() > 0) {

		//cheks if neighbour has been visited
		if (mazeFogMap->at(mazeMap->parsePosition(availableNeighbours.back().position))) {
			//if neighbour has been already visited remove it from the available neighbours and skip loop cicle
			availableNeighbours.pop_back();
			continue;
		}

		//delete wall between current node and neighbour by creating link between them
		linkNodes(
			mazeMap,
			&currentNodePos,
			&availableNeighbours.back()
		);

		//sets neighbourBoundedPos to the neihbouring node position for the next recursive call
		BoundedVector2 neighbourBoundedPos = currentNodePos;
		neighbourBoundedPos.setVector2(availableNeighbours.back().position);

		//pop the neighbour from the vector
		availableNeighbours.pop_back();

		//call recursive procedure on neighbour node
		exploreNodeDFS(
			mazeFogMap,
			mazeMap,
			neighbourBoundedPos
		);
	}
}

// \brief Link two MazeNodes by setting each other as neighbours
//
// \param *mazeMap:	reference to the maze map
// \param *mazeNodePos:	reference to the position of the maze node to connect to the neighbour
// \param *neighbour:	reference of the neighbour to connect to the maze node
void MazeGenerator::linkNodes(MazeMap* mazeMap, BoundedVector2* mazeNodePos, Neighbour* neighbour) {
	MazeNode* currentNode = mazeMap->GetMazeNode(*mazeNodePos);
	MazeNode* neighbourNode = mazeMap->GetMazeNode(neighbour->position);

	currentNode->setNeighbour(neighbour->positionFromNode, neighbour->node);
	neighbourNode->setNeighbour(MazeNode::getOppositeDirection(neighbour->positionFromNode), currentNode);
}

// \brief Gets the available neighbours around a maze node
//
// \param *mazeMap: Reference to the maze map
// \param *mazeFogMap: Reference to the fog map tracking the visited nodes
// \param *currentNodePos: Position of the node currently beign explored 
std::vector<MazeGenerator::Neighbour> MazeGenerator::getAvailableNeighbours(MazeMap* mazeMap, std::vector<int>* mazeFogMap, BoundedVector2* currentNodePos) {
	std::vector<Neighbour> availableNeighbours;
	Vector2* neighbourPos;
	
	//gets the available neighbours around the current node
	//check if there is a north neigbour and is not explored
	if (neighbourPos = currentNodePos->moveTo(NORTH))
		if (!mazeFogMap->at(mazeMap->parsePosition(*neighbourPos)))
			availableNeighbours.push_back(Neighbour(mazeMap->GetMazeNode(*neighbourPos), *neighbourPos, NORTH));

	//check if there is a south neigbour and is not explored
	if (neighbourPos = currentNodePos->moveTo(SOUTH))
		if (!mazeFogMap->at(mazeMap->parsePosition(*neighbourPos)))
			availableNeighbours.push_back(Neighbour(mazeMap->GetMazeNode(*neighbourPos), *neighbourPos, SOUTH));

	//check if there is a east neigbour and is not explored
	if (neighbourPos = currentNodePos->moveTo(EAST))
		if (!mazeFogMap->at(mazeMap->parsePosition(*neighbourPos)))
			availableNeighbours.push_back(Neighbour(mazeMap->GetMazeNode(*neighbourPos), *neighbourPos, EAST));

	//check if there is a west neigbour and is not explored
	if (neighbourPos = currentNodePos->moveTo(WEST))
		if (!mazeFogMap->at(mazeMap->parsePosition(*neighbourPos)))
			availableNeighbours.push_back(Neighbour(mazeMap->GetMazeNode(*neighbourPos), *neighbourPos, WEST));

	return availableNeighbours;
}
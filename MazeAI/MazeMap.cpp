#include "MazeMap.h"
#include "BoundedVector2.h"

///MAZEMAP IMPLEMENTATION///
// \brief creates an empty Maze with all the walls
// 
// \param mapSize	Defines the width and height of the map
MazeMap::MazeMap(const Vector2 mapSize) {
	this->mapSize = mapSize;

	// Creates a connectionless vector of nodes
	map.resize(mapSize.x * mapSize.y);

	mazePath = new MazePath(mapSize);
}

MazeMap::~MazeMap() {
	delete mazePath;
}

// \brief Get the pointer to the vector storing the map
// \return Pointer to the map vector
std::vector<MazeNode>* MazeMap::getMap() {
	return &map;
}

// \brief Get the Vector2 containing the map size
// \return Vector2 with map size
Vector2 MazeMap::getMapSize() {
	return mapSize;
}

// \brief Get a pointer to the MazePath isntance
// \return pointer to MazePath with pathing data
MazePath* MazeMap::getMazePath() {
	return mazePath;
}

// \brief Get a specific node on the map
// 
// Gets a specific node on the map with a position 
// specified with a position passed by reference.
// Vector2* position will be deleted from memory
// if position should not be deleted pass by value.
// 
// \param position	Position of the node to get
// 
// \return Pointer to a MazeNode in the map
MazeNode* MazeMap::getMazeNode(const Vector2* position) {
	return &map[parsePosition(position)];
}

// \brief Get a specific node on the map
// 
// Gets a specific node on the map with a position 
// specified with a position passed by value.
// Vector2 position is not deleted
// 
// \param position	Position of the node to get
// 
// \return Pointer to a MazeNode in the map
MazeNode* MazeMap::getMazeNode(const Vector2 position) {
	return &map[parsePosition(position)];
}

// \brief Set a specific node on the map
// 
// \param position	position of the node to set
// 
// \param newMazeNode
void MazeMap::setMazeNode(const Vector2 position, MazeNode newMazeNode) {
	map[parsePosition(position)] = newMazeNode;
}

// \brief Removes all the walls currently in the maze
void MazeMap::removeWalls() {
	//Connects all nodes between eachother to create a planar graph
	//TODO: Multithread implementation

	//Bounded vector2 for cheking bounds of the node while retrieving neighboars
	BoundedVector2* boundedPos = new BoundedVector2(0, 0);
	boundedPos->setBounds(Vector2(0, mapSize.x), Vector2(0, mapSize.y));
	Vector2* neighbourNodePosition;
	MazeNode* neighbourNode;

	//for each neighbour, retrieve it if possible and create connection with it
	//otherwise set neighbour to nullptr
	for (int x = 0; x < mapSize.x; x++) {
		for (int y = 0; y < mapSize.y; y++) {
			boundedPos->setVector2(x, y);

			//Check north
			if (neighbourNodePosition = boundedPos->moveTo(NORTH))
				neighbourNode = &map[parsePosition(neighbourNodePosition)];
			else
				neighbourNode = nullptr;

			map[parsePosition(Vector2(x, y))].setNeighbour(NORTH, neighbourNode);
			//Check sud
			if (neighbourNodePosition = boundedPos->moveTo(SOUTH))
				neighbourNode = &map[parsePosition(neighbourNodePosition)];
			else
				neighbourNode = nullptr;
			map[parsePosition(Vector2(x, y))].setNeighbour(SOUTH, neighbourNode);

			//Check east
			if (neighbourNodePosition = boundedPos->moveTo(EAST))
				neighbourNode = &map[parsePosition(neighbourNodePosition)];
			else
				neighbourNode = nullptr;
			map[parsePosition(Vector2(x, y))].setNeighbour(EAST, neighbourNode);

			//Check west
			if (neighbourNodePosition = boundedPos->moveTo(WEST))
				neighbourNode = &map[parsePosition(neighbourNodePosition)];
			else
				neighbourNode = nullptr;
			map[parsePosition(Vector2(x, y))].setNeighbour(WEST, neighbourNode);
		}
	}
}

// \brief Clears the current MazePath
void MazeMap::clearMazePath() {
	delete mazePath;
	mazePath = new MazePath(mapSize);
}

// \brief Resizes the current map and clears it
// \param newSize: New size for the maze map
void MazeMap::resize(const Vector2 newSize) {
	//Update map size
	mapSize = newSize;

	//Clear map vector, and resize it
	map.clear();
	map.resize(mapSize.x * mapSize.y);

	//Create new MazePath with new size
	delete mazePath;
	mazePath = new MazePath(newSize);
}

// \brief Parses (x, y) coordinates into single value for map vector
// 
// This function should be used when trying to access any node on the maze map.
// 
// 
// position is passed by reference.
// Vector2* position will be deleted from memory
// if position should not be deleted pass by value.
// The parsing is bounded by the map size, if the coordinates
// exceed the map bounds returns -1
// 
// \param position	Value of the position to parse
int MazeMap::parsePosition(const Vector2* position) {
	if (!validPosition(*position))
		return -1;

	int parsedPos = position->x * mapSize.y + position->y;

	//Position is deleted to avoid memory leaks when passing
	//to the function new Vector2 position	
	delete position;
	return parsedPos;
}

// \brief Parses (x, y) coordinates into single value for map vector
//
// This function should be used when trying to access any node on the maze map.
// 
// The parsing is bounded by the map size, if the coordinates
// exceed the map bounds returns -1
//
// \param position	Value of the position to parse
int MazeMap::parsePosition(const Vector2 position) {
	if (!validPosition(position))
		return -1;

	return position.x * mapSize.y + position.y;
}


// \brief Cheks if position is not out of bound
// \param position Vector2 containing (x, y) coordinates for the position
bool MazeMap::validPosition(const Vector2 position) {
	//if either of the coordinates is less then 0 or more the the max mapSize
	//return false
	if (position.x < 0 || position.x > mapSize.x)
		return false;
	if (position.y < 0 || position.y > mapSize.y)
		return false;
	return true;
}

///DEBUG FUNCTIONS

// \brief Get a map of neighbours for each node
// 
// The map returned is a vector of numbers that rapresents the
// amount of neighbours for each node
// 
// \return std::vector of ints
std::vector<int> MazeMap::getNeighboursMap() {
	std::vector<int> neighboursMap;
	int nodeNeighbours;
	//For each node in the maze map, count the amount of neighbours
	//the resulting amount is then inserted in the neighbours map
	for (int x = 0; x < mapSize.x; x++) {
		for (int y = 0; y < mapSize.y; y++) {
			//gets the amount of neighbours of the nove (i, j)
			nodeNeighbours = getMazeNode(Vector2(x, y))->getNeighboursAmount();
			//insert the value into the neighbours map in position (i, j)
			neighboursMap.push_back(nodeNeighbours);
		}
	}

	return neighboursMap;
}

// \brief Get a map of the maze 
// 
// The map returned is a vector of numbers that rapresents the
// maze.
// 1 means wall 0 means empty space.
// 
// \return std::vector of ints
std::vector<int> MazeMap::getMazeMap() {
	int mapOffset = 3;
	std::vector<int> mazeMap(mapSize.x * mapSize.y * 9);
	MazeNode* mazeNode = nullptr;

	int mapSizeY = mapSize.y;
	auto parseLocalMazeMapPosition = [mapSizeY](const Vector2 position) {
		return position.x * (mapSizeY * 3) + position.y;
	};

	//for each node of the map, create a 3x3 matrix inside vector<int> mazeMap
	//that rappresents the node paths and walls to the neighbours
	for (int x = 0; x < mapSize.x; x++) {
		for (int y = 0; y < mapSize.y; y++) {
			mazeNode = &map[parsePosition(Vector2(x, y))];

			//generate node array 
			//first row
			mazeMap[parseLocalMazeMapPosition(Vector2(x * mapOffset, y * mapOffset))] = 1;
			mazeMap[parseLocalMazeMapPosition(Vector2(x * mapOffset + 1, y * mapOffset))] = (mazeNode->getNeighbour(NORTH)) ? 0 : 1;
			mazeMap[parseLocalMazeMapPosition(Vector2(x * mapOffset + 2, y * mapOffset))] = 1;

			//second row
			mazeMap[parseLocalMazeMapPosition(Vector2(x * mapOffset, y * mapOffset + 1))] = (mazeNode->getNeighbour(WEST)) ? 0 : 1;
			mazeMap[parseLocalMazeMapPosition(Vector2(x * mapOffset + 1, y * mapOffset + 1))] = 0;
			mazeMap[parseLocalMazeMapPosition(Vector2(x * mapOffset + 2, y * mapOffset + 1))] = (mazeNode->getNeighbour(EAST)) ? 0 : 1;

			//third row
			mazeMap[parseLocalMazeMapPosition(Vector2(x * mapOffset, y * mapOffset + 2))] = 1;
			mazeMap[parseLocalMazeMapPosition(Vector2(x * mapOffset + 1, y * mapOffset + 2))] = (mazeNode->getNeighbour(SOUTH)) ? 0 : 1;
			mazeMap[parseLocalMazeMapPosition(Vector2(x * mapOffset + 2, y * mapOffset + 2))] = 1;
		}
	}
	
	return mazeMap;
}
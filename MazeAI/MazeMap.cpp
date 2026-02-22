#include "MazeMap.h"

///VECTOR2 IMPLEMENTATION///

void Vector2::setX(const int x) {
	this->x = x;
}

void Vector2::setY(const int y) {
	this->y = y;
}

void Vector2::setVector2(const int x, const int y) {
	this->x = x;
	this->y = y;
}

void Vector2::setVector2(Vector2 pos) {
	x = pos.x;
	y = pos.y;
}

// \brief Overload of operator =
void Vector2::operator=(const Vector2 other) {
	this->x = other.x;
	this->y = other.y;
}

///BOUNDED VECTOR2 IMPLEMENTATION///

// \brief Sets the bounds for the vector
// 
// The bounds are limits to where the vector can move
// Lower bound is x and upper bound is y
// 
// \param xBounds	Bounds for the x axis
// \param yBounds	Bounds for the y axis
void BoundedVector2::SetBounds(Vector2 xBounds, Vector2 yBounds) {
	this->xBounds = xBounds;
	this->yBounds = yBounds;
}

// \brief Creates a new vector moved to a specified direction
// 
// \param direction Value that indicates the direction of movement for the vector
// 
// \return New moved vector
Vector2* BoundedVector2::moveTo(Direction direction) {
	if (!canMoveTo(direction))
		return nullptr;

	switch (direction) {
	case NORTH:
		return new Vector2(x, y - 1);

	case SOUTH:
		return new Vector2(x, y + 1);

	case EAST:
		return new Vector2(x + 1, y);

	case WEST:
		return new Vector2(x - 1, y);

	default:
		return nullptr;
	}
}

// \brief Checks if the vector can move to the specified direction
// 
// \param direction Value that indicates the direction of movement for the vector
// 
// \return bool Result of the check
bool BoundedVector2::canMoveTo(Direction direction) {
	switch (direction) {
		case NORTH:
			if ((y - 1) < yBounds.x || (y - 1) > yBounds.y - 1)
				return false;
			break;

		case SOUTH:
			if ((y + 1) < yBounds.x || (y + 1) > yBounds.y - 1)
				return false;
			break;

		case EAST:
			if ((x + 1) < xBounds.x || (x + 1) > xBounds.y - 1)
				return false;
			break;

		case WEST:
			if ((x - 1) < xBounds.x || (x - 1) > xBounds.y - 1)
				return false;
			break;

		default:
			return false;
	}
	return true;
}

// \brief Overload of operator =
void BoundedVector2::operator=(const BoundedVector2 other) {
	this->x = other.x;
	this->y = other.y;
	this->xBounds = other.xBounds;
	this->yBounds = other.yBounds;
}

///MAZEMAP IMPLEMENTATION///
// \brief creates an empty Maze with all the walls
// 
// \param mapSize	Defines the width and height of the map
MazeMap::MazeMap(const Vector2 mapSize) : mapSize_(mapSize) {
	// Creates a connectionless vector of nodes
	for (size_t i = 0; i < mapSize_.x * mapSize_.y; i++)
		map_.push_back(new MazeNode());
}

// \brief Get the pointer to the vector storing the map
// \return Pointer to the map vector
std::vector<MazeNode*>* MazeMap::GetMap() {
	return &map_;
}

// \brief Get the Vector2 containing the map size
// \return Vector2 with map size
Vector2 MazeMap::getMapSize() {
	return mapSize_;
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
MazeNode* MazeMap::GetMazeNode(const Vector2* position) {
	MazeNode* mazeNode = map_[parsePosition(position)];
	return mazeNode;
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
MazeNode* MazeMap::GetMazeNode(const Vector2 position) {
	return map_[parsePosition(position)];
}

// \brief Set a specific node on the map
// 
// \param position	position of the node to set
// 
// \param newMazeNode
void MazeMap::SetMapNode(const Vector2 position, MazeNode* newMazeNode) {
	map_[parsePosition(position)] = newMazeNode;
}

// \brief Removes all the walls currently in the maze
void MazeMap::removeWalls() {
	//Connects all nodes between eachother to create a planar graph
	//TODO: Multithread implementation

	//Bounded vector2 for cheking bounds of the node while retrieving neighboars
	BoundedVector2* boundedPos = new BoundedVector2(0, 0);
	boundedPos->SetBounds(Vector2(0, mapSize_.x), Vector2(0, mapSize_.y));
	Vector2* neighbourNodePosition;
	MazeNode* neighbourNode;

	//for each neighbour, retrieve it if possible and create connection with it
	//otherwise set neighbour to nullptr
	for (size_t x = 0; x < mapSize_.x; x++) {
		for (size_t y = 0; y < mapSize_.y; y++) {
			boundedPos->setVector2(x, y);

			//Check north
			if (neighbourNodePosition = boundedPos->moveTo(NORTH))
				neighbourNode = map_[parsePosition(neighbourNodePosition)];
			else
				neighbourNode = nullptr;

			map_[parsePosition(Vector2(x, y))]->setNeighbour(NORTH, neighbourNode);
			//Check sud
			if (neighbourNodePosition = boundedPos->moveTo(SOUTH))
				neighbourNode = map_[parsePosition(neighbourNodePosition)];
			else
				neighbourNode = nullptr;
			map_[parsePosition(Vector2(x, y))]->setNeighbour(SOUTH, neighbourNode);

			//Check east
			if (neighbourNodePosition = boundedPos->moveTo(EAST))
				neighbourNode = map_[parsePosition(neighbourNodePosition)];
			else
				neighbourNode = nullptr;
			map_[parsePosition(Vector2(x, y))]->setNeighbour(EAST, neighbourNode);

			//Check west
			if (neighbourNodePosition = boundedPos->moveTo(WEST))
				neighbourNode = map_[parsePosition(neighbourNodePosition)];
			else
				neighbourNode = nullptr;
			map_[parsePosition(Vector2(x, y))]->setNeighbour(WEST, neighbourNode);
		}
	}
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

	int parsedPos = position->x * mapSize_.y + position->y;

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

	return position.x * mapSize_.y + position.y;
}


// \brief Cheks if position is not out of bound
// \param position Vector2 containing (x, y) coordinates for the position
bool MazeMap::validPosition(const Vector2 position) {
	//if either of the coordinates is less then 0 or more the the max mapSize
	//return false
	if (position.x < 0 || position.x > mapSize_.x)
		return false;
	if (position.y < 0 || position.y > mapSize_.y)
		return false;
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
	for (size_t x = 0; x < mapSize_.x; x++) {
		for (size_t y = 0; y < mapSize_.y; y++) {
			//gets the amount of neighbours of the nove (i, j)
			nodeNeighbours = GetMazeNode(Vector2(x, y))->getNeighboursAmount();
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
	std::vector<int> mazeMap(mapSize_.x * mapSize_.y * 9);
	MazeNode* mazeNode = nullptr;

	int mapSizeY = mapSize_.y;
	auto parseLocalMazeMapPosition = [mapSizeY](const Vector2 position) {
		return position.x * (mapSizeY * 3) + position.y;
	};

	//for each node of the map, create a 3x3 matrix inside vector<int> mazeMap
	//that rappresents the node paths and walls to the neighbours
	for (size_t x = 0; x < mapSize_.x; x++) {
		for (size_t y = 0; y < mapSize_.y; y++) {
			mazeNode = map_[parsePosition(Vector2(x, y))];

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
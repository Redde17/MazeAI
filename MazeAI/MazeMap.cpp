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

///BOUNDLESS VECTOR2 IMPLEMENTATION///
// \brief Sets the bounds for the vector
// 
// The bounds are limits to where the vector can move
// Lower bound is x and upper bound is y
// 
// \param xBounds	Bounds for the x axis
// \param yBounds	Bounds for the y axis
void BoundedVector2::SetBounds(Vector2* xBounds, Vector2* yBounds) {
	this->xBounds = xBounds;
	this->yBounds = yBounds;
}


// \brief Creates a new vector moved by (x, y - 1)
// \return New moved vector
Vector2* BoundedVector2::moveNorth() {
	if (canMoveNorth())
		return new Vector2(x, y - 1);
	return nullptr;
}

// \brief Creates a new vector moved by (x, y + 1)
// \return New moved vector
Vector2* BoundedVector2::moveSouth() {
	if (canMoveSouth())
		return new Vector2(x, y + 1);
	return nullptr;
}

// \brief Creates a new vector moved by (x + 1, y)
// \return New moved vector
Vector2* BoundedVector2::moveEast() {
	if (canMoveEast())
		return new Vector2(x + 1, y);
	return nullptr;
}

// \brief Creates a new vector moved by (x - 1, y)
// \return New moved vector
Vector2* BoundedVector2::moveWest() {
	if (canMoveWest())
		return new Vector2(x - 1, y);
	return nullptr;
}

// \brief Checks if the vector can move by (x, y - 1)
// \return bool Result of the check
bool BoundedVector2::canMoveNorth() {
	if ((y - 1) < yBounds->x || (y - 1) > yBounds->y - 1)
		return false;
	return true;
}

// \brief Checks if the vector can move by (x, y + 1)
// \return bool Result of the check
bool BoundedVector2::canMoveSouth() {
	if ((y + 1) < yBounds->x || (y + 1) > yBounds->y - 1)
		return false;
	return true;
}

// \brief Checks if the vector can move by (x + 1, y)
// \return bool Result of the check
bool BoundedVector2::canMoveEast() {
	if ((x + 1) < xBounds->x || (x + 1) > xBounds->y - 1)
		return false;
	return true;
}

// \brief Checks if the vector can move by (x - 1, y)
// \return bool Result of the check
bool BoundedVector2::canMoveWest() {
	if ((x - 1) < xBounds->x || (x - 1) > xBounds->y - 1)
		return false;
	return true;
}

///MAZEMAP IMPLEMENTATION///
// \brief creates an empty Maze without walls
// 
// \param mapSize	Defines the width and height of the map
MazeMap::MazeMap(const Vector2 mapSize) : mapSize_(mapSize) {
	// Creates a connectionless vector of nodes
	for (size_t i = 0; i < mapSize_.x * mapSize_.y; i++)
		map_.push_back(new MazeNode());

	//Connects all nodes between eachother to create a planar graph
	//TODO: Multithread implementation

	//Bounded vector2 for cheking bounds of the node while retrieving neighboars
	BoundedVector2* boundedPos = new BoundedVector2(0, 0);
	boundedPos->SetBounds(new Vector2(0, mapSize_.x), new Vector2(0, mapSize_.y));
	Vector2* neighbourNodePosition;
	MazeNode* neighbourNode;
	
	//for each neighbour, retrieve it if possible and create connection with it
	//otherwise set neighbour to nullptr
	for (size_t x = 0; x < mapSize_.x; x++) {
		for (size_t y = 0; y < mapSize_.y; y++) {
			boundedPos->setVector2(x, y);
			
			//Check north
			if (neighbourNodePosition = boundedPos->moveNorth())
				neighbourNode = map_[parsePosition(neighbourNodePosition)];
			else
				neighbourNode = nullptr;
			
			map_[parsePosition(new Vector2(x, y))]->setNeighbour(NORTH, neighbourNode);
			//Check sud
			if (neighbourNodePosition = boundedPos->moveSouth())
				neighbourNode = map_[parsePosition(neighbourNodePosition)];
			else 
				neighbourNode = nullptr;
			map_[parsePosition(new Vector2(x, y))]->setNeighbour(SOUTH, neighbourNode);

			//Check east
			if (neighbourNodePosition = boundedPos->moveEast())
				neighbourNode = map_[parsePosition(neighbourNodePosition)];
			else
				neighbourNode = nullptr;
			map_[parsePosition(new Vector2(x, y))]->setNeighbour(EAST, neighbourNode);

			//Check west
			if (neighbourNodePosition = boundedPos->moveWest())
				neighbourNode = map_[parsePosition(neighbourNodePosition)];
			else 
				neighbourNode = nullptr;
			map_[parsePosition(new Vector2(x, y))]->setNeighbour(WEST, neighbourNode);
		}
	}

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

// \brief Parses (x, y) coordinates into single value for map vector
// 
// position is passed by reference.
// Vector2* position will be deleted from memory
// if position should not be deleted pass by value.
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
std::vector<int> MazeMap::GetNeighboursMap() {
	std::vector<int> neighboursMap;
	int nodeNeighbours;
	//For each node in the maze map, count the amount of neighbours
	//the resulting amount is then inserted in the neighbours map
	for (size_t x = 0; x < mapSize_.x; x++) {
		for (size_t y = 0; y < mapSize_.y; y++) {
			//gets the amount of neighbours of the nove (i, j)
			nodeNeighbours = GetMazeNode(new Vector2(x, y))->getNeighboursAmount();
			//insert the value into the neighbours map in position (i, j)
			neighboursMap.push_back(nodeNeighbours);
		}
	}

	return neighboursMap;
}
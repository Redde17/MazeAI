#include "MazePath.h"

// \brief MazePath default constructor
MazePath::MazePath() {
	this->mapSize = Vector2(0, 0);
	mazePathMap = std::vector<int>(1, NOT_VISITED);
}

// \brief MazePath constructor
MazePath::MazePath(Vector2 mapSize) {
	this->mapSize = mapSize;
	mazePathMap = std::vector<int>(mapSize.x * mapSize.y, NOT_VISITED);
}

// \brief Getter for mazePathMap
//
// \param position: Position of the value to retrieve
//
// \return Value of the requested map position
int MazePath::getPathMapValue(Vector2 position) {
	return mazePathMap[position.x * mapSize.y + position.y];
}

// \brief Setter for mazePathMap
// \param position: Position of the value to set
// \param value: Value to set
void MazePath::setPathMapValue(Vector2 position, PathValue pathValue) {
	mazePathMap[position.x * mapSize.y + position.y] = pathValue;
}
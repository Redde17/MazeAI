#pragma once
#include <vector>

#include "Vector2.h"

//Class for maze solution path rappresentation and handling
class MazePath {
public:
	enum PathValue {
		NOT_VISITED = -1,
		VISITED = 0,
		PATH = 1
	};
private:
	Vector2 mapSize;
	std::vector<int> mazePathMap;

	//int pathCounter;
	//std::vector<Vector2> mazePath;
public:
	// \brief MazePath default constructor
	MazePath();
	// \brief MazePath constructor
	MazePath(const Vector2 mapSize);

	// \brief Getter for mazePathMap
	//
	// \param position: Position of the value to retrieve
	//
	// \return Value of the requested map position
	int getPathMapValue(const  Vector2 position);

	// \brief Setter for mazePathMap
	// \param position: Position of the value to set
	// \param value: Value to set
	void setPathMapValue(const  Vector2 position, const PathValue pathValue);
};


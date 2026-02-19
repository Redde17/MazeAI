#pragma once
#include <vector>

#include "MazeNode.h"

struct Vector2 {
	int x;
	int y;

	Vector2(const int x, const int y) {
		this->x = x;
		this->y = y;
	}
	
	//Setters
	void setX(const int x);
	void setY(const int y);
	void setVector2(const int x, const int y);
	void setVector2(Vector2 pos);
};

struct BoundedVector2 : Vector2 {
	Vector2* xBounds;
	Vector2* yBounds;

	using Vector2::Vector2;
	~BoundedVector2();

	// \brief Sets the bounds for the vector
	// 
	// The bounds are limits to where the vector can move
	// Lower bound is x and upper bound is y
	// 
	// \param xBounds	Bounds for the x axis
	// \param yBounds	Bounds for the y axis
	void SetBounds(Vector2* xBounds, Vector2* yBounds);

	// \brief Creates a new vector moved to a specified direction
	// 
	// \param direction Value that indicates the direction of movement for the vector
	// 
	// \return New moved vector
	Vector2* moveTo(Neighbour direction);

	// \brief Checks if the vector can move to the specified direction
	// 
	// \param direction Value that indicates the direction of movement for the vector
	// 
	// \return bool Result of the check
	bool canMoveTo(Neighbour direction);
};

class MazeMap {
private:
	const Vector2 mapSize_;			//Vector2 containing the map size
	std::vector<MazeNode*> map_;	//Map of the maze

public:
	// \brief creates an empty Maze
	// \param mapSize	Defines the width and height of the map
	MazeMap(const Vector2 mapSize);

	// \brief Get the pointer to the vector storing the map
	// \return Pointer to the map vector
	std::vector<MazeNode*>* GetMap();	

	// \brief Get the Vector2 containing the map size
	// \return Vector2 with map size
	Vector2 getMapSize();

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
	MazeNode* GetMazeNode(const Vector2* position);

	// \brief Get a specific node on the map
	// 
	// Gets a specific node on the map with a position 
	// specified with a position passed by value.
	// Vector2 position is not deleted
	// 
	// \param position	Position of the node to get
	// 
	// \return Pointer to a MazeNode in the map
	MazeNode* GetMazeNode(const Vector2 position);

	// \brief Set a specific node on the map
	// 
	// \param position	position of the node to set
	// 
	// \param newMazeNode
	void SetMapNode(const Vector2 position, MazeNode* newMazeNode);

	// \brief Removes all the walls currently in the maze
	void removeWalls();

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
	int parsePosition(const Vector2* position);

	// \brief Parses (x, y) coordinates into single value for map vector
	//
	// This function should be used when trying to access any node on the maze map.
	// 
	// The parsing is bounded by the map size, if the coordinates
	// exceed the map bounds returns -1
	//
	// \param position	Value of the position to parse
	int parsePosition(const Vector2 position);

	// \brief Cheks if position is not out of bound
	// \param position Vector2 containing (x, y) coordinates for the position
	bool validPosition(const Vector2 position);

	///DEBUG FUNCTIONS

	// \brief Get a map of neighbours for each node
	// 
	// The map returned is a vector of numbers that rapresents the
	// amount of neighbours for each node
	// 
	// \return std::vector of ints
	std::vector<int> getNeighboursMap();

	// \brief Get a map of the maze 
	// 
	// The map returned is a vector of numbers that rapresents the
	// maze.
	// 1 means wall 0 means empty space.
	// 
	// \return std::vector of ints
	std::vector<int> getMazeMap();
};


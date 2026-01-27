#pragma once
#include <vector>

#include "MazeNode.h"

struct Vector2 {
	int x;
	int y;
};

class MazeMap {
private:
	const Vector2 mapSize_;		//Vector2 containing the map size
	std::vector<MazeNode*> map;	//Map of the maze

public:
	// \brief creates an empty Maze
	// 
	// \param mapSize	Defines the width and height of the map
	MazeMap(const Vector2 mapSize);

	// \brief Get the pointer to the vector storing the map
	// 
	// \return Pointer to the map vector
	std::vector<MazeNode*>* GetMap();	

	// \brief Get the Vector2 containing the map size
	// 
	// \return Vector2 with map size
	Vector2 getMapSize();

	// \brief Get a specific node on the map
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
};


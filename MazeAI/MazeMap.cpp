#include "MazeMap.h"

// \brief creates an empty Maze
// 
// \param mapSize	Defines the width and height of the map
MazeMap::MazeMap(const Vector2 mapSize) : mapSize_(mapSize) {
	for (int i = 0; i < mapSize_.x * mapSize_.y; i++)
		map.push_back(new MazeNode());
}

// \brief Get the pointer to the vector storing the map
// 
// \return Pointer to the map vector
std::vector<MazeNode*>* MazeMap::GetMap() {
	return &map;
}

// \brief Get the Vector2 containing the map size
// 
// \return Vector2 with map size
Vector2 MazeMap::getMapSize() {
	return mapSize_;
}

// \brief Get a specific node on the map
// 
// \param position	Position of the node to get
// 
// \return Pointer to a MazeNode in the map
MazeNode* MazeMap::GetMazeNode(const Vector2 position) {
	return map[position.x * position.y];
}

// \brief Set a specific node on the map
// 
// \param position	position of the node to set
// 
// \param newMazeNode
void MazeMap::SetMapNode(const Vector2 position, MazeNode* newMazeNode) {
	map[position.x * mapSize_.y + position.y] = newMazeNode;
}
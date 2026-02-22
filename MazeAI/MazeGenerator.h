#pragma once
#include "MazeMap.h"

//Generation class containing all maze generation functions 
class MazeGenerator {
private:
	struct Neighbour {
		MazeNode* node;
		Vector2 position;
		Direction positionFromNode;

		Neighbour(MazeNode* node, Vector2 position, Direction positionFromNode) {
			this->node = node;
			this->position = position;
			this->positionFromNode = positionFromNode;
		}
	};

public:
	// \brief Generates a maze from a given empty map using randomized depth-first search
	// 
	// \param mazeMap: Reference to an empty map
	//
	// \return bool: True if generation has been completed False otherwise
	static void generateMazeDFS(MazeMap* mazeMap, const Vector2 startPos);

private:
	// \brief Recursive function for generateMazeDFS function
	//
	// \param *mazeFogMap:	Reference to the fog map tracking the visited nodes
	// \param *mazeMap:	Reference to the maze map
	// \param currentNodePos: Position of the node currently beign explored 
	static void exploreNodeDFS(std::vector<int>* mazeFogMap, MazeMap* mazeMap, BoundedVector2 currentNodePos);
	
	// \brief Link two MazeNodes by setting each other as neighbours
	//
	// \param *mazeMap:	Reference to the maze map
	// \param *mazeNodePos:	Reference to the position of the maze node to connect to the neighbour
	// \param *neighbour: Reference of the neighbour to connect to the maze node
	static void linkNodes(MazeMap* mazeMap, BoundedVector2* mazeNodePos, Neighbour* neighbour);

	// \brief Gets the available neighbours around a maze node
	//
	// \param *mazeMap: Reference to the maze map
	// \param *mazeFogMap: Reference to the fog map tracking the visited nodes
	// \param *currentNodePos: Position of the node currently beign explored 
	static std::vector<Neighbour> getAvailableNeighbours(MazeMap* mazeMap, std::vector<int>* mazeFogMap, BoundedVector2* currentNodePos);
};


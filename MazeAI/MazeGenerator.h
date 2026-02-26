#pragma once
#include "MazeMap.h"

//Generation class containing all maze generation functions 
class MazeGenerator {
private:

	// Struct to rappresent a neighbour to a node with track of it's position and direction
	// from said node
	struct Neighbour {
		MazeNode* node;
		Vector2 position;
		Direction directionFromNode;

		// \brief Neighbour struct empty constructor
		Neighbour();

		// \brief Neighbour struct constructor
		// 
		// \param *node: Pointer to a maze node
		// \param position: Position of the maze node
		// \param directionFromNode: Direction in relation to a node
		Neighbour(MazeNode* node, Vector2 position, Direction positionFromNode);
	};

	// Struct to rappresent a visited node with his available neighbours to explore
	struct VisitedNode {
		MazeNode* node;
		Vector2 position;

		// \brief VisistedNode constructor
		// 
		// \param *node: Pointer to the maze node
		// \param position: Position of the maze node
		VisitedNode(MazeNode* node, const Vector2 position);
	};

public:
	// \brief Generates a maze from a given empty map using randomized depth-first search
	// 
	// \param mazeMap: Pointer to an empty map
	// \param startPos: Starting position for the algorithm
	//
	// \return bool: True if generation has been completed False otherwise
	static void generateMazeDFS(MazeMap* mazeMap, const Vector2 startPos);

	// \brief Generates an empty map with a given size
	// 
	// \param *mazeMap: Pointer to the maze map
	// \param mapSize: Vector2 with map size passed by value
	static void generateMazeEmpty(MazeMap* mazeMap, const Vector2 mapSize);

private:
	// \brief Link two MazeNodes by setting each other as neighbours
	//
	// \param *mazeMap:	Pointer to the maze map
	// \param *mazeNodePos:	Pointer to the position of the maze node to connect to the neighbour
	// \param *neighbour: Pointer of the neighbour to connect to the maze node
	static void linkNodes(MazeMap* mazeMap, Vector2* mazeNodePos, Neighbour* neighbour);

	// \brief Gets the available neighbours around a maze node
	//
	// \param *mazeMap: Pointer to the maze map
	// \param *mazeFogMap: Pointer to the fog map tracking the visited nodes
	// \param nodePosition: Position of the node currently beign explored 
	static std::vector<Neighbour> getAvailableNeighbours(MazeMap* mazeMap, std::vector<int>* mazeFogMap, const BoundedVector2 nodePosition);

	// \brief Gets a random explorable neighbour to a node
	//
	// \param *mazeMap: Pointer to the maze map
	// \param *mazeFogMap: Pointer to the fog map tracking the visited nodes
	// \param nodePosition: Position of the node currently beign explored 
	//
	// \return Neighbour: Returns the random neighbour 
	static Neighbour getRandomExplorableNeighbour(MazeMap* mazeMap, std::vector<int>* mazeFogMap, const Vector2 nodePosition);
};


#pragma once
#include "MazePath.h"
#include <map>

class PathFinder {
private:
	struct VisitedNode {
		MazeNode* mazeNode;
		Vector2 position;

		// \brief VisitedNode constructor
		VisitedNode();

		// \brief VisitedNode constructor
		// \param *mazeNode: Pointer to a maze node
		// \param position: Vector2 containing position of the maze node
		VisitedNode(MazeNode* mazeNode, const Vector2 position);

		// \brief Overload of operator =
		void operator=(const VisitedNode& other);
		// \brief Overload of operator ==
		bool operator==(const VisitedNode& rhs);
	};

public:
	enum PathFinderAlgorithm {
		DEPTH_FIRST_SEARCH
	};

	// \brief Find the path to a given maze
	//
	// \param pathFinderAlgorithm: Algorithm type to use for path search
	// \param startPos: Starting position for the path finder
	// \param finishPos: Finishing position for the path finder
	// \param *mazePath: Pointer to the mazePath instance where to save the solution path
	// \param *mazeMap: Pointer to the mazeMap instance containing the maze to solve
	static void findPath(const PathFinderAlgorithm pathFinderAlgorithm, const Vector2 startPos, const Vector2 finishPos, MazePath* mazePath, MazeNode* startingNode);

private:
	// \brief Find the path to a given maze using random depth first search
	//
	// \param startPos: Starting position for the path finder
	// \param finishPos: Finishing position for the path finder
	// \param *mazePath: Pointer to the mazePath instance where to save the solution path
	// \param *mazeMap: Pointer to the mazeMap instance containing the maze to solve
	static void findPathDFS(const Vector2 startPos, const Vector2 finishPos, MazePath* mazePath, MazeNode* startingNode);

	// \brief Get a non random unexplored neighbour from a given node
	//
	// \param mazeNode: Node from wich to get the neighbour
	// \param *mazePath: Map with current path and visited nodes
	//
	// \return VisitedNode object rappresenting a neighbour to the passed node
	static VisitedNode getUnexploredNeighbour(const VisitedNode mazeNode, MazePath* mazePath);
};

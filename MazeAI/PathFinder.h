#pragma once
#include "MazePath.h"

#include <map>

class PathFinder {
private:
	struct DFSNode {
		MazeNode* mazeNode;
		Vector2 position;

		// \brief VisitedNode constructor
		DFSNode();

		// \brief VisitedNode constructor
		// \param *mazeNode: Pointer to a maze node
		// \param position: Vector2 containing position of the maze node
		DFSNode(MazeNode* mazeNode, const Vector2 position);

		// \brief Overload of operator =
		void operator=(const DFSNode& other);
		// \brief Overload of operator ==
		bool operator==(const DFSNode& rhs) const;
		// \brief Overload of operator !=
		bool operator!=(const DFSNode& rhs) const;
		// \brief Overload of operator <
		bool operator<(const DFSNode& other) const;
		// \brief Overload of operator >
		bool operator>(const DFSNode& other) const;
	};

	struct BestFirstNode : DFSNode {
		int gScore;
		int fScore;

		// \brief BestFirstNode constructor
		BestFirstNode();

		// \brief BestFirstNode constructor
		// \param dfsNode: instance of a DFSNode class to extendr
		// \param gScore: int with the value of gScore
		// \param gScore: int with the value of fScore
		BestFirstNode(DFSNode dfsNode, const int gScore, const int fScore);

		// \brief BestFirstNode constructor
		// \param *mazeNode: Pointer to a maze node
		// \param position: Vector2 containing position of the maze node
		// \param gScore: int with the value of gScore
		// \param gScore: int with the value of fScore
		BestFirstNode(MazeNode* mazeNode, const Vector2 position, const int gScore, const int fScore);
	};

public:
	enum PathFinderAlgorithm {
		NONE,
		DEPTH_FIRST_SEARCH,
		BEST_FIRST_GREEDY,
		A_STAR
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

	// \brief Find the path to a given maze using a Best First algorithm
	//
	// \param startPos: Starting position for the path finder
	// \param finishPos: Finishing position for the path finder
	// \param *mazePath: Pointer to the mazePath instance where to save the solution path
	// \param *mazeMap: Pointer to the mazeMap instance containing the maze to solve
	static void findPathBestFirst(const Vector2 startPos, const Vector2 finishPos, MazePath* mazePath, MazeNode* startingNode, const PathFinderAlgorithm algorithm);

	// \brief Get a non random unexplored neighbour from a given node
	//
	// \param mazeNode: Node from wich to get the neighbour
	// \param *mazePath: Map with current path and visited nodes
	//
	// \return VisitedNode object rappresenting a neighbour to the passed node
	static DFSNode getUnexploredNeighbour(const DFSNode mazeNode, MazePath* mazePath);

	// \brief Get all unexplored neighbours from a given node
	//
	// \param mazeNode: Node from wich to get the neighbours
	// \param *mazePath: Map with current path and visited nodes
	//
	// \return Vector of DFSNodes objects rappresenting the unexplored neighbours
	static std::vector<DFSNode> getAllUnexploredNeighbours(const DFSNode dfsNode, MazePath* mazePath);

	// \brief Get the manhattan distance between two vectors
	//
	// \param pos1: Vector2 of the first position
	// \param pos2: Vector2 of the second position
	//
	// \return returns the distance between the two points
	static int getManhattanDistance(const Vector2 pos1, const Vector2 pos2);

	// \brief Get the g score for a certain node
	//
	// \param pathMap: Map with all the paths currently knows to the algorithm
	// \param startNode: Position of the starting node
	// \param node: Node of wich to calculate the gScore
	//
	// \return int of the gScore
	static int getGScore(const std::map<BestFirstNode, BestFirstNode>& pathMap, const Vector2 startNode, const DFSNode node, const PathFinderAlgorithm algorithm);

	// \brief Get the position of a neighbouring node from the nodes position
	//
	// \param nodePosition: Position of the node from wich to get the neighbour position
	// \param direction: Direction of the neighbour
	//
	// \return returns the position of the neighbour inside a Vector2
	static Vector2 getNeighbourPosition(const Vector2 nodePosition, const Direction direction);

	// \brief Traces the path inside of the mazePath
	//
	// \param pathMap: Map with all the paths currently knows to the algorithm
	// \param *mazePath: Map with current path and visited nodes
	// \param startNode: Position of the starting node
	static void traceSolutionPath(const std::map<BestFirstNode, BestFirstNode>& pathMap, MazePath* mazePath, const Vector2 startNode, const BestFirstNode finishNode);

	// \brief Counts the amount fo nodes from a certain node to the starting node
	// 
	// \param pathMap: Map with all the paths currently knows to the algorithm
	// \param startNode: Position of the starting node
	// \param node: Node from wich to start counting
	//
	// \return returns the number of nodes needed to reach the start from the current node
	static int countPathToStart(const std::map<BestFirstNode, BestFirstNode>& pathMap, const Vector2 startNode, const BestFirstNode node);
};

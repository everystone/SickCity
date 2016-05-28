#pragma once
#include "AStar\stlastar.h"

// used with A* mapping
class TileNode {
public:
	int x;
	int y;

	TileNode(int px, int py) { x = px; y = py; }
	TileNode() { x = 0; y = 0; }

	float GoalDistanceEstimate(TileNode & nodeGoal);
	bool isGoal(TileNode &nodeGoal);
	bool getSuccessors(AStarSearch<TileNode> *astarsearch, TileNode *parent_node);
	float getCost(TileNode & succesor);
	bool isSameState(TileNode & rhs);
};
#include "stdafx.h"
#include "TileNode.h"

float TileNode::GoalDistanceEstimate(TileNode & nodeGoal)
{
	// heuristic function that estimates the distance from a Node to the Goal
	return fabsf(this->x -)
}

bool TileNode::isSameState(TileNode & rhs)
{
	
	// same state is when (x,y) are the same
	if (this->x == rhs.x && this->y == rhs.y) {
		return true;
	}
	return false;
}

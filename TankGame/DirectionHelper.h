#pragma once

#include <glm/glm.hpp>

// 0 = North
// 1 = East
// 2 = South
// 3 = West

class DirectionHelper
{
public:
	static glm::vec3 GetDirection(int directionIndex);
	static int GetRandDirection();
};
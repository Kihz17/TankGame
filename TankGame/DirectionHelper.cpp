#include "DirectionHelper.h"

glm::vec3 DirectionHelper::GetDirection(int directionIndex)
{
	if (directionIndex == 0) // North
	{
		return glm::vec3(1.0f, 0.0f, 0.0f);
	}
	else if (directionIndex == 1) // East
	{
		return glm::vec3(0.0f, 0.0f, 1.0f);
	}
	else if (directionIndex == 2) // South
	{
		return glm::vec3(-1.0f, 0.0f, 0.0f);
	}
	else // West
	{
		return glm::vec3(0.0f, 0.0f, -1.0f);
	}
}

int DirectionHelper::GetRandDirection()
{
	int random = rand() % (3 - 0 + 1) + 0;
	return random;
}
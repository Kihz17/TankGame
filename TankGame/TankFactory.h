#pragma once

#include "ITank.h"
#include "InputHandler.h"

class TankFactory
{
public:
	~TankFactory() = default;

	static TankFactory* GetInstance();

	ITank* MakeTank(const glm::vec3& position, const glm::vec4& color, bool isPlayer = false, InputHandler* inputHandler = NULL);

private:
	TankFactory() = default;

	static TankFactory* instance;
	static unsigned int currentTankID;
};
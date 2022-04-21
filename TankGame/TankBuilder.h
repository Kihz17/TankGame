#pragma once

#include "ITank.h"
#include "InputHandler.h"

#include <string>

class TankBuilder
{
public:
	~TankBuilder() = default;

	static TankBuilder* GetInstance();

	ITank* MakeTank(const glm::vec3& position, const glm::vec4& color, const std::string& weaponType, bool isPlayer = false, InputHandler* inputHandler = NULL);

private:
	TankBuilder() = default;
	static TankBuilder* instance;
};
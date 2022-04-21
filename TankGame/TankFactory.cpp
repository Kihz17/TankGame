#include "TankFactory.h"
#include "Tank.h"
#include "PlayerTank.h"

TankFactory* TankFactory::instance = NULL;
unsigned int TankFactory::currentTankID = 0;

TankFactory* TankFactory::GetInstance()
{
	if (TankFactory::instance == NULL)
	{
		TankFactory::instance = new TankFactory();
	}

	return instance;
}

ITank* TankFactory::MakeTank(const glm::vec3& position, const glm::vec4& color, bool isPlayer, InputHandler* inputHandler)
{
	if (isPlayer)
	{
		return new PlayerTank(currentTankID++, "tank", position, 1.0f, color, 100.0f, *inputHandler);
	}

	return new Tank(currentTankID++, "tank", position, 1.0f, color, 100.0f);
}
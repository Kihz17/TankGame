#include "TankBuilder.h"
#include "TankFactory.h"
#include "Weapon/BulletWeapon.h"
#include "Weapon/LaserWeapon.h"

TankBuilder* TankBuilder::instance = NULL;

TankBuilder* TankBuilder::GetInstance()
{
	if (TankBuilder::instance == NULL)
	{
		TankBuilder::instance = new TankBuilder();
	}

	return instance;
}

ITank* TankBuilder::MakeTank(const glm::vec3& position, const glm::vec4& color, const std::string& weaponType, bool isPlayer, InputHandler* inputHandler)
{
	TankFactory* tankFactory = TankFactory::GetInstance();
	ITank* tank = tankFactory->MakeTank(position, color, isPlayer, inputHandler);

	// Give tanks weapons
	tank->SetPrimaryWeapon(new BulletWeapon());
	tank->SetSecondaryWeapon(new LaserWeapon());

	return tank;
}
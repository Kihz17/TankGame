#pragma once

#include "../Question1/Maze.h"
#include "ITank.h"
#include "Projectile/IProjectile.h"
#include "Weapon/IWeapon.h"

#include <vector>

class ITankManagement
{
public:
	struct sProjectileInfo
	{
		IProjectile* projectile;
		IWeapon* shotFrom;
	};

	virtual ~ITankManagement() = default;

	virtual void SetMaze(Maze* maze) = 0;
	virtual void AddTank(ITank* tank) = 0;
	virtual void RemoveTank(ITank* tank) = 0;
	virtual void AddProjectile(const sProjectileInfo& projectileInfo) = 0;
};
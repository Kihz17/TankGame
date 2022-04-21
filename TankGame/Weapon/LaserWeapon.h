#pragma once

#include "Weapon.h"

class LaserWeapon : public Weapon
{
public:
	LaserWeapon();
	virtual ~LaserWeapon() = default;

	virtual bool CanShoot() override;
};
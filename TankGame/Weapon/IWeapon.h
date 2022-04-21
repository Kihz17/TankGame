#pragma once

#include "../Projectile/IProjectile.h"
#include "../IMessage.h"
#include <glm/glm.hpp>

class IWeapon
{
public:
	virtual ~IWeapon() {};

	virtual IProjectile* Fire(IMessage* receiver, const glm::vec3& position, unsigned int direction) = 0;

	virtual bool CanShoot() = 0;

	virtual void DetachProjectile(IProjectile* projectile) = 0;

	virtual void SetDamage(float damage) = 0;
};
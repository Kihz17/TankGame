#pragma once

#include "Weapon.h"
#include "../Projectile/IProjectile.h"

class BulletWeapon : public Weapon
{
public:
	BulletWeapon();
	virtual ~BulletWeapon() = default;

	virtual IProjectile* Fire(IMessage* receiver, const glm::vec3& position, unsigned int direction) override;
	virtual bool CanShoot() override;
	virtual void DetachProjectile(IProjectile* projectile) override;

private:
	IProjectile* bullet;
};
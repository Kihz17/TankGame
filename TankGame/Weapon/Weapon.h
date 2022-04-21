#pragma once

#include "IWeapon.h"

#include <string>

class WeaponImpl;
class Weapon : public IWeapon {
public:
	Weapon(const std::string& projectileType, float shootCooldown, float damage);
	virtual ~Weapon();

	virtual IProjectile* Fire(IMessage* receiver, const glm::vec3& position, unsigned int direction) override;
	virtual bool CanShoot() = 0;
	virtual void DetachProjectile(IProjectile* projectile) override;
	virtual void SetDamage(float damage) override;

protected:
	virtual float& GetLastShootTime();

	virtual float GetShootCooldown();
private:
	WeaponImpl* weaponImpl;
};
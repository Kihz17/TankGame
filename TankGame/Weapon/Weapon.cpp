#include "Weapon.h"
#include "WeaponImpl.h"
#include "../Projectile/ProjectileFactory.h"
#include "../Projectile/Projectile.h"

Weapon::Weapon(const std::string& projectileType, float shootCooldown, float damage)
	: weaponImpl(new WeaponImpl(projectileType, shootCooldown, damage))
{

}

Weapon::~Weapon()
{
	delete this->weaponImpl;
}

float& Weapon::GetLastShootTime()
{
	return this->weaponImpl->lastShootTime;
}

float Weapon::GetShootCooldown()
{
	return this->weaponImpl->shootCooldown;
}

IProjectile* Weapon::Fire(IMessage* receiver, const glm::vec3& position, unsigned int direction)
{
	IProjectile* proj = ProjectileFactory::GetInstance()->MakeProjectile(this->weaponImpl->projectileType, position, direction, this->weaponImpl->damage);
	static_cast<Projectile*>(proj)->SetReciever(receiver);
	return proj;
}

void Weapon::DetachProjectile(IProjectile* projectile)
{

}

void Weapon::SetDamage(float damage)
{
	this->weaponImpl->damage = damage;
}
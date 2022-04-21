#include "BulletWeapon.h"
#include "../../GLCommon.h"

BulletWeapon::BulletWeapon()
	: Weapon("bullet", 1.0f, 10.0f)
{

}

IProjectile* BulletWeapon::Fire(IMessage* receiver, const glm::vec3& position, unsigned int direction)
{
	if (!CanShoot())
	{
		return NULL; // Something we shot hasn't been destroyed yet, wait
	}

	IProjectile* bullet = Weapon::Fire(receiver, position, direction);
	this->bullet = bullet;
	return bullet;
}

void BulletWeapon::DetachProjectile(IProjectile* projectile)
{
	if (this->bullet == projectile) // Stop referencing this bullet
	{
		this->bullet = NULL;
	}
}

bool BulletWeapon::CanShoot()
{
	return this->bullet == NULL;
}
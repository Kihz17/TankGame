#include "Bullet.h"

Bullet::Bullet(unsigned int ID, const glm::vec3& position, unsigned int direction, float damage)
	: Projectile(ID, position, direction, 0.1f, 0, damage)
{

}

bool Bullet::HasTrail()
{
	return false;
}
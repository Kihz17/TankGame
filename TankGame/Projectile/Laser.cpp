#include "Laser.h"

Laser::Laser(unsigned int ID, const glm::vec3& position, unsigned int direction, float damage)
	: Projectile(ID, position, direction, 0.2f, 0, damage)
{

}

bool Laser::HasTrail()
{
	return true;
}
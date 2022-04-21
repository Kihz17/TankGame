#include "ProjectileFactory.h"
#include "Bullet.h"
#include "Laser.h"

unsigned int ProjectileFactory::currentProjectileID = 0;
ProjectileFactory* ProjectileFactory::instance = NULL;

ProjectileFactory* ProjectileFactory::GetInstance()
{
	if (ProjectileFactory::GetInstance == NULL)
	{
		ProjectileFactory::instance = new ProjectileFactory();
	}

	return instance;
}

IProjectile* ProjectileFactory::MakeProjectile(const std::string& type, const glm::vec3 position, unsigned int direction, float damage)
{
	if (type == "bullet")
	{
		return new Bullet(currentProjectileID++, position, direction, damage);
	}
	else if (type == "laser")
	{
		return new Laser(currentProjectileID++, position, direction, damage);
	}

	return NULL;
}
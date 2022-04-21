#pragma once

#include "IProjectile.h"

#include <glm/glm.hpp>
#include <string>

class ProjectileFactory
{
public:
	~ProjectileFactory() = default;

	static ProjectileFactory* GetInstance();

	IProjectile* MakeProjectile(const std::string& type, const glm::vec3 position, unsigned int direction, float damage);
private:
	ProjectileFactory() = default;

	static ProjectileFactory* instance;

	static unsigned int currentProjectileID;
};
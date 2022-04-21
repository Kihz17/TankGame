#pragma once

#include "Projectile.h"

class Bullet : public Projectile
{
public:
	Bullet(unsigned int ID, const glm::vec3& position, unsigned int direction, float damage);
	~Bullet() = default;

	virtual bool HasTrail() override;
};
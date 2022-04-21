#pragma once 

#include "Projectile.h"

class Laser : public Projectile
{
public:
	Laser(unsigned int ID, const glm::vec3& position, unsigned int direction, float damage);
	~Laser() = default;
	
	virtual bool HasTrail() override;
};
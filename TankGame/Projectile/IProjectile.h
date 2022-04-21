#pragma once

#include <glm/glm.hpp>

class IProjectile
{
public:
	virtual ~IProjectile() {}

	virtual unsigned int GetID() const = 0;

	virtual bool IsValid() const = 0;

	virtual void OnUpdate(float deltaTime) = 0;

	virtual float GetDamage() const = 0;

	virtual glm::vec3 GetPosition() const = 0;
	virtual glm::vec3 GetOriginalPosition() const = 0;
	virtual glm::vec3 GetDirection() const = 0;

	virtual bool HasTrail() = 0;
};
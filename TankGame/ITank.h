#pragma once

#include "Weapon/IWeapon.h"

#include <glm/glm.hpp>
#include <string>

class ITank
{
public:
	virtual ~ITank() {};

	virtual std::string GetModelName() = 0;

	virtual unsigned int GetID() = 0;

	virtual void OnUpdate(float deltaTime) = 0;

	virtual float GetHealth() = 0;
	virtual float GetMaxHealth() = 0;
	virtual void DamageTank(float damage) = 0;

	virtual glm::vec3 GetPosition() = 0;
	virtual unsigned int GetDirection() = 0;
	virtual glm::mat3x3 GetRotation() = 0;
	virtual float GetSpeed() = 0;

	virtual void ChooseNewDirection() = 0;

	virtual glm::vec4 GetColor() = 0;

	virtual void SetPrimaryWeapon(IWeapon* weapon) = 0;
	virtual void SetSecondaryWeapon(IWeapon* weapon) = 0;

	virtual void MakeSuperTank() = 0;
};
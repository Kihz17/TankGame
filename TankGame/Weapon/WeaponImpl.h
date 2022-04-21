#pragma once

#include <string>

class WeaponImpl
{
public:
	WeaponImpl(const std::string& projectileType, float shootCooldown, float damage)
		: projectileType(projectileType), lastShootTime(0.0f), shootCooldown(shootCooldown), damage(damage) {}

	~WeaponImpl() = default;

	std::string projectileType;

	float lastShootTime;
	float shootCooldown;

	float damage;
};
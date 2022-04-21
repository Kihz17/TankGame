#pragma once

#include "Weapon/IWeapon.h"

#include <glm/glm.hpp>
#include <string>

class TankImpl
{
public:
	enum class TankState
	{
		None = 0,
		Moving,
		Waiting,
		Blocked
	};


	TankImpl(unsigned int ID, const std::string& modelName, const glm::vec3& position, unsigned int startDirection, float speed, const glm::vec4& color, float health, IWeapon* weapon)
		: ID(ID), modelName(modelName), position(position), direction(startDirection), speed(speed), color(color), foundEnemy(false),
		currenHealth(health), maxHealth(health), state(TankState::None), lastMoveTime(0.0f), canMove(true), primaryWeapon(weapon)
	{}

	~TankImpl()
	{
		if (this->primaryWeapon)
		{
			delete this->primaryWeapon;
		}
		
		if (this->secondaryWeapon)
		{
			delete this->secondaryWeapon;
		}
	}

	std::string modelName;

	unsigned int ID;

	glm::vec3 position;
	unsigned int direction; // 0 = North, 1 = East, 2 = South, 3 = West
	float speed;

	float currenHealth;
	float maxHealth;

	TankState state;

	glm::vec4 color;

	float lastMoveTime;
	bool canMove;

	IWeapon* primaryWeapon;
	IWeapon* secondaryWeapon;

	bool foundEnemy;
};
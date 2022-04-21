#pragma once 

#include <glm/glm.hpp>

class ProjectileImpl
{
public:
	ProjectileImpl(unsigned int ID, const glm::vec3& position, unsigned int direction, float speed, int bouncesLeft, float damage)
		: ID(ID), position(position), startPosition(position), speed(speed), lastMoveTime(0.0f), bouncesLeft(bouncesLeft), direction(direction), damage(damage) {}

	~ProjectileImpl() = default;

	unsigned int ID;
	unsigned int direction;

	float damage;

	glm::vec3 position;
	const glm::vec3 startPosition;
	float speed;

	float lastMoveTime;

	int bouncesLeft;
};
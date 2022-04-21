#include "Projectile.h"
#include "ProjectileImpl.h"

#include "../DirectionHelper.h"
#include "../../GLCommon.h"

Projectile::Projectile(unsigned int ID, const glm::vec3& position, unsigned int direction, float speed, int bouncesLeft, float damage)
	: projectileImpl(new ProjectileImpl(ID, position, direction, speed, bouncesLeft, damage))
{

}

Projectile::~Projectile()
{
	delete this->projectileImpl;
}

void Projectile::OnUpdate(float deltaTime)
{
	float currentTime = glfwGetTime();
	float lastMoveTime = this->projectileImpl->lastMoveTime;
	if (lastMoveTime != 0.0f && (currentTime - lastMoveTime) < this->projectileImpl->speed)
	{
		return; // Don't move yet
	}

	this->projectileImpl->lastMoveTime = currentTime;

	glm::vec3 newPosition = this->projectileImpl->position + (DirectionHelper::GetDirection(this->projectileImpl->direction));

	sMessage message;
	message.command = "ProjectileCollisionCheck";
	// Pass data
	glm::vec4 fData((float)this->projectileImpl->ID, // ID
		newPosition.x,  // X Pos
		newPosition.z, // Z Pos
		(float) this->projectileImpl->bouncesLeft); // Bounces Left
	message.fData.push_back(fData);

	// Send off to mediator to decide collision
	this->receiver->RecieveMessage(message);
}

unsigned int Projectile::GetID() const
{
	return this->projectileImpl->ID;
}

bool Projectile::IsValid() const
{
	return this->projectileImpl->bouncesLeft > 0;
}

bool Projectile::RecieveMessage(sMessage theMessage)
{
	const std::string& command = theMessage.command;
	if (command == "ProjectileCollision")
	{
		// Apply our new position
		this->projectileImpl->position.x = theMessage.fData[0].x;
		this->projectileImpl->position.z = theMessage.fData[0].y;

		// Set to proper bounce count
		this->projectileImpl->bouncesLeft = (int) theMessage.fData[0].z;
		return true;
	}

	return false;
}

bool Projectile::SetReciever(IMessage* pTheReciever)
{
	this->receiver = pTheReciever;
	return true;
}

bool Projectile::RecieveMessage(sMessage theMessage, sMessage& messageReply)
{
	return true;
}

float Projectile::GetDamage() const
{
	return this->projectileImpl->damage;
}

glm::vec3 Projectile::GetPosition() const
{
	return this->projectileImpl->position;
}

glm::vec3 Projectile::GetOriginalPosition() const
{
	return this->projectileImpl->startPosition;
}

glm::vec3 Projectile::GetDirection() const
{
	return DirectionHelper::GetDirection(this->projectileImpl->direction);
}
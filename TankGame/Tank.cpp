#include "Tank.h"
#include "TankImpl.h"
#include "../GLCommon.h"
#include "DirectionHelper.h"

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp> 


Tank::Tank(unsigned int ID, const std::string& modelName, const glm::vec3& position, float speed, const glm::vec4& color, float health)
	: tankImpl(new TankImpl(ID, modelName, position, DirectionHelper::GetRandDirection(), speed, color, health, NULL)), receiver(NULL)
{

}

Tank::~Tank()
{
	delete this->tankImpl;
}

std::string Tank::GetModelName()
{
	return this->tankImpl->modelName;
}

unsigned int Tank::GetID()
{
	return this->tankImpl->ID;
}

glm::vec3 Tank::GetPosition()
{
	return this->tankImpl->position;
}

unsigned int Tank::GetDirection()
{
	return this->tankImpl->direction;
}

float Tank::GetSpeed()
{
	return this->tankImpl->speed;
}

glm::vec4 Tank::GetColor()
{
	return this->tankImpl->color;
}

float Tank::GetHealth()
{
	return this->tankImpl->currenHealth;
}

float Tank::GetMaxHealth()
{
	return this->tankImpl->maxHealth;
}

void Tank::OnUpdate(float deltaTime) 
{
	float currentTime = glfwGetTime();
	bool canEvaluate = this->tankImpl->lastMoveTime == 0.0f || (currentTime - this->tankImpl->lastMoveTime) >= 0.1f; // Only allow one action per second
	if (!canEvaluate) // We haven't waited long enough, do nothing
	{
		return;
	}

	this->tankImpl->lastMoveTime = currentTime;

	if (!this->tankImpl->canMove) // We can't move!
	{
		return;
	}

	TankImpl::TankState& state = this->tankImpl->state;
	if (state == TankImpl::TankState::None || state == TankImpl::TankState::Waiting) // Try moving
	{
		state = TankImpl::TankState::Moving; // Start moving
	}
	else if (state == TankImpl::TankState::Blocked)
	{
		this->ChooseNewDirection();

		state = TankImpl::TankState::Waiting; // We have completed our action, go back to waiting
	}
	else if (state == TankImpl::TankState::Moving)
	{
		glm::vec3 newPosition = this->tankImpl->position + (((DirectionHelper::GetDirection(this->tankImpl->direction)) * this->tankImpl->speed)); // Move forward in the direction we are going

		// Ask the mediator if we can move here
		sMessage message;
		message.command = "TankCheckValidPosition";
		glm::vec4 fData(this->tankImpl->ID, newPosition.x, newPosition.z, 0.0f);
		message.fData.push_back(fData);
		this->receiver->RecieveMessage(message);
	}

	TryShoot();

	// Try to fire our weapon
	if (this->tankImpl->foundEnemy)
	{
		this->tankImpl->foundEnemy = false;
		int randNum = (int)(((((double)std::rand()) / RAND_MAX) * 2) + 1);
		ShootWeapon(randNum == 1 ? this->tankImpl->primaryWeapon : this->tankImpl->secondaryWeapon);
	}
}

bool Tank::RecieveMessage(sMessage theMessage)
{
	const std::string& command = theMessage.command;
	if (command == "PositionValidReply")
	{
		bool valid = theMessage.fData[0].x == 1.0f;
		if (valid) // We are valid, move there!
		{
			this->tankImpl->position.x = theMessage.fData[0].y;
			this->tankImpl->position.z = theMessage.fData[0].z;
			this->tankImpl->state = TankImpl::TankState::Waiting; // We have completed our action, go back to waiting
		}
		else
		{
			this->tankImpl->state = TankImpl::TankState::Blocked; // We can't move, we are blocked
		}

		return true;
	}
	else if (command == "FoundEnemy")
	{
		this->tankImpl->foundEnemy = true;
	}

	return false;
}

bool Tank::SetReciever(IMessage* pTheReciever)
{
	this->receiver = pTheReciever;
	return true;
}

bool Tank::RecieveMessage(sMessage theMessage, sMessage& messageReply)
{
	return true;
}

glm::mat3x3 Tank::GetRotation()
{
	if (this->tankImpl->direction == 0) // North
	{
	return glm::mat3x3(
		0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f);
	}
	else if (this->tankImpl->direction == 1) // East
	{
		return glm::mat3x3(
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f);
	}
	else if (this->tankImpl->direction == 2) // South
	{
		return glm::mat3x3(
			0.0f, 0.0f, -1.0f,
			0.0f, 1.0f, 0.0f,
			-1.0f, 0.0f, 0.0f);
	}
	else // West
	{
		return glm::mat3x3(
			-1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, -1.0f);
	}
}

void Tank::ChooseNewDirection()
{
	this->tankImpl->direction = DirectionHelper::GetRandDirection();
}

void Tank::SetPrimaryWeapon(IWeapon* weapon)
{
	if (this->tankImpl->primaryWeapon)
	{
		delete this->tankImpl->primaryWeapon;
	}

	this->tankImpl->primaryWeapon = weapon;
}

void Tank::SetSecondaryWeapon(IWeapon* weapon)
{
	if (this->tankImpl->secondaryWeapon )
	{
		delete this->tankImpl->secondaryWeapon;
	}

	this->tankImpl->secondaryWeapon = weapon;
}

void Tank::DamageTank(float damage)
{
	this->tankImpl->currenHealth -= damage;
}

void Tank::TryShoot() const
{
	sMessage message;
	message.command = "FindEnemy";
	glm::vec4 fData(this->tankImpl->ID, this->tankImpl->direction, 0.0f, 0.0f);
	message.fData.push_back(fData);
	this->receiver->RecieveMessage(message);
}

void Tank::ShootWeapon(IWeapon* weapon)
{
	IProjectile* proj = weapon->Fire(this->receiver, this->tankImpl->position, this->tankImpl->direction);
	if (proj == NULL) // Can't shoot
	{
		return;
	}

	sMessage message;
	message.command = "AddProjectile";
	message.pData.push_back(proj); // Tell the mediator about the projectile
	message.pData.push_back(weapon); // Tell the mediator what weapon the projectile is being shot from
	this->receiver->RecieveMessage(message);
}

void Tank::MakeSuperTank()
{
	// Heal up to 150
	this->tankImpl->currenHealth = 150.0f;
	this->tankImpl->maxHealth = 150.0f;
	this->tankImpl->primaryWeapon->SetDamage(20.0f);
	this->tankImpl->secondaryWeapon->SetDamage(20.0f);
}
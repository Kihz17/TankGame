#include "PlayerTank.h"
#include "TankImpl.h"
#include "DirectionHelper.h"

#include <iostream>

PlayerTank::PlayerTank(unsigned int ID, const std::string& modelName, const glm::vec3& position, float speed, const glm::vec4& color, float health, InputHandler& inputHandler)
	: Tank(ID, modelName, position, speed, color, health), inputHandler(inputHandler)
{

}

void PlayerTank::OnUpdate(float deltaTime)
{
	this->tankImpl->canMove = this->inputHandler.isMoveKeyDown();
	if (this->tankImpl->canMove) // Only allow movement if we have a key down
	{
		Tank::OnUpdate(deltaTime);
	}

	

	// Controls to shoot
	if (this->inputHandler.isSpaceDown)
	{
		this->inputHandler.isSpaceDown = false;

		IWeapon* weapon = this->tankImpl->primaryWeapon;
		if (weapon)
		{
			this->ShootWeapon(weapon);
		}
	}
	else if (this->inputHandler.isShiftDown)
	{
		this->inputHandler.isShiftDown = false;

		IWeapon* weapon = this->tankImpl->secondaryWeapon;
		if (weapon)
		{
			this->ShootWeapon(weapon);
		}
	}
}

void PlayerTank::SetDirection(unsigned int direction)
{
	this->tankImpl->direction = direction;
}

void PlayerTank::SetCanMove(bool canMove)
{

}

void PlayerTank::UseWeapon()
{

}

void PlayerTank::ChooseNewDirection()
{
	// Important that this does nothing to allow our player to control
}

void PlayerTank::TryShoot() const
{
	// Important that this does nothing to allow our player to shoot weapons
}

void PlayerTank::MakeSuperTank() // Players can't become super tanks
{

}

void PlayerTank::ShootWeapon(IWeapon* weapon)
{
	IProjectile* proj = NULL;
	if (weapon == this->tankImpl->primaryWeapon)
	{
		proj = this->tankImpl->primaryWeapon->Fire(this->receiver, this->tankImpl->position, this->tankImpl->direction);
	}
	else if (weapon == this->tankImpl->secondaryWeapon)
	{
		proj = this->tankImpl->secondaryWeapon->Fire(this->receiver, this->tankImpl->position + DirectionHelper::GetDirection(this->tankImpl->direction), this->tankImpl->direction);
	}

	if (proj) 
	{
		sMessage message;
		message.command = "AddProjectile";
		message.pData.push_back(proj); // Tell the mediator about the projectile
		message.pData.push_back(this->tankImpl->primaryWeapon); // Tell the mediator what weapon the projectile is being shot from
		this->receiver->RecieveMessage(message);
	}	
}
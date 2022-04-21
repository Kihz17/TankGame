#pragma once

#include "InputHandler.h"
#include "Tank.h"

class PlayerTank : public Tank
{
public:
	PlayerTank(unsigned int ID, const std::string& modelName, const glm::vec3& position, float speed, const glm::vec4& color, float health, InputHandler& inputHandler);
	virtual ~PlayerTank() = default;

	virtual void OnUpdate(float deltaTime) override;

	virtual void ChooseNewDirection() override;

	virtual void SetDirection(unsigned int direction);
	virtual void SetCanMove(bool canMove);
	virtual void UseWeapon();

	virtual void TryShoot() const override;

	virtual void MakeSuperTank() override;

protected:
	virtual void ShootWeapon(IWeapon* weapon) override;

private:
	InputHandler& inputHandler;
};
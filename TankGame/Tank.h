#pragma once

#include "ITank.h"
#include "IMessage.h"
#include "Weapon/IWeapon.h"

class TankImpl;
class Tank : public ITank, public IMessage
{
public:
	Tank(unsigned int ID, const std::string& modelName, const glm::vec3& position, float speed, const glm::vec4& color, float health);
	virtual ~Tank();

	// Inherited from ITank
	virtual std::string GetModelName() override;
	virtual unsigned int GetID() override;
	virtual void OnUpdate(float deltaTime) override;
	virtual float GetHealth() override;
	virtual float GetMaxHealth() override;
	virtual glm::vec3 GetPosition() override;
	virtual unsigned int GetDirection() override;
	virtual float GetSpeed()  override;
	virtual glm::mat3x3 GetRotation() override;
	virtual void ChooseNewDirection() override;
	virtual glm::vec4 GetColor() override;
	virtual void DamageTank(float damage) override;
	virtual void SetPrimaryWeapon(IWeapon* weapon) override;
	virtual void SetSecondaryWeapon(IWeapon* weapon) override;
	virtual void MakeSuperTank() override;

	virtual void TryShoot() const;

	// Inherited from IMessage
	virtual bool RecieveMessage(sMessage theMessage) override;
	virtual bool SetReciever(IMessage* pTheReciever) override;
	virtual bool RecieveMessage(sMessage theMessage, sMessage& messageReply) override;
protected:
	virtual void ShootWeapon(IWeapon* weapon);

	IMessage* receiver;
	TankImpl* tankImpl;
};
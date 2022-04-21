#pragma once

#include "IProjectile.h"
#include "../IMessage.h"

#include <glm/glm.hpp>

class ProjectileImpl;
class Projectile : public IProjectile, IMessage
{
public:
	Projectile(unsigned int ID, const glm::vec3& position, unsigned int direction, float speed, int bouncesLeft, float damage);
	~Projectile();

	virtual void OnUpdate(float deltaTime) override;
	virtual unsigned int GetID() const override;
	virtual bool IsValid() const override;
	virtual float GetDamage() const override;
	virtual glm::vec3 GetPosition() const override;
	virtual glm::vec3 GetOriginalPosition() const override;
	virtual glm::vec3 GetDirection() const override;
	virtual bool HasTrail() = 0;

	// Inherited from IMessage
	virtual bool RecieveMessage(sMessage theMessage) override;
	virtual bool SetReciever(IMessage* pTheReciever) override;
	virtual bool RecieveMessage(sMessage theMessage, sMessage& messageReply) override;

private:
	ProjectileImpl* projectileImpl;

	IMessage* receiver;
};
#pragma once

#include "ITankManagement.h"
#include "IMessage.h"
#include "../Question1/Maze.h"
#include "../CompiledShader.h"

#include <map>

class TankMediator : public ITankManagement, public IMessage
{
public:
	TankMediator() = default;
	virtual ~TankMediator() = default;

	// Inherited from ITankManagement
	virtual void SetMaze(Maze* maze) override;
	virtual void AddTank(ITank* tank) override;
	virtual void AddProjectile(const sProjectileInfo& projectileInfo) override;
	virtual void RemoveTank(ITank* tank) override;

	// Inherited from IMessage
	virtual bool RecieveMessage(sMessage theMessage) override;
	virtual bool SetReciever(IMessage* pTheReciever) override;
	virtual bool RecieveMessage(sMessage theMessage, sMessage& messageReply) override;

	virtual void UpdateProjectiles(const CompiledShader& shader, float deltaTime);

private:
	void RemoveProjectile(unsigned int ID);

	std::map<unsigned int, ITank*> tanks;

	std::map<unsigned int, sProjectileInfo> projectiles;
	std::vector<unsigned int> queuedForRemoval;

	Maze* maze;

	IMessage* receiver;
};
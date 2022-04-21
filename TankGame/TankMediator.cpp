#include "TankMediator.h"
#include "Tank.h"
#include "Projectile/Projectile.h"
#include "../ModelManager.h"
#include "DirectionHelper.h"

void TankMediator::SetMaze(Maze* maze)
{
	this->maze = maze;
}

void TankMediator::AddTank(ITank* tank)
{
	this->tanks.insert(std::make_pair(tank->GetID(), tank));
	static_cast<Tank*>(tank)->SetReciever(this);
}

void TankMediator::RemoveTank(ITank* tank)
{
	this->tanks.erase(tank->GetID());
}

void TankMediator::AddProjectile(const sProjectileInfo& projectileInfo)
{
	this->projectiles.insert(std::make_pair(projectileInfo.projectile->GetID(), projectileInfo));
}

bool TankMediator::RecieveMessage(sMessage theMessage)
{
	const std::string& command = theMessage.command;
	if (command == "TankCheckValidPosition") // Checks if a position is is valid in the maze
	{
		const glm::vec4& floatData = theMessage.fData[0];

		unsigned int ID = (unsigned int) floatData.x;
		std::map<unsigned int, ITank*>::iterator it = this->tanks.find(ID);
		if (it == this->tanks.end()) // Tank wans't found...
		{
			return true;
		}

		Tank* callingTank = static_cast<Tank*>(it->second);

		// Second two are always position data
		int x = (int) floatData.y;
		int z = (int) floatData.z;

		bool isWall = this->maze->IsWall(x, z);
		ITank* hitTank = NULL;
		if (!isWall) // There is no need to check for tank collision if we have already hit a wall
		{
			std::map<unsigned int, ITank*>::iterator it = this->tanks.begin();
			while (it != this->tanks.end())
			{
				ITank* tank = it->second;
				const glm::vec3& pos = tank->GetPosition();
				int tankX = (int)pos.x;
				int tankZ = (int)pos.z;

				if (x == tankX && z == tankZ) // There is a tank here
				{
					hitTank = tank;
					break;
				}

				it++;
			}
		}

		bool canMoveHere = !isWall && hitTank == NULL; // We are valid if nothing is here

		sMessage reply;
		reply.command = "PositionValidReply";
		reply.fData.push_back(glm::vec4((canMoveHere ? 1.0f : 0.0f), floatData.y, floatData.z, 0.0f));

		callingTank->RecieveMessage(reply);

		return true;
	}
	else if (command == "ProjectileCollisionCheck")
	{
		const glm::vec4& floatData = theMessage.fData[0];

		unsigned int ID = (unsigned int) floatData.x;
		std::map<unsigned int, sProjectileInfo>::iterator it = this->projectiles.find(ID);
		if (it == this->projectiles.end()) // Projectile wans't found...
		{
			return true;
		}

		sProjectileInfo projectileInfo = it->second;
		Projectile* projectileCaller = static_cast<Projectile*>(projectileInfo.projectile);

		// Second two are always position data
		int x = (int) floatData.y;
		int z = (int) floatData.z;
		int bouncesLeft = (int)floatData.w; // If we are a bouncing projectile, this will represent the number of bounces left

		bool isWall = this->maze->IsWall(x, z);
		ITank* hitTank = NULL;
		if (!isWall) // There is no need to check for tank collision if we have already hit a wall
		{
			std::map<unsigned int, ITank*>::iterator it = this->tanks.begin();
			while (it != this->tanks.end())
			{
				ITank* tank = it->second;
				const glm::vec3& pos = tank->GetPosition();
				int tankX = (int)pos.x;
				int tankZ = (int)pos.z;

				if (x == tankX && z == tankZ) // There is a tank here
				{
					hitTank = tank;
					break;
				}

				it++;
			}
		}
	
		sMessage reply;
		reply.command = "ProjectileCollision";

		if (hitTank != NULL) // We hit a tank, deal some damage to them and tell the projectile to destroy itself
		{
			hitTank->DamageTank(projectileCaller->GetDamage());
			queuedForRemoval.push_back(projectileCaller->GetID());
		}
		else if (isWall) // We hit a wall
		{
			bouncesLeft--;
			bool destroy = bouncesLeft <= 0;
			if (destroy)
			{
				queuedForRemoval.push_back(projectileCaller->GetID());
				return true; // Projectile is dead, nothing more to do

			}
			else
			{
				// We bounced, find a spot to bounce to (maze.CheckAdjacent())
				// Set x, z to found values
				glm::vec2 newPoint = maze->FindAvailableAdjacent(x, z);
				if (newPoint.x == x && newPoint.y == z) // We had nowhere to go
				{
					queuedForRemoval.push_back(projectileCaller->GetID());
					return true;
				}

				// Assign out new points
				x = (int) newPoint.x;
				z = (int) newPoint.y;
			}
		}

		// Tell our projectile where to go
		glm::vec4 fData(x, z, bouncesLeft, 0.0f);
		reply.fData.push_back(fData);
		projectileCaller->RecieveMessage(reply);
		return true;
	}
	else if (command == "AddProjectile")
	{
		IProjectile* projectile = static_cast<IProjectile*>(theMessage.pData[0]);
		IWeapon* weapon = static_cast<IWeapon*>(theMessage.pData[1]);
		sProjectileInfo projectileInfo;
		projectileInfo.projectile = projectile;
		projectileInfo.shotFrom = weapon;

		AddProjectile(projectileInfo);
		return true;
	}
	else if (command == "FindEnemy") 
	{
		const glm::vec4& floatData = theMessage.fData[0];

		unsigned int ID = (unsigned int)floatData.x;
		std::map<unsigned int, ITank*>::iterator it = this->tanks.find(ID);
		if (it == this->tanks.end()) // Tank wans't found...
		{
			return true;
		}

		Tank* callingTank = static_cast<Tank*>(it->second);
		glm::vec3 direction = DirectionHelper::GetDirection((int) floatData.y);
		glm::vec3 position = callingTank->GetPosition();
		ITank* hitTank = NULL;
		for (int i = 0; 0 < 100; i++)
		{
			position += direction;
			int x = (int) position.x;
			int z = (int) position.z;
			if (this->maze->IsWall(x, z)) // Hit a wall, stop looking
			{
				break;
			}

			std::map<unsigned int, ITank*>::iterator it = this->tanks.begin();
			while (it != this->tanks.end())
			{
				ITank* tank = it->second;
				const glm::vec3& pos = tank->GetPosition();
				int tankX = (int)pos.x;
				int tankZ = (int)pos.z;

				if (x == tankX && z == tankZ) // There is a tank here
				{
					hitTank = tank;
					break;
				}

				it++;
			}

			if (hitTank)
			{
				break;
			}
		}

		if (hitTank)
		{
			sMessage reply;
			reply.command = "FoundEnemy";
			callingTank->RecieveMessage(reply);
		}
	}

	return false;
}

bool TankMediator::SetReciever(IMessage* pTheReciever)
{
	this->receiver = pTheReciever;
	return true;
}

bool TankMediator::RecieveMessage(sMessage theMessage, sMessage& messageReply)
{
	return true;
}

void TankMediator::UpdateProjectiles(const CompiledShader& shader, float deltaTime)
{
	shader.Bind();
	std::map<unsigned int, sProjectileInfo>::iterator it = this->projectiles.begin();
	while (it != this->projectiles.end())
	{
		IProjectile* projectile = it->second.projectile;
		projectile->OnUpdate(deltaTime);

		if (projectile->HasTrail()) // Draw spheres from original position to current position
		{
			glm::vec3 startPos = projectile->GetOriginalPosition();
			glm::vec3 direction = projectile->GetDirection();
			glm::vec3 currentPos = projectile->GetPosition();
			int unitsAway = (int) glm::distance(startPos, currentPos);
			for (int i = 1; i <= unitsAway; i++)
			{
				ModelManager::GetInstance()->Draw("sphere", shader, startPos + (direction * (float) i), glm::vec3(1.0, 0.0f, 0.0f), glm::vec3(0.0, 1.0f, 0.0f), glm::vec3(0.0, 0.0f, 1.0f), glm::vec3(0.3f, 0.3f, 0.3f));
			}
		}

		ModelManager::GetInstance()->Draw("sphere", shader, projectile->GetPosition(), glm::vec3(1.0, 0.0f, 0.0f), glm::vec3(0.0, 1.0f, 0.0f), glm::vec3(0.0, 0.0f, 1.0f), glm::vec3(0.3f, 0.3f, 0.3f));
		it++;
	}

	for (unsigned int ID: queuedForRemoval)
	{
		RemoveProjectile(ID);
	}

	queuedForRemoval.clear();
}

void TankMediator::RemoveProjectile(unsigned int ID)
{
	std::map<unsigned int, sProjectileInfo>::iterator it = this->projectiles.find(ID);
	if (it == this->projectiles.end())
	{
		return;
	}

	sProjectileInfo& info = it->second;

	if (info.shotFrom != NULL)
	{
		info.shotFrom->DetachProjectile(info.projectile);
	}

	delete info.projectile; // Release projectile
	this->projectiles.erase(ID); // Stop tracking
}
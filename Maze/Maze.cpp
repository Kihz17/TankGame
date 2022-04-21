#include "Maze.h"

#include <fstream>
#include <iostream>

int Maze::spawnPointCounter = 0;

Maze::Maze(const std::string& modelName, const std::string& path)
	:  modelName(path)
{
	LoadFromFile(path);
}

void Maze::LoadFromFile(const std::string& path)
{
	std::fstream mazeFile(path);
	std::string line;

	int x = 0;
	int z = 0;
	if (mazeFile.is_open())
	{
		while (std::getline(mazeFile, line))
		{
			if (line.size() > mazeXSize + 1)
			{
				throw "Line size was greater than maximum X!";
			}

			const char* characters = line.c_str();
			for (x = 0; x < line.size(); x++)
			{
				char c = characters[x];
				this->mazeMap[x][z] = (c == 'x' || c == 'X'); // We are a 'wall' if the character is an X
			}

			z++;

			if (z > mazeZSize + 1)
			{
				throw "Line size was greater than maximum Z!";
			}
		}
		mazeFile.close();
	}

	int consecutiveSlotCount = 0;
	bool doneSearching = false;
	for (int z = 0; z < mazeZSize; z++)
	{
		for (int x = 0; x < mazeXSize; x++)
		{
			if (!this->mazeMap[x][z]) // We are an empty space!
			{
				consecutiveSlotCount++;

				if (consecutiveSlotCount == spawnPointXWidth) // We are potentially a spawn point, let's check
				{
					consecutiveSlotCount = 0; // Reset counter

					int startX = (x - spawnPointXWidth) + 1;
					bool notSpawnPoint = CheckSpawnPoint(startX, z);

					if (!notSpawnPoint) // We made it this far, we are a spawn point!
					{
						glm::vec2 newPoint(startX + (spawnPointXWidth / 2), z + (spawnPointZWidth / 2));
						if (IsSpawnPointValid(newPoint))
						{
							this->spawnPoints[spawnPointCounter++] = glm::vec2(newPoint.x, newPoint.y);
							if (spawnPointCounter >= maxSpawnPoints) // We have the max amount of spawn points, stop searching
							{
								doneSearching = true;
								break;
							}
						}
						
					}
					else
					{
						x = (x - spawnPointXWidth) + 2; // Go back to check where we left off
					}
				}
			}
			else
			{
				consecutiveSlotCount = 0; // We found a barrier, restart count
			}

		}

		if (doneSearching)
		{
			break;
		}
	}
}

std::vector<glm::vec2> Maze::GetSpawnPoints()
{
	std::vector<glm::vec2> spawnPoints;
	for (int i = 0; i < maxSpawnPoints; i++)
	{
		spawnPoints.push_back(this->spawnPoints[i]);
	}

	return spawnPoints;
}

bool Maze::IsSpawnPointValid(const glm::vec2& point)
{
	bool alreadyUsed = false;
	for (int i = 0; i < spawnPointCounter; i++)
	{
		glm::vec2& spawnPoint = this->spawnPoints[i];
		int minX = spawnPoint.x - (spawnPointXWidth / 2);
		int minZ = spawnPoint.y - (spawnPointZWidth / 2);
		int maxX = spawnPoint.x + (spawnPointXWidth / 2);
		int maxZ = spawnPoint.y + (spawnPointZWidth / 2);

		for (int j = minX; j < maxX; j++)
		{
			for (int k = minZ; k < maxZ; k++)
			{
				if (point.x == j && point.y == k)
				{
					alreadyUsed = true;
					break;
				}
			}

			if (alreadyUsed)
			{
				break;
			}
		}
	}

	return !alreadyUsed;
}

bool Maze::CheckSpawnPoint(const int& startX, const int& startZ)
{
	bool notSpawnPoint = false;

	for (int i = 1; i < spawnPointZWidth; i++)
	{
		int xSlotCount = 0;
		for (int j = 0; j < spawnPointXWidth; j++)
		{
			if (!this->mazeMap[startX + j][startZ + i]) // We are an empty space, track it!
			{
				xSlotCount++;
			}
		}

		if (xSlotCount < spawnPointXWidth - 2) // Not wide enough
		{
			notSpawnPoint = true;
			break;
		}
	}

	return notSpawnPoint;
}

glm::vec2 Maze::FindAvailableAdjacent(int x, int z)
{
	if (CheckValidPosition(x + 1, z) && !this->mazeMap[x + 1][z])
	{
		return glm::vec2(x + 1, z);
	}
	else if (CheckValidPosition(x - 1, z) && !this->mazeMap[x - 1][z])
	{
		return glm::vec2(x - 1, z);
	}
	else if (CheckValidPosition(x, z + 1) && !this->mazeMap[x][z + 1])
	{
		return glm::vec2(x, z + 1);
	}
	else if (CheckValidPosition(x, z - 1) && !this->mazeMap[x][z - 1])
	{
		return glm::vec2(x, z - 1);
	}

	return glm::vec2(x, z);
}

bool Maze::CheckValidPosition(int x, int z)
{
	return x >= 0 && x < mazeXSize&& z >= 0 && z <= mazeZSize;
}
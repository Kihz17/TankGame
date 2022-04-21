#pragma once

#include <glm/glm.hpp>

#include <string>
#include <vector>

// Class that maps out a simple maze all "grid" slots must be positive in the X & Z coord to work
// Could map the maze a number of ways but I figures I'd use plain old arrays since they are the fastest
class Maze
{
public:
	Maze(const std::string& modelName, const std::string& path);
	~Maze() = default;

	std::vector<glm::vec2> GetSpawnPoints();

	inline bool IsWall(int x, int z) const
	{
		if (x < 0 || z < 0)
		{
			throw "A coordinate was negative!";
		}

		if (x > mazeXSize || z > mazeZSize)
		{
			return false;
		}

		return mazeMap[x][z];
	}

	glm::vec2 FindAvailableAdjacent(int x, int z);

	static const unsigned int maxSpawnPoints = 6;

private:
	void LoadFromFile(const std::string& path);

	bool IsSpawnPointValid(const glm::vec2& point);
	bool CheckSpawnPoint(const int& startX, const int& startZ);

	bool CheckValidPosition(int x, int z);

	static const unsigned int mazeXSize = 100;
	static const unsigned int mazeZSize = 60;

	static const int spawnPointXWidth = 10;
	static const int spawnPointZWidth = 10;
	static int spawnPointCounter;

	// We cannot initialize a 2D array unless the row length is a compile time constant, since we are only loading one maze for this project that is always the same
	// This implementation should be fine
	bool mazeMap[mazeXSize][mazeZSize]; 
	std::string modelName;

	glm::vec2 spawnPoints[maxSpawnPoints];
};
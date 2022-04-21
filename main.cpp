#pragma once

#include "GLCommon.h"

#include <glm/glm.hpp>
#include <glm/vec3.hpp> 
#include <glm/vec4.hpp> 
#include <glm/mat4x4.hpp> 
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp> 

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector> 
#include <fstream>
#include <sstream>

#include "ShaderManager.h"
#include "Camera.h"
#include "Light.h"
#include "ModelManager.h"
#include "LightManager.h"

#include "Question1/Maze.h"
#include "Question2-7/ITank.h"
#include "Question2-7/PlayerTank.h"
#include "Question2-7/TankFactory.h"
#include "Question2-7/TankBuilder.h"
#include "Question2-7/TankMediator.h"
#include "Question2-7/InputHandler.h"

const float windowWidth = 1200;
const float windowHeight = 640;
bool editMode = true;

ShaderManager gShaderManager;

Camera camera(windowHeight, windowWidth);
float moveSpeed = 1.1f;
float lightMoveSpeed = 0.01f;

PlayerTank* playerTank;
bool playerTankRemoved = false;

static InputHandler inputHandler;

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//// Movement controls
	//if (!editMode)
	//{
	//	if (key == GLFW_KEY_W)
	//	{
	//		camera.position += camera.direction * moveSpeed;
	//	}
	//	if (key == GLFW_KEY_A)
	//	{
	//		// Rotate our camera's direction 90 degrees to the left
	//		glm::vec3 left;
	//		constexpr float theta = glm::radians(90.0f);
	//		left.x = camera.direction.x * cos(theta) + camera.direction.z * sin(theta);
	//		left.y = 0.0f;
	//		left.z = -camera.direction.x * sin(theta) + camera.direction.z * cos(theta);
	//		camera.position += left * moveSpeed;
	//	}
	//	if (key == GLFW_KEY_S)
	//	{
	//		camera.position -= camera.direction * moveSpeed;
	//	}
	//	if (key == GLFW_KEY_D)
	//	{
	//		// Rotate our camera's direction 90 degrees to the right
	//		glm::vec3 right;
	//		constexpr float theta = glm::radians(-90.0f);
	//		right.x = camera.direction.x * cos(theta) + camera.direction.z * sin(theta);
	//		right.y = 0.0f;
	//		right.z = -camera.direction.x * sin(theta) + camera.direction.z * cos(theta);
	//		camera.position += right * moveSpeed;
	//	}
	//	if (key == GLFW_KEY_SPACE)
	//	{
	//		camera.position.y += moveSpeed;
	//	}
	//}

	//// Toggle cursor view
	//if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	//{
	//	editMode = !editMode;
	//	int cursorOption = editMode ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED;
	//	glfwSetInputMode(window, GLFW_CURSOR, cursorOption);
	//}

	if (!playerTankRemoved)
	{
		if (!inputHandler.isWDown && key == GLFW_KEY_W && action == GLFW_PRESS)
		{
			inputHandler.isWDown = true;
			playerTank->SetDirection(1);
		}
		else if (inputHandler.isWDown && key == GLFW_KEY_W && action == GLFW_RELEASE)
		{
			inputHandler.isWDown = false;
		}

		else if (!inputHandler.isADown && key == GLFW_KEY_A && action == GLFW_PRESS)
		{
			inputHandler.isADown = true;
			playerTank->SetDirection(0);
		}
		else if (inputHandler.isADown && key == GLFW_KEY_A && action == GLFW_RELEASE)
		{
			inputHandler.isADown = false;
		}

		else if (!inputHandler.isSDown && key == GLFW_KEY_S && action == GLFW_PRESS)
		{
			inputHandler.isSDown = true;
			playerTank->SetDirection(3);
		}
		else if (inputHandler.isSDown && key == GLFW_KEY_S && action == GLFW_RELEASE)
		{
			inputHandler.isSDown = false;
		}

		else if (!inputHandler.isDDown && key == GLFW_KEY_D && action == GLFW_PRESS)
		{
			inputHandler.isDDown = true;
			playerTank->SetDirection(2);
		}
		else if (inputHandler.isDDown && key == GLFW_KEY_D && action == GLFW_RELEASE)
		{
			inputHandler.isDDown = false;
		}

		if (!inputHandler.isSpaceDown && key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		{
			inputHandler.isSpaceDown = true;
		}
		else if (inputHandler.isSpaceDown && key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
		{
			inputHandler.isSpaceDown = false;
		}

		else if (!inputHandler.isShiftDown && key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS)
		{
			inputHandler.isShiftDown = true;
		}
		else if (inputHandler.isShiftDown && key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE)
		{
			inputHandler.isShiftDown = false;
		}
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (!editMode)
	{
		camera.MoveCamera(xpos, ypos);
	}
}

bool InitializerShaders();
void LoadModels();

int main(void)
{
	GLFWwindow* window;

	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
	{
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	// Initialize our window
	window = glfwCreateWindow(windowWidth, windowHeight, "Midterm", NULL, NULL);

	// Make sure the window initialized properly
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(window, key_callback); // Tell GLFW where our key callbacks are
	glfwSetCursorPosCallback(window, mouse_callback); // Tell GLFW where our mouse callback is

	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc) glfwGetProcAddress); // Give glad this process ID
	glfwSwapInterval(1);

	if (!InitializerShaders())
	{
		return -1;
	}

	LoadModels();

	CompiledShader shader = *gShaderManager.pGetShaderProgramFromFriendlyName("Shader#1");

	float fpsFrameCount = 0.f;
	float fpsTimeElapsed = 0.f;

	float previousTime = static_cast<float>(glfwGetTime());

	LightManager::GetInstance()->AddLight(shader, "sun", glm::vec3(30.0f, 10.0f, 70.0f));
	Light* sun = LightManager::GetInstance()->GetLight("sun");
	sun->EditLightType(Light::LightType::DIRECTIONAL, 100.0f, 150.0f);
	sun->EditDirection(0.0001f, -0.99f, 0.0001f, 1.0f);

	camera.position = glm::vec3(50.0f, 100.0f, 30.0f);
	camera.direction = glm::vec3(0.0001f, -0.99f, 0.01f);

	Maze maze("maze", "maze.txt");

	// Initialze tanks
	TankBuilder* tankBuilder = TankBuilder::GetInstance();
	std::vector<ITank*> tanks;

	glm::vec4 colors[Maze::maxSpawnPoints];
	colors[0] = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	colors[1] = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	colors[2] = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
	colors[3] = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
	colors[4] = glm::vec4(0.5f, 0.5f, 0.0f, 1.0f);
	colors[5] = glm::vec4(0.0f, 0.6f, 0.8f, 1.0f);

	std::vector<glm::vec2> spawnPoints = maze.GetSpawnPoints();

	// Make player tank first
	glm::vec2& playerSpawnPoint = spawnPoints[0];
	playerTank = static_cast<PlayerTank*>(tankBuilder->MakeTank(glm::vec3(playerSpawnPoint.x, 0.0f, playerSpawnPoint.y), colors[0], "", true, &inputHandler));
	tanks.push_back(playerTank);

	for (int i = 1; i < Maze::maxSpawnPoints; i++)
	{
		std::string weaponType = "bullet"; // TODO: Random chance between bullet & laser
		const glm::vec2& spawnPoint = spawnPoints[i];
		tanks.push_back(tankBuilder->MakeTank(glm::vec3(spawnPoint.x, 0.0f, spawnPoint.y), colors[i], weaponType));
	}

	// Setup mediator
	TankMediator mediator;
	mediator.SetMaze(&maze);
	for (ITank* tank : tanks)
	{
		mediator.AddTank(tank);
	}
	
	
	// Our actual render loop
	while (!glfwWindowShouldClose(window))
	{
		float currentTime = static_cast<float>(glfwGetTime());
		float deltaTime = currentTime - previousTime;
		previousTime = currentTime;

		// FPS TITLE
		{
			fpsTimeElapsed += deltaTime;
			fpsFrameCount += 1.0f;
			if (fpsTimeElapsed >= 0.03f)
			{
				std::string fps = std::to_string(fpsFrameCount / fpsTimeElapsed);
				std::string ms = std::to_string(1000.f * fpsTimeElapsed / fpsFrameCount);
				std::string newTitle = "FPS: " + fps + "   MS: " + ms;
				glfwSetWindowTitle(window, newTitle.c_str());

	
				fpsTimeElapsed = 0.f;
				fpsFrameCount = 0.f;
			}
		}

		float ratio;
		int width, height;
		glm::mat4 projection;
		glm::mat4 view;

		glfwGetFramebufferSize(window, &width, &height); // Assign width and height to our window width and height
		ratio = width / (float)height;

		glEnable(GL_DEPTH);
		glEnable(GL_DEPTH_TEST); // Enables the Depth Buffer, which decides which pixels will be drawn based on their depth (AKA don't draw pixels that are behind other pixels)

		glViewport(0, 0, width, height); // Specifies the transformation of device coords to window coords 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clears the buffers

		view = camera.GetViewMatrix();
		projection = glm::perspective(0.6f, ratio, 0.1f, 1000.0f);

		shader.Bind();
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "matView"), 1, GL_FALSE, glm::value_ptr(view)); // Assign new view matrix
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "matProjection"), 1, GL_FALSE, glm::value_ptr(projection)); // Assign projection
		glUniform4f(glGetUniformLocation(shader.ID, "cameraPosition"), camera.position.x, camera.position.y, camera.position.z, 1.0f);

		// Safety, mostly for first frame
		if (deltaTime == 0.0f)
		{
			deltaTime = 0.03f;
		}

		mediator.UpdateProjectiles(shader, deltaTime);

		std::vector<ITank*> invalidTanks;
		for (ITank* tank : tanks)
		{
			tank->OnUpdate(deltaTime);
			const glm::vec3& position = tank->GetPosition();
			const glm::mat4& rotation = tank->GetRotation();
			Model* model = ModelManager::GetInstance()->GetModel(tank->GetModelName());
			model->SetIsOverrideColor(true);
			model->SetColorOverride(tank->GetColor());
			ModelManager::GetInstance()->Draw(tank->GetModelName(), shader, glm::vec3(position.x, position.y, position.z), rotation[0], rotation[1], rotation[2], glm::vec3(0.3f, 0.3f, 0.3f));

			// Draw healthbars
			float healthPercent = tank->GetHealth() / tank->GetMaxHealth();
			glm::vec4 color; 
			if (healthPercent >= 0.8f)
			{
				color = glm::vec4(0.0, 1.0f, 0.0f, 1.0f);
			}
			else if (healthPercent >= 0.6f)
			{
				color = glm::vec4(0.4, 0.8f, 0.0f, 1.0f);
			}
			else if (healthPercent >= 0.4f)
			{
				color = glm::vec4(0.8, 0.8f, 0.0f, 1.0f);
			}
			else if (healthPercent >= 0.2f)
			{
				color = glm::vec4(0.8, 0.4f, 0.0f, 1.0f);
			}
			else
			{
				color = glm::vec4(1.0, 0.0f, 0.0f, 1.0f);
			}

			Model* healthModel = ModelManager::GetInstance()->GetModel("box");
			healthModel->SetIsOverrideColor(true);
			healthModel->SetColorOverride(color);

			ModelManager::GetInstance()->Draw("box", shader, glm::vec3(position.x, position.y + 2.0, position.z), rotation[0], rotation[1], rotation[2], glm::vec3(healthPercent, 0.3f, 0.3f));

			if (tank->GetHealth() <= 0.0f)
			{
				invalidTanks.push_back(tank);
			}
		}

		for (ITank* tank : invalidTanks)
		{
			std::vector<ITank*>::iterator it = std::find(tanks.begin(), tanks.end(), tank);
			if (it != tanks.end())
			{
				glm::vec3 pos = tank->GetPosition();
				playerTankRemoved = tank == playerTank;
				mediator.RemoveTank(tank);
				tanks.erase(it);
				delete tank;

				// When something dies, get the closes tank to the person who died and make them a super tank
				// Super hacky but im out of time :(
				float closestTank = 100000.0f;
				ITank* closest = NULL;
				for (ITank* aliveTank : tanks)
				{
					float distance = glm::distance(pos, aliveTank->GetPosition());
					if (distance < closestTank)
					{
						closestTank = distance;
						closest = aliveTank;
					}
				}

				if (closest)
				{
					closest->MakeSuperTank();
				}
			}
		}

		// Render the models
		ModelManager::GetInstance()->Draw("maze", shader, glm::vec3(0.0f, 0.0f, 60.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		
		// Draw lights
		for (Light* light : LightManager::GetInstance()->GetLights())
		{
			ModelManager::GetInstance()->Draw("sphere", shader, light->GetPosition(), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		}


		// Show what we've drawn
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Clean up tanks
	for (ITank* tank : tanks)
	{
		delete tank;
	}

	delete TankFactory::GetInstance();
	delete TankBuilder::GetInstance();

	ModelManager::GetInstance()->CleanUp();
	delete ModelManager::GetInstance();

	glfwDestroyWindow(window); // Clean up the window

	glfwTerminate(); 
	exit(EXIT_SUCCESS);
}


bool InitializerShaders()
{
	std::stringstream ss;

	// "Normal" Shader
	Shader vertexShader;
	ss << SOLUTION_DIR << "Extern\\assets\\shaders\\vertexShader.glsl";
	vertexShader.fileName = ss.str();
	ss.str("");

	Shader fragmentShader;
	ss << SOLUTION_DIR << "Extern\\assets\\shaders\\fragmentShader.glsl";
	fragmentShader.fileName = ss.str();
	ss.str("");

	bool success = gShaderManager.createProgramFromFile("Shader#1", vertexShader, fragmentShader);
	if (success)
	{
		std::cout << "Shaders compiled OK" << std::endl;
	}
	else
	{
		std::cout << "Error making shaders: " << std::endl;
		std::cout << gShaderManager.getLastError() << std::endl;
		return 1;
	}

	return success;
}

void LoadModels()
{
	{
		std::stringstream ss;
		ss << SOLUTION_DIR << "Extern\\assets\\models\\maze.ply";
		ModelManager::GetInstance()->LoadModel(ss.str(), "maze");
	}
	{
		std::stringstream ss;
		ss << SOLUTION_DIR << "Extern\\assets\\models\\ISO_Sphere.ply";
		ModelManager::GetInstance()->LoadModel(ss.str(), "sphere");
	}
	{
		std::stringstream ss;
		ss << SOLUTION_DIR << "Extern\\assets\\models\\tank.ply";
		ModelManager::GetInstance()->LoadModel(ss.str(), "tank");
	}
	{
		std::stringstream ss;
		ss << SOLUTION_DIR << "Extern\\assets\\models\\box.ply";
		ModelManager::GetInstance()->LoadModel(ss.str(), "box");
	}
}
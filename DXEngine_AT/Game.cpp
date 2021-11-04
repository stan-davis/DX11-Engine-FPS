#include "Game.h"
#include <vector>
#include <iostream>
#include <fstream>

Game::Game(HINSTANCE hInstance) : DiectXApp(hInstance, "A DirectX11 Game", 800, 600) {}

void Game::Start() 
{
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	graphics = std::make_unique<Graphics>(device, context);

	CreateMapData("test_map.txt");

	camera = std::make_unique<Camera>(0.4f * 3.14f, static_cast<float>(windowWidth / windowHeight), 1.0f, 1000.0f);
}

void Game::Update(float delta)
{
	float move_speed = 10 * delta;
	float rotation_speed = 4 * delta;

	//Key input

	if (input->isPressed(KEYS::A))
	{
		cam_angle -= rotation_speed;
	}

	if (input->isPressed(KEYS::D))
	{
		cam_angle += rotation_speed;
	}

	if (input->isPressed(KEYS::W))
	{
		cam_x -= std::sinf(cam_angle) * move_speed;
		cam_z -= std::cosf(cam_angle) * move_speed;
	}

	if (input->isPressed(KEYS::S))
	{
		cam_x += std::sinf(cam_angle) * move_speed;
		cam_z += std::cosf(cam_angle) * move_speed;
	}

	//Adjust camera
	camera->Translate(cam_x, 0, cam_z);
	camera->Rotate(0, 1, 0, -cam_angle);
	camera->Update();
}

void Game::Draw(float delta)
{
	//Clear
	context->ClearRenderTargetView(renderTargetView.Get(), CLEAR_COLOR);
	context->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	//Camera update
	graphics->UpdateBufferData(camera->GetWorldProjectionMatrix());

	swapchain->Present(0, 0);
}

void Game::CreateMapData(std::string filePath)
{
	int mapWidth = 0;
	int mapHeight = 0;
	int cubeSize = 2;

	char* mapData = nullptr;

	std::ifstream data(filePath, std::ios::in | std::ios::binary);

	if (data.is_open())
	{
		OutputDebugString(L"Successfully opened level data\n");

		data >> mapWidth >> mapHeight;
		mapData = new char[mapWidth * mapHeight];

		for (int i = 0; i < mapWidth * mapHeight; i++)
		{
			data >> mapData[i];
		}
	}
	else
	{
		OutputDebugString(L"Failed to open level data\n");
	}

	for(int x = 0; x < mapWidth; x++)
		for (int z = 0; z < mapHeight; z++)
		{
			char index = mapData[z * mapWidth + x];

			if (index == '#')
				graphics->CreateTestCube(x * cubeSize, 0.0f, z * cubeSize);
		}

	delete[] mapData;
}

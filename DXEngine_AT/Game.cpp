#include "Game.h"
#include <vector>
#include <iostream>
#include <fstream>
#include "Logger.h"

Game::Game(HINSTANCE hInstance) : DiectXApp(hInstance, "A DirectX11 Game", 800, 600) {}

void Game::Start() 
{
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	graphics = std::make_unique<Graphics>(device, context);

	CreateMapData("test_map.txt");
}

void Game::Update(float delta)
{
	//some camera vars
	float move_speed = 10 * delta;
	float rotation_speed = 4 * delta;

	//Key input
	if (input->isPressed(KEYS::W))
	{
		camera->MoveForward(move_speed);
	}

	if (input->isPressed(KEYS::S))
	{
		camera->MoveForward(-move_speed);
	}

	if (input->isPressed(KEYS::D))
	{
		camera->RotateYAW(rotation_speed);
	}

	if (input->isPressed(KEYS::A))
	{
		camera->RotateYAW(-rotation_speed);
	}

	camera->Update();

	for (auto& e : entities)
	{
		e->Update();
		e->BillboardUpdate(camera->GetTransform());
	}
}

void Game::Draw(float delta)
{
	//Clear
	context->ClearRenderTargetView(renderTargetView.Get(), CLEAR_COLOR);
	context->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	//Buffer Update
	for (auto& e : entities)
		e->Draw();
	
	graphics->UpdateBufferData(entities, camera->GetCameraMatrix());
	
	//Present
	swapchain->Present(0, 0);
}

void Game::CreateMapData(std::string filePath)
{
	int mapWidth = 0;
	int mapHeight = 0;
	float cubeSize = 2;

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

	wallModel = Mesh("models/cube_wall.obj", L"textures/wall_brick.png", device);

	//Draw map
	for(int x = 0; x < mapWidth; x++)
		for (int z = 0; z < mapHeight; z++)
		{
			char index = mapData[z * mapWidth + x];
			Vector3 pos = { static_cast<float>(x * cubeSize), 0, static_cast<float>(z * cubeSize) };
			//Entity e = Entity(wallModel);

			switch (index)
			{
			case '#':

				break;
			case '@':
				//Create player camera
				camera = std::make_unique<Camera>(0.4f * 3.14f, static_cast<float>(windowWidth / windowHeight), 1.0f, 1000.0f);
				camera->Translate(pos);
				break;
			case 'e':

				break;
			}
		}

	delete[] mapData;
}

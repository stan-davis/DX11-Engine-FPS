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
	bool is_colliding = false;
	Vector3 camera_previous = camera->GetTransform();

	//Update Camera
	if (input->isPressed(KEYS::W))
		camera->MoveForward(1, delta);
	if (input->isPressed(KEYS::S))
		camera->MoveForward(-1, delta);
	if (input->isPressed(KEYS::D)) 
		camera->RotateYAW(1, delta);
	if (input->isPressed(KEYS::A)) 
		camera->RotateYAW(-1, delta);

	camera->Update();
	
	//Update Entities
	for (auto& e : entities)
	{
		e->BillboardUpdate(camera->GetTransform());

		if (camera->GetCollider().RectCollision(e->GetCollider().GetColliderObject()))
		{
			//Resolve collision
			camera->Translate(camera_previous);
		}
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
	enemyModel = Mesh("models/billboard_plane.obj", L"textures/demon.png", device);

	//Draw map
	for(int x = 0; x < mapWidth; x++)
		for (int z = 0; z < mapHeight; z++)
		{
			char index = mapData[z * mapWidth + x];

			Entity temp;
			Vector3 pos = { static_cast<float>(x * cubeSize), 0, static_cast<float>(z * cubeSize) };
			Collider col = Collider({ cubeSize, 0, cubeSize});

			switch (index)
			{
			case '#':
				temp = Entity(wallModel);
				temp.Translate(pos);
				temp.SetCollider(col);
				temp.Update(); //update aabb
				break;
			case '@':
				//Create player camera
				camera = std::make_unique<Camera>(70, static_cast<float>(windowWidth / windowHeight), 1, 1000);
				camera->Translate(pos);
				temp.SetCollider(col);
				break;
			case 'e':
				temp = Entity(enemyModel);
				temp.Translate(pos);
				temp.IsBillboard(true);
				temp.SetCollider(col);
				temp.Update();
				break;
			}

			entities.push_back(std::make_unique<Entity>(temp));
		}

	delete[] mapData;
}

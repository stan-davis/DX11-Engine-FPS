#define NOMINMAX

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
	//Camera Input
	if (input->isPressed(KEYS::W))
		player->Move({ 1,0,1 }, delta);
	if (input->isPressed(KEYS::S))
		player->Move({ -1,0,-1 }, delta);
	if (input->isPressed(KEYS::A))
		player->RotateYAW(-1, delta);
	if (input->isPressed(KEYS::D))
		player->RotateYAW(1, delta);
	
	Vector3 target = player->GetTarget();

	//Update Entities
	for (auto& e : entities)
	{
		Vector3 distance = e->GetTransform() - player->GetTransform();

		if (distance.Magnitude() < ENTITY_UPDATE_DISTANCE) //Only update entities within range
		{
			if(e->IsBillboard()) e->SetCameraDistance(distance);
			e->Update();
			target = player->GetCollider().CircleRectCollision(target, e->GetCollider().GetColliderObject());
		}
	}

	//Update Camera
	player->Translate(target);
	player->Update();
}

void Game::Draw(float delta)
{
	//Clear
	context->ClearRenderTargetView(renderTargetView.Get(), CLEAR_COLOR);
	context->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	//Buffer Update
	for (auto& e : entities)
		e->Draw();
	
	graphics->UpdateBufferData(entities, player->GetCamera().GetCameraMatrix());
	
	//Present
	swapchain->Present(0, 0);
}

void Game::CreateMapData(std::string filePath)
{
	int mapWidth = 0;
	int mapHeight = 0;
	float tileSize = 2;

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
			Vector3 position = { static_cast<float>(x * tileSize), 0, static_cast<float>(z * tileSize) };

			switch (index)
			{
			case '#':
				temp = Entity(wallModel);
				temp.Translate(position);
				temp.SetCollider(Collider({ tileSize, 0, tileSize }));
				temp.MakeStatic();
				break;
			case '@':
				//Create player entity
				player = std::make_unique<Player>(CameraComponent(70, static_cast<float>(windowWidth / windowHeight), 1, 1000));
				player->SetTarget(position);
				player->SetCollider(Collider(0.5f));
				break;
			case 'e':
				temp = Entity(enemyModel);
				temp.Translate(position);
				temp.MakeBillboard();
				temp.SetCollider(Collider({ 1,0,1 }));
				break;
			}

			entities.push_back(std::make_unique<Entity>(temp));
		}

	delete[] mapData;
}

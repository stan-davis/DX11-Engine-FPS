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
	bool shoot = false;

	//Raycast bullet
	if (input->isPressed(KEYS::SPACE))
	{
		shoot = true;
	}

	//Update Entities
	for (auto& e : entities)
	{
		if (e->IsActive())
		{
			Vector3 distance = e->GetTransform() - player->GetTransform();

			if (distance.Magnitude() < ENTITY_UPDATE_DISTANCE) //Only update entities within range
			{
				if (shoot && e->Tag() == "enemy")
				{
					if (RayCastHit(e->GetCollider().GetColliderObject()))
					{
						e->SetActive(false);
						shoot = false;
					}
				}

				if(e->IsBillboard()) e->SetCameraDistance(distance);
				e->Update();

				if (player->GetCollider().CircleRectCollision(target, e->GetCollider().GetColliderObject(), target))
				{
					if (e->Tag() == "door")
					{
						e->SetActive(false);
					}
				}
			}
		}
	}

	//Update Camera
	player->Translate(target);
	player->Update();
}

bool Game::RayCastHit(Collider::RectColliderObject o)
{
	const float rayLength = 5;

	Entity ray = Entity();
	ray.Translate(player->GetTransform());
	ray.SetCollider(Collider(1));

	Vector3 playerDirection = player->GetDirection();
	Vector3 f = { 0,0,0 };

	for (int i = 0; i < rayLength; i++)
	{
		ray.Translate(ray.GetTransform() + (playerDirection * i));

		if (ray.GetCollider().CircleRectCollision(ray.GetTransform(), o, f))
		{
			return true;
		}
	}

	return false;
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
	
	modelWall = Mesh("models/cube_wall.obj", L"textures/wall_brick.png", device);
	modelDoor = Mesh("models/cube_wall.obj", L"textures/door.png", device);
	modelEnemy = Mesh("models/billboard_plane.obj", L"textures/demon.png", device);
	modelFire = Mesh("models/billboard_plane.obj", L"textures/gun_fire.png", device);

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
				temp = Entity(modelWall);
				temp.Translate(position);
				temp.SetCollider(Collider({ tileSize, 0, tileSize }));
				temp.MakeStatic();
				break;
			case 'd':
				temp = Entity(modelDoor);
				temp.Translate(position);
				temp.SetCollider(Collider({ tileSize, 0, tileSize }));
				temp.MakeStatic();
				temp.SetTag("door");
				break;
			case '@':
				//Create player entity
				player = std::make_unique<Player>(CameraComponent(70, static_cast<float>(windowWidth / windowHeight), 1, 1000));
				player->SetTarget(position);
				player->SetCollider(Collider(0.5f));
				break;
			case 'e':
				temp = Entity(modelEnemy);
				temp.Translate(position);
				temp.MakeBillboard();
				temp.SetCollider(Collider({ 1,0,1 }));
				temp.SetTag("enemy");
				break;
			}

			entities.push_back(std::make_unique<Entity>(temp));
		}

	delete[] mapData;
}

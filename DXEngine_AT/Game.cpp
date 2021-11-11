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
}

void Game::Update(float delta)
{
	//some camera vars
	float move_speed = 10 * delta;
	float rotation_speed = 4 * delta;

	//Key input
	if (input->isPressed(KEYS::W))
	{
		camera->Translate(Vector3(move_speed,0, move_speed));
	}

	if (input->isPressed(KEYS::S))
	{
		camera->Translate(Vector3(-move_speed, 0, -move_speed));
	}

	if (input->isPressed(KEYS::D))
	{
		camera->RotateYAW(rotation_speed);
	}

	if (input->isPressed(KEYS::A))
	{
		camera->RotateYAW(-rotation_speed);
	}
}

void Game::Draw(float delta)
{
	//Clear
	context->ClearRenderTargetView(renderTargetView.Get(), CLEAR_COLOR);
	context->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	//Buffer Update
	for (auto& e : entities)
	{
		e.Draw();
	}
	
	graphics->UpdateBufferData(entities, camera->GetCameraMatrix());
	
	//Present
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

	//Cube Shape
	std::vector<Mesh::Vertex> verts =
	{
		// Front Face
		 Mesh::Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f),
		 Mesh::Vertex(-1.0f,  1.0f, -1.0f, 0.0f, 0.0f),
		 Mesh::Vertex(1.0f,  1.0f, -1.0f, 1.0f, 0.0f),
		 Mesh::Vertex(1.0f, -1.0f, -1.0f, 1.0f, 1.0f),

		 // Back Face
		 Mesh::Vertex(-1.0f, -1.0f, 1.0f, 1.0f, 1.0f),
		 Mesh::Vertex(1.0f, -1.0f, 1.0f, 0.0f, 1.0f),
		 Mesh::Vertex(1.0f,  1.0f, 1.0f, 0.0f, 0.0f),
		 Mesh::Vertex(-1.0f,  1.0f, 1.0f, 1.0f, 0.0f),

		 // Top Face
		 Mesh::Vertex(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f),
		 Mesh::Vertex(-1.0f, 1.0f,  1.0f, 0.0f, 0.0f),
		 Mesh::Vertex(1.0f, 1.0f,  1.0f, 1.0f, 0.0f),
		 Mesh::Vertex(1.0f, 1.0f, -1.0f, 1.0f, 1.0f),

		 // Bottom Face
		 Mesh::Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f),
		 Mesh::Vertex(1.0f, -1.0f, -1.0f, 0.0f, 1.0f),
		 Mesh::Vertex(1.0f, -1.0f,  1.0f, 0.0f, 0.0f),
		 Mesh::Vertex(-1.0f, -1.0f,  1.0f, 1.0f, 0.0f),

		 // Left Face
		 Mesh::Vertex(-1.0f, -1.0f,  1.0f, 0.0f, 1.0f),
		 Mesh::Vertex(-1.0f,  1.0f,  1.0f, 0.0f, 0.0f),
		 Mesh::Vertex(-1.0f,  1.0f, -1.0f, 1.0f, 0.0f),
		 Mesh::Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f),

		 // Right Face
		 Mesh::Vertex(1.0f, -1.0f, -1.0f, 0.0f, 1.0f),
		 Mesh::Vertex(1.0f,  1.0f, -1.0f, 0.0f, 0.0f),
		 Mesh::Vertex(1.0f,  1.0f,  1.0f, 1.0f, 0.0f),
		 Mesh::Vertex(1.0f, -1.0f,  1.0f, 1.0f, 1.0f),
	};

	std::vector<DWORD> ind =
	{
		// Front Face
		0,  1,  2,
		0,  2,  3,

		// Back Face
		4,  5,  6,
		4,  6,  7,

		// Top Face
		8,  9, 10,
		8, 10, 11,

		// Bottom Face
		12, 13, 14,
		12, 14, 15,

		// Left Face
		16, 17, 18,
		16, 18, 19,

		// Right Face
		20, 21, 22,
		20, 22, 23
	};

	//Wall
	Mesh::Primitive CUBE = Mesh::Primitive(verts, ind);
	Mesh wallMesh = Mesh(CUBE, L"texture_stone.png", device);

	//Enemy stuff
	Mesh::Primitive PLANE = Mesh::Primitive(verts, ind);
	Mesh enemyMesh = Mesh(PLANE, L"demon.png", device);

	//Draw map
	for(int x = 0; x < mapWidth; x++)
		for (int z = 0; z < mapHeight; z++)
		{
			char index = mapData[z * mapWidth + x];

			Entity e;

			switch (index)
			{
			case '#':
				e = Entity(wallMesh);
				e.Translate({ static_cast<float>(x * cubeSize), 0.0f, static_cast<float>(z * cubeSize) });
				break;
			case '@':
				//Create player camera
				camera = std::make_unique<Camera>(0.4f * 3.14f, static_cast<float>(windowWidth / windowHeight), 1.0f, 1000.0f);
				camera->Translate(Vector3(x * cubeSize, 0, z * cubeSize));
				break;
			case 'e':
				e = Entity(enemyMesh);
				e.Translate({ static_cast<float>(x * cubeSize), 0.0f, static_cast<float>(z * cubeSize) });
				enemyIndex = index;
				break;
			}

			entities.push_back(e);
		}

	delete[] mapData;
}

#pragma once

#include "DirectXApp.h"
#include "Graphics.h"
#include "Entity.h"
#include "Player.h"

class Game : public DiectXApp
{
public:
	Game(HINSTANCE hInstance);
	~Game() = default;

	void Start();
	void Update(float delta);
	void Draw(float delta);

private:
	std::unique_ptr<Graphics> graphics;
	std::unique_ptr<Player> player;

	//Level
	void CreateMapData(std::string filePath);
	std::vector<std::unique_ptr<Entity>> entities;

	//Meshes
	Mesh wallModel;
	Mesh enemyModel;

	//Important
	const float CLEAR_COLOR[4] = { 0.2f, 0.6f, 0.8f, 1.0f };
	const float ENTITY_UPDATE_DISTANCE = 20;
};

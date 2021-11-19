#pragma once

#include "DirectXApp.h"
#include "Graphics.h"
#include "Entity.h"
#include "Player.h"
#include "Bullet.h"

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

	//Important
	const float CLEAR_COLOR[4] = { 0.2f, 0.6f, 0.8f, 1.0f };

	//Game
	const float ENTITY_UPDATE_DISTANCE = 20;
	
	//Level
	void CreateMapData(std::string filePath);
	bool RayCastHit(Collider::RectColliderObject o);
	std::unique_ptr<Player> player;
	std::vector<std::unique_ptr<Entity>> entities;
	std::vector<std::unique_ptr<Bullet>> bullets;

	//Meshes
	Mesh modelWall;
	Mesh modelDoor;
	Mesh modelEnemy;
	Mesh modelFire;
};

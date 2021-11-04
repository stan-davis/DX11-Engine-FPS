#pragma once

#include "DirectXApp.h"
#include "Graphics.h"
#include "Camera.h"

class Game : public DiectXApp
{
public:
	Game(HINSTANCE hInstance);
	~Game() = default;

	void Start();
	void Update(float delta);
	void Draw(float delta);

private:
	const float CLEAR_COLOR[4] = { 0.0f, 0.2f, 0.4f, 1.0f };

	std::unique_ptr<Graphics> graphics;
	std::unique_ptr<Camera> camera;

	//Level
	void CreateMapData(std::string filePath);

	//Testing
	float cam_x = 0.0f;
	float cam_z = 0.0f;
};


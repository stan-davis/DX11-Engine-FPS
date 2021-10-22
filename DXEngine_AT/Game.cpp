#include "Game.h"
#include <vector>

Game::Game(HINSTANCE hInstance) : DiectXApp(hInstance, "A DirectX11 Game", 800, 600) {}

void Game::Start() 
{
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	graphics = std::make_unique<Graphics>(device, context);

	//Cube Shape
	std::vector<Graphics::Vertex> verts =
	{
		{-1.0f, -1.0f, -1.0f, {1.0f, 0.0f, 0.0f, 1.0f}},
		{-1.0f, +1.0f, -1.0f, {0.0f, 1.0f, 0.0f, 1.0f}},
		{+1.0f, +1.0f, -1.0f, {0.0f, 0.0f, 1.0f, 1.0f}},
		{+1.0f, -1.0f, -1.0f, {1.0f, 1.0f, 0.0f, 1.0f}},
		{-1.0f, -1.0f, +1.0f, {0.0f, 1.0f, 1.0f, 1.0f}},
		{-1.0f, +1.0f, +1.0f, {1.0f, 1.0f, 1.0f, 1.0f}},
		{+1.0f, +1.0f, +1.0f, {1.0f, 0.0f, 1.0f, 1.0f}},
		{+1.0f, -1.0f, +1.0f, {1.0f, 0.0f, 0.0f, 1.0f}}
	};

	std::vector<DWORD> ind =
	{
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// left face
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		4, 0, 3,
		4, 3, 7
	};

	graphics->CreateMesh(verts, ind);

	camera = std::make_unique<Camera>(0.4f * 3.14f, static_cast<float>(windowWidth / windowHeight), 1.0f, 1000.0f);
}

void Game::Update(float delta)
{
	//KEYBOARD INPUT
	if (input->isPressed(KEYS::D))
	{
		cam_x -= 5 * delta;
	}

	if (input->isPressed(KEYS::A))
	{
		cam_x += 5 * delta;
	}

	if (input->isPressed(KEYS::W))
	{
		cam_z -= 5 * delta;
	}

	if (input->isPressed(KEYS::S))
	{
		cam_z += 5 * delta;
	}

	camera->Translate(cam_x, 0.0f, cam_z);
	camera->Update();
}

void Game::Draw(float delta)
{
	//Clear
	context->ClearRenderTargetView(renderTargetView.Get(), CLEAR_COLOR);
	context->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	//Camera update
	graphics->UpdateBufferData(camera->GetWorldProjectionMatrix());

	context->DrawIndexed(36, 0, 0);

	swapchain->Present(0, 0);
}
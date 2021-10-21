#pragma once

#include <wrl/client.h>
#include <d3d11.h>
#include <DirectXMath.h>

namespace dx = DirectX;
namespace wrl = Microsoft::WRL;

class Camera
{
public:
	Camera(wrl::ComPtr<ID3D11Device> _device, wrl::ComPtr<ID3D11DeviceContext> _context, int _windowWidth, int _windowHeight);
	~Camera() = default;

	void Update();
	void Render();

	void Translate(float x, float y, float z);
	void Rotate(float x, float y, float z, float angle);

private:
	struct cBuffer
	{
		DirectX::XMMATRIX WVP;
	};

	//DirectX
	wrl::ComPtr<ID3D11Device> device;
	wrl::ComPtr<ID3D11DeviceContext> context;
	wrl::ComPtr<ID3D11Buffer> constantBuffer;

	//Window
	int windowWidth;
	int windowHeight;

	//World Matrices
	dx::XMMATRIX WVP;
	dx::XMMATRIX World;

	dx::XMMATRIX Rotation;
	dx::XMMATRIX Scale;
	dx::XMMATRIX Translation;

	//Camera
	dx::XMMATRIX camView;
	dx::XMMATRIX camProjection;

	dx::XMVECTOR camPosition;
	dx::XMVECTOR camTarget;
	dx::XMVECTOR camUp;

	cBuffer cBuff;
};


#pragma once
#include <d3dcompiler.h>
#include "Entity.h"

#pragma comment(lib, "d3dcompiler.lib")

namespace wrl = Microsoft::WRL;

class Graphics
{
public:
	Graphics(wrl::ComPtr<ID3D11Device> _device, wrl::ComPtr<ID3D11DeviceContext> _context);
	~Graphics() = default;
	
	void UpdateBufferData(std::vector<Entity>& entities, DX::XMMATRIX cameraMatrix);

private:
	struct cBuffer
	{
		DirectX::XMMATRIX WVP;
	};

	void CreateConstantBuffer();
	void CreateVertexShader(LPCWSTR filePath, LPCSTR entryPoint);
	void CreatePixelShader(LPCWSTR filePath, LPCSTR entryPoint);
	void SetInputLayout();

	//DirectX
	wrl::ComPtr<ID3D11Device> device;
	wrl::ComPtr<ID3D11DeviceContext> context;
	wrl::ComPtr<ID3D11InputLayout> inputLayout;

	//Shaders
	wrl::ComPtr<ID3D11VertexShader> vertexShader;
	wrl::ComPtr<ID3D11PixelShader> pixelShader;

	//Buffers
	ID3D10Blob* VS;
	ID3D10Blob* PS;

	wrl::ComPtr<ID3D11Buffer> constantBuffer;
	cBuffer localConstantBuffer;
};


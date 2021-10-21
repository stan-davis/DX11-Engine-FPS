#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3dcompiler.lib")

#include <wrl/client.h>
#include <vector>

namespace wrl = Microsoft::WRL;

class Graphics
{
public:
	struct Vertex
	{
		float x, y, z;
		float color[4];
	};

	Graphics(wrl::ComPtr<ID3D11Device> _device, wrl::ComPtr<ID3D11DeviceContext> _context);
	~Graphics() = default;

	HRESULT CreateVertexShader(LPCWSTR filePath, LPCSTR entryPoint);
	HRESULT CreatePixelShader(LPCWSTR filePath, LPCSTR entryPoint);
	HRESULT SetInputLayout();

	HRESULT CreateMesh(std::vector<Vertex> _vertices, std::vector<DWORD> _indices);

private:
	HRESULT CreateIndexBuffer();
	HRESULT CreateVertexBuffer();

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
	wrl::ComPtr<ID3D11Buffer> indexBuffer;
	wrl::ComPtr<ID3D11Buffer> vertexBuffer;

	//Mesh
	std::vector<Vertex> vertices;
	std::vector<DWORD> indices;
};


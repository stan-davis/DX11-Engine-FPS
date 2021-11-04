#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <d3dcompiler.h>

#include <vector>
#include <DirectXMath.h>

#pragma comment(lib, "d3dcompiler.lib")

namespace wrl = Microsoft::WRL;
namespace dx = DirectX;

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
	
	void UpdateBufferData(dx::XMMATRIX bufferData);

	void CreateTestCube(float x, float y, float z);

	HRESULT CreateMesh(std::vector<Vertex> _vertices, std::vector<DWORD> _indices);

private:
	struct cBuffer
	{
		DirectX::XMMATRIX WVP;
	};

	void CreateConstantBuffer();
	void CreateVertexShader(LPCWSTR filePath, LPCSTR entryPoint);
	void CreatePixelShader(LPCWSTR filePath, LPCSTR entryPoint);
	void SetInputLayout();

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

	wrl::ComPtr<ID3D11Buffer> constantBuffer;
	cBuffer localConstantBuffer;

	//Mesh (Split off into mesh class)
	std::vector<Vertex> vertices;
	std::vector<DWORD> indices;

	//Objects
	std::vector<dx::XMMATRIX> objectMatrices;
};


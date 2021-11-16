#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <DirectXMath.h>
#include <vector>
#include <string>

namespace wrl = Microsoft::WRL;

class Mesh
{
public:
	struct Vertex
	{
		Vertex() {}
		Vertex(float x, float y, float z, float u, float v) : position(x, y, z), texCoord(u, v) {}

		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texCoord;

		bool operator==(const Vertex& other) const
		{
			return position.x == other.position.x && position.y == other.position.y && position.z == other.position.z && texCoord.x == other.texCoord.x && texCoord.y == other.texCoord.y;
		}
	};
	
	struct Primitive
	{
		Primitive() {}
		Primitive(std::vector<Vertex> _vertices, std::vector<DWORD> _indices) : _vertices(_vertices), _indices(_indices) {}

		std::vector<Vertex> _vertices;
		std::vector<DWORD> _indices;
	};

	Mesh() = default;
	Mesh(std::vector<Vertex> _vertices, std::vector<DWORD> _indices, std::wstring _texturePath, wrl::ComPtr<ID3D11Device> _device);
	Mesh(Primitive primitive, std::wstring _texturePath, wrl::ComPtr<ID3D11Device> _device);
	Mesh(std::string _modelPath, std::wstring _texturePath, wrl::ComPtr<ID3D11Device> _device);

	~Mesh() = default;

	//Getters
	wrl::ComPtr<ID3D11Buffer> GetIndexBuffer() { return indexBuffer; }
	wrl::ComPtr<ID3D11Buffer> GetVertexBuffer() { return vertexBuffer; }

	std::vector<Vertex> GetVertices() { return vertices; }
	std::vector<DirectX::XMFLOAT3> GetRawVertices() { return rawVertices; }
	std::vector<DWORD> GetIndices() { return indices; }
	wrl::ComPtr<ID3D11ShaderResourceView> GetTexture() { return texture; }
	wrl::ComPtr<ID3D11SamplerState> GetSamplerState() { return textureSamplerState; }

private:
	void Initilize();

	//DirectX
	wrl::ComPtr<ID3D11Device> device;

	//Buffers
	wrl::ComPtr<ID3D11Buffer> indexBuffer;
	wrl::ComPtr<ID3D11Buffer> vertexBuffer;

	//Required
	std::vector<Vertex> vertices;
	std::vector<DirectX::XMFLOAT3> rawVertices;
	std::vector<DWORD> indices;

	std::wstring texturePath;
	std::string modelPath;

	wrl::ComPtr<ID3D11ShaderResourceView> texture;
	wrl::ComPtr<ID3D11SamplerState> textureSamplerState;
};

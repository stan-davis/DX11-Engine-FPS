#define TINYOBJLOADER_IMPLEMENTATION

#include "Mesh.h"
#include "tiny_obj_loader.h"
#include <WICTextureLoader.h>
#include <unordered_map>

Mesh::Mesh(std::vector<Vertex> _vertices, std::vector<DWORD> _indices, std::wstring _texturePath, wrl::ComPtr<ID3D11Device> _device) : vertices(_vertices), indices(_indices), texturePath(_texturePath), device(_device)
{
	Initilize();
}

Mesh::Mesh(Primitive primitive, std::wstring _texturePath, wrl::ComPtr<ID3D11Device> _device) : vertices(primitive._vertices), indices(primitive._indices), texturePath(_texturePath), device(_device)
{
	Initilize();
}

Mesh::Mesh(std::string _modelPath, std::wstring _texturePath, wrl::ComPtr<ID3D11Device> _device) : modelPath(_modelPath), texturePath(_texturePath), device(_device)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, modelPath.c_str()))
	{
		throw std::runtime_error(warn + err);
		OutputDebugString(L"Failed to load obj\n");
	}
	else
	{
		OutputDebugString(L"Obj loaded successfully!\n");
	}

	for (const auto& s : shapes)
		for (const auto& index : s.mesh.indices)
		{
			Vertex vertex;

			vertex.position =
			{
					attrib.vertices[3 * size_t(index.vertex_index) + 0],
					attrib.vertices[3 * size_t(index.vertex_index) + 1],
					attrib.vertices[3 * size_t(index.vertex_index) + 2]
			};

			if (index.texcoord_index >= 0)
			{
				vertex.texCoord =
				{
						attrib.texcoords[2 * size_t(index.texcoord_index) + 0],
						1.0f - attrib.texcoords[2 * size_t(index.texcoord_index) + 1]
				};
			}

			rawVertices.push_back(vertex.position);
			vertices.push_back(vertex);
			indices.push_back(indices.size());
		}

	Initilize();
}



void Mesh::Initilize()
{
	//Create Index Buffer
	D3D11_BUFFER_DESC ibd = {};

	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.ByteWidth = sizeof(DWORD) * indices.size();
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA indData;
	indData.pSysMem = indices.data();

	device->CreateBuffer(&ibd, &indData, indexBuffer.GetAddressOf());

	//Create vertex buffer
	D3D11_BUFFER_DESC vbd = {};

	vbd.Usage = D3D11_USAGE_DEFAULT;
	vbd.ByteWidth = sizeof(Vertex) * vertices.size();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertData = {};
	vertData.pSysMem = vertices.data();

	device->CreateBuffer(&vbd, &vertData, vertexBuffer.GetAddressOf()); //Probably needs error checking

	//Create texture sampler
	DirectX::WIC_LOADER_IGNORE_SRGB;
	DirectX::CreateWICTextureFromFile(device.Get(), texturePath.c_str(), nullptr, texture.GetAddressOf());

	D3D11_SAMPLER_DESC sdc = {};

	sdc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sdc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sdc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sdc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sdc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sdc.MinLOD = 0;
	sdc.MaxLOD = D3D11_FLOAT32_MAX;

	device->CreateSamplerState(&sdc, textureSamplerState.GetAddressOf());
}

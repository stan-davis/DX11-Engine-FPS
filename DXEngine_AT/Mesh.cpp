#include "Mesh.h"
#include <WICTextureLoader.h>

Mesh::Mesh(std::vector<Vertex> _vertices, std::vector<DWORD> _indices, std::wstring texturePath, DirectX::XMFLOAT3 position, wrl::ComPtr<ID3D11Device> _device) : vertices(_vertices), indices(_indices), device(_device)
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

	//Set World Matrix
	matrix = DirectX::XMMatrixIdentity();
	matrix = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
}

#include "Graphics.h"

Graphics::Graphics(wrl::ComPtr<ID3D11Device> _device, wrl::ComPtr<ID3D11DeviceContext> _context) : device(_device), context(_context)
{
	CreateConstantBuffer();
	CreateVertexShader(L"BasicShader.hlsl", "VS");
	CreatePixelShader(L"BasicShader.hlsl", "PS");
	SetInputLayout();
}

void Graphics::CreateConstantBuffer()
{
	//Set Up Constant Buffer
	D3D11_BUFFER_DESC cbbd = {};

	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.ByteWidth = sizeof(cBuffer);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;

	device->CreateBuffer(&cbbd, nullptr, constantBuffer.GetAddressOf());
}

void Graphics::UpdateBufferData(DX::XMMATRIX bufferData)
{
	for (auto& mesh : meshVector)
	{
		//Set world translation/rotation/scale
		DX::XMMATRIX world = mesh.GetMatrix() * bufferData;
		localConstantBuffer.WVP = DX::XMMatrixTranspose(world);

		//Update Vertex Shader Constant Buffers
		context->UpdateSubresource(constantBuffer.Get(), 0, nullptr, &localConstantBuffer, 0, 0);
		context->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());

		//Update Pixel Shader Constant Buffers
		context->PSSetShaderResources(0, 1, mesh.GetTexture().GetAddressOf());
		context->PSSetSamplers(0, 1, mesh.GetSamplerState().GetAddressOf());

		//Draw the object
		context->DrawIndexed((UINT)std::size(mesh.GetIndices()), 0, 0);
	}
}

void Graphics::CreateTestCube(float x, float y, float z, std::wstring texturePath)
{
	//Cube Shape
	std::vector<Mesh::Vertex> verts =
	{
		// Front Face
		 Mesh::Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f),
		 Mesh::Vertex(-1.0f,  1.0f, -1.0f, 0.0f, 0.0f),
		 Mesh::Vertex(1.0f,  1.0f, -1.0f, 1.0f, 0.0f),
		 Mesh::Vertex(1.0f, -1.0f, -1.0f, 1.0f, 1.0f),

		 // Back Face
		 Mesh::Vertex(-1.0f, -1.0f, 1.0f, 1.0f, 1.0f),
		 Mesh::Vertex(1.0f, -1.0f, 1.0f, 0.0f, 1.0f),
		 Mesh::Vertex(1.0f,  1.0f, 1.0f, 0.0f, 0.0f),
		 Mesh::Vertex(-1.0f,  1.0f, 1.0f, 1.0f, 0.0f),

		 // Top Face
		 Mesh::Vertex(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f),
		 Mesh::Vertex(-1.0f, 1.0f,  1.0f, 0.0f, 0.0f),
		 Mesh::Vertex(1.0f, 1.0f,  1.0f, 1.0f, 0.0f),
		 Mesh::Vertex(1.0f, 1.0f, -1.0f, 1.0f, 1.0f),

		 // Bottom Face
		 Mesh::Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f),
		 Mesh::Vertex(1.0f, -1.0f, -1.0f, 0.0f, 1.0f),
		 Mesh::Vertex(1.0f, -1.0f,  1.0f, 0.0f, 0.0f),
		 Mesh::Vertex(-1.0f, -1.0f,  1.0f, 1.0f, 0.0f),

		 // Left Face
		 Mesh::Vertex(-1.0f, -1.0f,  1.0f, 0.0f, 1.0f),
		 Mesh::Vertex(-1.0f,  1.0f,  1.0f, 0.0f, 0.0f),
		 Mesh::Vertex(-1.0f,  1.0f, -1.0f, 1.0f, 0.0f),
		 Mesh::Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f),

		 // Right Face
		 Mesh::Vertex(1.0f, -1.0f, -1.0f, 0.0f, 1.0f),
		 Mesh::Vertex(1.0f,  1.0f, -1.0f, 0.0f, 0.0f),
		 Mesh::Vertex(1.0f,  1.0f,  1.0f, 1.0f, 0.0f),
		 Mesh::Vertex(1.0f, -1.0f,  1.0f, 1.0f, 1.0f),
	};

	std::vector<DWORD> ind =
	{
		// Front Face
		0,  1,  2,
		0,  2,  3,

		// Back Face
		4,  5,  6,
		4,  6,  7,

		// Top Face
		8,  9, 10,
		8, 10, 11,

		// Bottom Face
		12, 13, 14,
		12, 14, 15,

		// Left Face
		16, 17, 18,
		16, 18, 19,

		// Right Face
		20, 21, 22,
		20, 22, 23
	};

	Mesh mesh = Mesh(verts, ind, texturePath, DX::XMFLOAT3(x,y,z) , device);

	UINT stride = sizeof(Mesh::Vertex);
	UINT offset = 0;

	context->IASetIndexBuffer(mesh.GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);
	context->IASetVertexBuffers(0, 1, mesh.GetVertexBuffer().GetAddressOf(), &stride, &offset);

	meshVector.push_back(mesh);
}

void Graphics::CreateVertexShader(LPCWSTR filePath, LPCSTR entryPoint)
{
	D3DCompileFromFile(filePath, 0, 0, entryPoint, "vs_4_0", 0, 0, &VS, 0);

	device->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), nullptr, &vertexShader);
	context->VSSetShader(vertexShader.Get(), 0, 0);
}

void Graphics::CreatePixelShader(LPCWSTR filePath, LPCSTR entryPoint)
{
	D3DCompileFromFile(filePath, 0, 0, entryPoint, "ps_4_0", 0, 0, &PS, 0);

	device->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), nullptr, &pixelShader);
	context->PSSetShader(pixelShader.Get(), 0, 0);
}

void Graphics::SetInputLayout()
{
	D3D11_INPUT_ELEMENT_DESC ild[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	UINT elementCount = ARRAYSIZE(ild);

	device->CreateInputLayout(ild, elementCount, VS->GetBufferPointer(), VS->GetBufferSize(), inputLayout.GetAddressOf());

	context->IASetInputLayout(inputLayout.Get());
}

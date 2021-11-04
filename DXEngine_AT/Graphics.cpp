#include "Graphics.h"

Graphics::Graphics(wrl::ComPtr<ID3D11Device> _device, wrl::ComPtr<ID3D11DeviceContext> _context) : device(_device), context(_context)
{
	CreateConstantBuffer();
	CreateVertexShader(L"shaders.shader", "VShader");
	CreatePixelShader(L"shaders.shader", "PShader");
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

void Graphics::UpdateBufferData(dx::XMMATRIX bufferData)
{
	for (auto& m : objectMatrices)
	{
		dx::XMMATRIX world = m * bufferData;
		localConstantBuffer.WVP = dx::XMMatrixTranspose(world);
		context->UpdateSubresource(constantBuffer.Get(), 0, nullptr, &localConstantBuffer, 0, 0);
		context->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
		context->DrawIndexed((UINT)std::size(indices), 0, 0);
	}
}

void Graphics::CreateTestCube(float x, float y, float z)
{
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

	CreateMesh(verts, ind);

	//Create Matrix
	dx::XMMATRIX matrix = dx::XMMatrixIdentity();
	matrix = dx::XMMatrixTranslation(x, y, z);

	objectMatrices.push_back(matrix);
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
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	UINT elementCount = ARRAYSIZE(ild);

	device->CreateInputLayout(ild, elementCount, VS->GetBufferPointer(), VS->GetBufferSize(), inputLayout.GetAddressOf());
	context->IASetInputLayout(inputLayout.Get());
}

HRESULT Graphics::CreateIndexBuffer()
{
	HRESULT hr = S_OK;

	if (indices.empty())
		return E_FAIL;

	//Create & Set index buffer
	D3D11_BUFFER_DESC ibd = {};

	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.ByteWidth = sizeof(DWORD) * indices.size();
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA indData;
	indData.pSysMem = indices.data();

	hr = device->CreateBuffer(&ibd, &indData, indexBuffer.GetAddressOf());
	context->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	return hr;
}

HRESULT Graphics::CreateVertexBuffer()
{
	HRESULT hr = S_OK;

	//Create & Set vertex buffer
	D3D11_BUFFER_DESC vbd = {};

	vbd.Usage = D3D11_USAGE_DEFAULT;
	vbd.ByteWidth = sizeof(Vertex) * vertices.size();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertData = {};
	vertData.pSysMem = vertices.data();

	hr = device->CreateBuffer(&vbd, &vertData, vertexBuffer.GetAddressOf());

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	context->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);

	return hr;
}

HRESULT Graphics::CreateMesh(std::vector<Vertex> _vertices, std::vector<DWORD> _indices)
{
	HRESULT hr = S_OK;

	//Set verts & indices
	vertices = _vertices;
	indices = _indices;
	
	//Create buffers
	hr = CreateIndexBuffer();
	hr = CreateVertexBuffer();

	return hr;
}

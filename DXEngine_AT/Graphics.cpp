#include "Graphics.h"

Graphics::Graphics(wrl::ComPtr<ID3D11Device> _device, wrl::ComPtr<ID3D11DeviceContext> _context) : device(_device), context(_context)
{}

HRESULT Graphics::CreateVertexShader(LPCWSTR filePath, LPCSTR entryPoint)
{
	HRESULT hr = S_OK;

	D3DCompileFromFile(filePath, 0, 0, entryPoint, "vs_4_0", 0, 0, &VS, 0);
	hr = device->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), nullptr, &vertexShader);
	context->VSSetShader(vertexShader.Get(), 0, 0);

	return hr;
}

HRESULT Graphics::CreatePixelShader(LPCWSTR filePath, LPCSTR entryPoint)
{
	HRESULT hr = S_OK;

	D3DCompileFromFile(filePath, 0, 0, entryPoint, "ps_4_0", 0, 0, &PS, 0);
	hr = device->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), nullptr, &pixelShader);
	context->PSSetShader(pixelShader.Get(), 0, 0);

	return hr;
}

HRESULT Graphics::SetInputLayout()
{
	HRESULT hr = S_OK;

	D3D11_INPUT_ELEMENT_DESC ild[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	UINT elementCount = ARRAYSIZE(ild);

	hr = device->CreateInputLayout(ild, elementCount, VS->GetBufferPointer(), VS->GetBufferSize(), inputLayout.GetAddressOf());
	context->IASetInputLayout(inputLayout.Get());

	return hr;
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

	vertices = _vertices;
	indices = _indices;

	hr = CreateIndexBuffer();
	hr = CreateVertexBuffer();

	return hr;
}
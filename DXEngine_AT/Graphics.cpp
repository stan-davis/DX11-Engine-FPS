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

void Graphics::UpdateBufferData(std::vector<std::unique_ptr<Entity>>& entities, DX::XMMATRIX cameraMatrix)
{
	UINT stride = sizeof(Mesh::Vertex);
	UINT offset = 0;

	for (auto& e : entities)
	{
		//Set Buffers
		context->IASetIndexBuffer(e->GetMesh().GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);
		context->IASetVertexBuffers(0, 1, e->GetMesh().GetVertexBuffer().GetAddressOf(), &stride, &offset);

		//Set world translation/rotation/scale
		DX::XMMATRIX world = e->GetMatrix() * cameraMatrix;
		localConstantBuffer.WVP = DX::XMMatrixTranspose(world);

		//Update Vertex Shader Constant Buffers
		context->UpdateSubresource(constantBuffer.Get(), 0, nullptr, &localConstantBuffer, 0, 0);
		context->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());

		//Update Pixel Shader Constant Buffers
		context->PSSetShaderResources(0, 1, e->GetMesh().GetTexture().GetAddressOf());
		context->PSSetSamplers(0, 1, e->GetMesh().GetSamplerState().GetAddressOf());

		//Draw the object
		context->DrawIndexed((UINT)std::size(e->GetMesh().GetIndices()), 0, 0);
	}
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

#include "Graphics.h"

Graphics::Graphics(wrl::ComPtr<ID3D11Device> _device, wrl::ComPtr<ID3D11DeviceContext> _context) : device(_device), context(_context)
{
	CreateConstantBuffer();
	//CreateBlendState(); //Might be useful later
	SetCullModes();
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

void Graphics::CreateBlendState()
{
	//Setup blend state desc
	D3D11_BLEND_DESC bd = {};
	D3D11_RENDER_TARGET_BLEND_DESC rtbd = {};

	rtbd.BlendEnable = true;
	rtbd.SrcBlend = D3D11_BLEND_SRC_COLOR;
	rtbd.DestBlend = D3D11_BLEND_BLEND_FACTOR;
	rtbd.BlendOp = D3D11_BLEND_OP_ADD;
	rtbd.SrcBlendAlpha = D3D11_BLEND_ONE;
	rtbd.DestBlendAlpha = D3D11_BLEND_ZERO;
	rtbd.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	rtbd.RenderTargetWriteMask = D3D10_COLOR_WRITE_ENABLE_ALL;

	bd.AlphaToCoverageEnable = false;
	bd.RenderTarget[0] = rtbd;

	device->CreateBlendState(&bd, blendState.GetAddressOf());
}

void Graphics::SetCullModes()
{
	D3D11_RASTERIZER_DESC cd = {};

	//Pixel clip mode
	cd.FillMode = D3D11_FILL_SOLID;
	cd.CullMode = D3D11_CULL_NONE;

	device->CreateRasterizerState(&cd, NoCullMode.GetAddressOf());
}

void Graphics::UpdateBufferData(std::vector<std::unique_ptr<Entity>>& entities, DX::XMMATRIX cameraMatrix)
{
	UINT stride = sizeof(Mesh::Vertex);
	UINT offset = 0;

	for (auto& e : entities)
	{
		if (e->IsActive())
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
			
			//Set cullmode
			context->RSSetState(NoCullMode.Get());
			
			//Draw the object
			context->DrawIndexed((UINT)std::size(e->GetMesh().GetIndices()), 0, 0);
		}
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

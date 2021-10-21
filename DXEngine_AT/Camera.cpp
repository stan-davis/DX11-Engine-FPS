#include "Camera.h"

Camera::Camera(wrl::ComPtr<ID3D11Device> _device, wrl::ComPtr<ID3D11DeviceContext> _context, int _windowWidth, int _windowHeight) 
	: device(_device), context(_context), windowWidth(_windowWidth), windowHeight(_windowHeight)
{
	//Set Up Constant Buffer
	D3D11_BUFFER_DESC cbbd = {};

	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.ByteWidth = sizeof(cBuffer);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;

	device->CreateBuffer(&cbbd, nullptr, constantBuffer.GetAddressOf());

	//Set up camera TESTING ONLY
	camPosition = DirectX::XMVectorSet(0.0f, 3.0f, -8.0f, 0.0f);
	camTarget = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	camUp = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	camView = DirectX::XMMatrixLookAtLH(camPosition, camTarget, camUp);

	camProjection = DirectX::XMMatrixPerspectiveFovLH(0.4f * 3.14f, static_cast<float>(windowWidth / windowHeight), 1.0f, 1000.0f);
}

void Camera::Update()
{
	World = DirectX::XMMatrixIdentity();
	World = Translation * Rotation;
}

void Camera::Render()
{
	WVP = World * camView * camProjection;
	cBuff.WVP = DirectX::XMMatrixTranspose(WVP);
	context->UpdateSubresource(constantBuffer.Get(), 0, nullptr, &cBuff, 0, 0);
	context->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
}

void Camera::Translate(float x, float y, float z)
{
	Translation = DirectX::XMMatrixTranslation(x, y, z);
}

void Camera::Rotate(float x, float y, float z, float angle)
{
	dx::XMVECTOR axis = dx::XMVectorSet(x, y, z, 0.0f);
	Rotation = dx::XMMatrixRotationAxis(axis, angle);
}

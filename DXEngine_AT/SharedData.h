#pragma once

#include <wrl/client.h>
#include <d3d11.h>

#pragma comment(lib, "d3d11.lib")

namespace wrl = Microsoft::WRL;

struct SharedData
{
	static UINT windowWidth;
	static UINT windowHeight;

	static wrl::ComPtr<ID3D11Device> device;
	static wrl::ComPtr<ID3D11DeviceContext> context;
};
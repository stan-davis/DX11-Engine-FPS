#pragma once

#include <Windows.h>
#include <d3d11.h>

#pragma comment(lib, "d3d11.lib")

#include <DirectXMath.h>
#include <wrl/client.h>
#include <string>

namespace wrl = Microsoft::WRL;

class DiectXApp
{
public:
	DiectXApp(HINSTANCE _hInstance, std::string _windowTitle, UINT _windowWidth, UINT _windowHeight);
	~DiectXApp() = default;

	//Message processing
	static DiectXApp* DXInstance;
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	LRESULT ProcessMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	//Initlization required for Windows and DX11
	HRESULT InitWindow();
	HRESULT InitDirectX();
	HRESULT Run();

	void UpdateTime();

	//Virtual Methods for Gameplay
	virtual void Start() = 0;
	virtual void Update(float _deltaTime) = 0;
	virtual void Draw(float _deltaTime) = 0;

protected:
	//Window
	HINSTANCE hInstance;
	HWND hWnd;
	std::string windowTitle;
	UINT windowWidth;
	UINT windowHeight;

	//DirectX  //MOVE SOME OF THESE INTO A SHARED DATA SINGLETON - Primarily device and context
	wrl::ComPtr<IDXGISwapChain> swapchain;
	wrl::ComPtr<ID3D11Device> device;
	wrl::ComPtr<ID3D11DeviceContext> context;
	wrl::ComPtr<ID3D11RenderTargetView> renderTargetView;
	wrl::ComPtr<ID3D11DepthStencilView> depthStencilView;

private:
	//Useful timing functions
	double performanceSeconds;
	UINT64 startTime;
	UINT64 currentTime;
	UINT64 previousTime;
	float deltaTime;
	float totalElapsedTime;
};


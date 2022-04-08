#include "DirectXApp.h"
#include <stdio.h>
#include <io.h>
#include <fcntl.h>

DiectXApp* DiectXApp::DXInstance = 0;

DiectXApp::DiectXApp(HINSTANCE _hInstance, std::string _windowTitle, UINT _windowWidth, UINT _windowHeight)
{
	DXInstance = this;

	hInstance = _hInstance;
	windowTitle = _windowTitle;
	windowWidth = _windowWidth;
	windowHeight = _windowHeight;

	startTime = 0;
	currentTime = 0;
	deltaTime = 0;
	totalElapsedTime = 0;

	UINT64 perfFreq;
	QueryPerformanceFrequency((LARGE_INTEGER*)&perfFreq);
	performanceSeconds = 1.0 / static_cast<double>(perfFreq);
}

LRESULT DiectXApp::WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return DXInstance->ProcessMessage(hWnd, msg, wParam, lParam);
}

LRESULT DiectXApp::ProcessMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		case WM_KEYDOWN:
			input->keyDown(wParam);
			break;
		case WM_KEYUP:
			input->keyUp(wParam);
			break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

HRESULT DiectXApp::InitWindow()
{
	//Register Window Class
	WNDCLASSEX wc = {};

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = DiectXApp::WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = L"DXWindowClass";

	if (!RegisterClassEx(&wc))
	{
		DWORD lastError = GetLastError();

		if (lastError != ERROR_CLASS_ALREADY_EXISTS)
			return HRESULT_FROM_WIN32(lastError);
	}

	RECT wr = { 0, 0, static_cast<LONG>(windowWidth), static_cast<LONG>(windowHeight) };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	//Convert Window Title
	int wchar_count = MultiByteToWideChar(CP_UTF8, 0, windowTitle.c_str(), -1, 0, 0);
	wchar_t* w_title = new wchar_t[wchar_count];
	MultiByteToWideChar(CP_UTF8, 0, windowTitle.c_str(), -1, w_title, wchar_count);

	//Create & Set Window
	hWnd = CreateWindowEx(
		0,
		wc.lpszClassName,
        w_title,
		WS_OVERLAPPEDWINDOW,
		300,
		300,
		wr.right - wr.left,
		wr.bottom - wr.top,
		0,
		0,
		hInstance,
		0);

	delete[] w_title; //delete pointer after use

	if (hWnd == nullptr)
	{
		DWORD lastError = GetLastError();
		return HRESULT_FROM_WIN32(lastError);
	}

	ShowWindow(hWnd, SW_SHOW);

	return S_OK;
}

HRESULT DiectXApp::InitDirectX()
{
	//Swap Chain Configuration
	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(scd));

	scd.BufferCount = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.Width = windowWidth;
	scd.BufferDesc.Height = windowHeight;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = hWnd;
	scd.SampleDesc.Count = 1;
	scd.Windowed = TRUE;

	HRESULT result = S_OK;

	//Create device, devcon & swapchain
	result = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		0,
		0,
		D3D11_SDK_VERSION,
		&scd,
		&swapchain,
		&device,
		0,
		&context);

	if (FAILED(result)) return result;

	//Get Back Buffer Address
	wrl::ComPtr<ID3D11Texture2D> tex_backbuffer = 0;
	swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&tex_backbuffer);

	//Create & Set Render Target
	if (tex_backbuffer != 0)
	{
		device->CreateRenderTargetView(tex_backbuffer.Get(), nullptr, renderTargetView.GetAddressOf());
	}

	//Create Stencil/Depth Buffer
	D3D11_TEXTURE2D_DESC dsd;

	dsd.Width = windowWidth;
	dsd.Height = windowHeight;
	dsd.MipLevels = 1;
	dsd.ArraySize = 1;
	dsd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsd.SampleDesc.Count = 1;
	dsd.SampleDesc.Quality = 0;
	dsd.Usage = D3D11_USAGE_DEFAULT;
	dsd.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	dsd.CPUAccessFlags = 0;
	dsd.MiscFlags = 0;

	wrl::ComPtr<ID3D11Texture2D> tex_depthbuffer = 0;
	device->CreateTexture2D(&dsd, nullptr, tex_depthbuffer.GetAddressOf());

	if (tex_depthbuffer != 0)
	{
		device->CreateDepthStencilView(tex_depthbuffer.Get(), nullptr, depthStencilView.GetAddressOf());
	}

	//Set Render Targets
	context->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), depthStencilView.Get());

	//Create & Set Viewport
	D3D11_VIEWPORT viewport = {};

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = windowWidth;
	viewport.Height = windowHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	context->RSSetViewports(1, &viewport);

	return S_OK;
}

HRESULT DiectXApp::Run()
{
	//Start Time
	UINT64 time;
	QueryPerformanceCounter((LARGE_INTEGER*)&time);
	
	startTime = time;
	currentTime = time;
	previousTime = time;

	//First Frame Start
	Start();

	input = std::make_unique<Input>();

	//Message & Game Loop
	MSG msg = {};

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			UpdateTime();

			//Update & Draw Game
			Update(deltaTime);
			Draw(deltaTime);

			UpdateDebugInfo();
		}
	}

	return msg.wParam;
}

void DiectXApp::UpdateTime()
{
	UINT64 time;
	QueryPerformanceCounter((LARGE_INTEGER*)&time);
	currentTime = time;

	deltaTime = max(static_cast<float>((currentTime - previousTime) * performanceSeconds), 0.0f);
	totalElapsedTime = static_cast<float>((currentTime - startTime) * performanceSeconds);

	previousTime = currentTime;
}

void DiectXApp::UpdateDebugInfo()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&endFrameTime);
	
	timeToUpdate = static_cast<float>((endFrameTime - currentTime) * performanceSeconds) * 1000;
	
	std::string titleText = "Last Frame Update: " + std::to_string(static_cast<int>(round(timeToUpdate))) + "ms";
	SetWindowTextA(hWnd, titleText.c_str());
}

#include "CoreDX11.h"

CoreDX11::CoreDX11()
{
	pgrp = std::make_shared<PGRP>();
	game = std::make_unique<Game>();
}

bool CoreDX11::InitializeDirect3d11App(HINSTANCE hInstance, int _width, int _height, HWND hwnd)
{
	HRESULT hr;

	//Describes what our buffer is
	DXGI_MODE_DESC b_desc;

	ZeroMemory(&b_desc, sizeof(DXGI_MODE_DESC));

	width = _width;
	height = _height;

	b_desc.Width = _width;
	b_desc.Height = _height;
	b_desc.RefreshRate.Numerator = 60; //60 fps?
	b_desc.RefreshRate.Denominator = 1;
	b_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //RGBA 8bit, 0-255 in Red, Green, Blue and Alpha
	b_desc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	b_desc.Scaling = DXGI_MODE_SCALING_STRETCHED; //stretches image on screen to fit monitor ratio

	//Describes Swap chain
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapChainDesc.BufferDesc = b_desc;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	//makes the swapchain
	hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL, D3D11_SDK_VERSION, &swapChainDesc, &SwapChain, &d3d11Device, NULL, &d3d11DevCon);

	ID3D11Texture2D* BackBuffer;
	hr = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer);

	hr = d3d11Device->CreateRenderTargetView(BackBuffer, NULL, &renderTargetView);
	BackBuffer->Release();

	d3d11DevCon->OMSetRenderTargets(1, &renderTargetView, NULL);
	return true;
}

void CoreDX11::Draw()
{
	//clear backbuffer to updated color
	float bgColor[4] = { (0.0f, 0.0f, 0.0f, 0.0f) };
	d3d11DevCon->ClearRenderTargetView(renderTargetView, bgColor);
	//d3d11DevCon->ClearRenderTargetView(renderTargetView, RGBA{ red,green,blue,alpha });
	pgrp->Draw(d3d11DevCon);
	game->Render(d3d11DevCon);
	
	//Presents the backbuffer to the screen
	SwapChain->Present(0, 0);
}

void CoreDX11::Update(double deltaTime)
{
	bgColorRGB();
	pgrp->Update(deltaTime);
	game->Update(deltaTime);
}

void CoreDX11::releaseObject()
{
	//Release all pointer objects
	pgrp->Free();
	SwapChain->Release();
	d3d11Device->Release();
	d3d11DevCon->Release();
}

void CoreDX11::bgColorRGB()
{
	red += colormodr * 0.00005f;
	green += colormodg * 0.00002f;
	blue += colormodb * 0.00001f;

	if (red >= 1.0f ||red <= 0.0f)
	{
		colormodr *= -1;
	}
	if (green >= 1.0f || green <= 0.0f)
	{
		colormodg *= -1;
	}
	if (blue >= 1.0f ||blue <= 0.0f)
	{
		colormodb *= -1;
	}
}

void CoreDX11::init()
{
	pgrp->init(d3d11Device, d3d11DevCon, width, height, renderTargetView);
	if (!game->Init(pgrp,d3d11DevCon))
	{
		MessageBox(0, "Scene Initialization - Failed", "Error", MB_OK);
	}
}


std::shared_ptr<Camera> CoreDX11::Cam()
{
	return this->pgrp->Cam();
}


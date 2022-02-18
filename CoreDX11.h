#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <PGRP.h>
#include <game.h>
#include <DoomTime.h>
#include <chrono>

using RGBA = float[4];
class CoreDX11
{
public:
	CoreDX11();
	~CoreDX11() = default;

	bool InitializeDirect3d11App(HINSTANCE hInstance, int _width, int _height, HWND hwnd);
	void Draw();
	void Update(double deltaTime);
	void releaseObject();
	void bgColorRGB();
	void init();

	std::shared_ptr<Camera> Cam();

	float red = 0.0f;
	float green = 0.0f;
	float blue = 0.0f;
	float alpha = 1.0f;
	int colormodr = 1;
	int colormodg = 1;
	int colormodb = 1;
	int width = 0;
	int height = 0;
private:
	IDXGISwapChain* SwapChain;
	ID3D11Device* d3d11Device;
	ID3D11DeviceContext* d3d11DevCon;
	ID3D11RenderTargetView* renderTargetView;
	std::shared_ptr<PGRP> pgrp = nullptr;
	std::unique_ptr<Game> game;
};


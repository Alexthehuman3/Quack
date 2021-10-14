#pragma once
#include <Windows.h>
#include <d3d11.h>
class CoreDX11
{
public:
	CoreDX11() = default;
	~CoreDX11() = default;

	bool InitializeDirect3d11App(HINSTANCE hInstance, int _width, int _height);
	void ReleaseObjects();
	bool InitScene();
	void UpdateScene();
	void Draw();

	void Red(float _red);
	void Green(float _green);
	void Blue(float _blue);
	float Red();
	float Green();
	float Blue();
	void ColorModR(int _cmred);
	void ColorModG(int _cmgreen);
	void ColorModB(int _cmblue);
	int ColorModR();
	int ColorModG();
	int ColorModB();

private:
	IDXGISwapChain* SwapChain;
	ID3D11Device* d3d11Device;
	ID3D11DeviceContext* d3d11DevCon;
	ID3D11RenderTargetView* renderTargetView;

	float red = 0.0f;
	float green = 0.0f;
	float blue = 0.0f;
	int colormodr = 1;
	int colormodg = 1;
	int colormodb = 1;
};


#include "CoreDX11.h"



bool CoreDX11::InitializeDirect3d11App(HINSTANCE hInstance, int _width, int _height)
{
	HRESULT hr;

	//Describes what our buffer is
	DXGI_MODE_DESC b_desc;

	ZeroMemory(&b_desc, sizeof(DXGI_MODE_DESC));

	b_desc.Width = _width;
	b_desc.Height = _height; 
	b_desc.RefreshRate.Numerator = 60; //60 hz
	b_desc.RefreshRate.Denominator = 1;
	b_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //RGBA 8bit, 0-255 in Red, Green, Blue and Alpha
	b_desc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	b_desc.Scaling = DXGI_MODE_SCALING_STRETCHED; //stretches image on screen to fit monitor ratio

	//Describes Swap chain


	if (!hr)
	{
		return false;
	}
	return true;
}

void CoreDX11::ReleaseObjects()
{
}

bool CoreDX11::InitScene()
{
	return false;
}

void CoreDX11::UpdateScene()
{
}

void CoreDX11::Draw()
{
}

void CoreDX11::Red(float _red)
{
	red = _red;
}

void CoreDX11::Green(float _green)
{
	green = _green;
}

void CoreDX11::Blue(float _blue)
{
	blue = _blue;
}

float CoreDX11::Red()
{
	return red;
}

float CoreDX11::Green()
{
	return green;
}

float CoreDX11::Blue()
{
	return blue;
}

void CoreDX11::ColorModR(int _cmred)
{
	colormodr = _cmred;
}

void CoreDX11::ColorModG(int _cmgreen)
{
	colormodg = _cmgreen;
}

void CoreDX11::ColorModB(int _cmblue)
{
	colormodb = _cmblue;
}

int CoreDX11::ColorModR()
{
	return colormodr;
}

int CoreDX11::ColorModG()
{
	return colormodg;
}

int CoreDX11::ColorModB()
{
	return colormodb;
}

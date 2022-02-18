#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <comdef.h>
#include <memory>
#include <string>
#include <Vector3.h>
#include <Vector2.h>
#include <Camera.h>
#include <vector>
#include <GameObject.h>
#include <WICTextureLoader11.h>
#include <DoomTime.h>

using RGBA = float[4] ;
struct cbPerObject
{
	XMMATRIX WVP;
	XMMATRIX World;
};
struct Light
{
	Light()
	{
		ZeroMemory(this, sizeof(Light));
	}
	Vector3 dir;
	float pad;
	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
};
struct cbPerFrame
{
	Light light;
};
class PGRP
{
	//Programmable Graphics Rendering Pipeline
public:
	PGRP();
	~PGRP() = default;
	void Free();
	void Update(double deltaTime);
	void Draw(ID3D11DeviceContext* devCon);
//	void ReDrawAllObjects(ID3D11DeviceContext* devCon);
	void init(ID3D11Device* d3d11Device, ID3D11DeviceContext* d3d11DevCon, int width, int height, ID3D11RenderTargetView* _renderTargetView);
	void initShaders(ID3D11Device* device, ID3D11DeviceContext* devCon);
	void initTextures(ID3D11Device* device, ID3D11DeviceContext* devCon);
	void initIndices(ID3D11Device* device, ID3D11DeviceContext* devCon);
	void initCubeIndices(ID3D11Device* device, ID3D11DeviceContext* devCon);
	void initDepthStencilBuffer(ID3D11Device* device, ID3D11DeviceContext* devCon, int width, int height, ID3D11RenderTargetView* _renderTargetView);
	void initColouredVertex(ID3D11Device* device, ID3D11DeviceContext* devCon);
	void initTexturedVertex(ID3D11Device* device, ID3D11DeviceContext* devCon);
	void createViewport(ID3D11Device* device, ID3D11DeviceContext* devCon, int width, int height);
	void createConstBuffer(ID3D11Device* device);
	void makeCube(ID3D11DeviceContext* devCon, std::shared_ptr<GameObject> obj);
	void renderCube(ID3D11DeviceContext* devCon, std::shared_ptr<GameObject> obj);
	void setCubeRotation(std::string _name, std::shared_ptr<GameObject> obj);

	std::shared_ptr<Camera> Cam();
	cbPerObject cbPerObj;
	cbPerFrame constbuffPerFrame;
private:
	ID3D11Buffer* squareVertBuffer;			  // Buffer for the Vertices that generates a square
	ID3D11Buffer* squareIndexBuffer;		  // Buffer for Square Indices
	ID3D11Buffer* cbPerFrameBuffer;           // holds information to be used in Pixel Shader
	ID3D11Buffer* cbPerObjectBuffer;
	ID3D11VertexShader* VS;					  // Vertex Shader
	ID3D11PixelShader* PS;					  // Pixel Shader
	ID3D10Blob* VS_Buffer;					  // Buffer for Vertex Shader
	ID3D10Blob* PS_Buffer;					  // Buffer for Pixel Shader
	ID3D11InputLayout* vertLayout;			  // Vertex Layout, contains some form of Input I can't remember
	ID3D11DepthStencilView* depthStencilView; // Depth/Stencil View
	ID3D11Texture2D* depthStencilBuffer;      // Depth/Stencil Buffer
	ID3D11ShaderResourceView* CubesTexture;   // Cube Texture - Potentially an array
	ID3D11SamplerState* CubesTexSamplerState; // Holds Sampler State Information
	ID3D11ShaderResourceView* enemyTexture;
	
	Light light;
	
	
	std::shared_ptr<Camera> cam;              // Unique Camera that handles the viewing of world space
	std::vector<std::pair<std::string, std::shared_ptr<XMMATRIX>>> cubesWorld;

	XMMATRIX WVP;

	float cycleSpeed = 0.1f;
	
	float rotY = 0.0001f;
	float vertR = 1.0f;
	float vertG = 1.0f;
	float vertB = 1.0f;
	float light_x = 0;
	float light_y = 0;
	float light_z = 0;
	float terminal_angle = 360;
	bool can_spin = false;
};


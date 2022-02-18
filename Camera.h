#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <memory>
#include <Transform.h>
#include <Vector3.h>
using namespace DirectX;

class Camera
{
	/*
	* Note: Make Camera a component of GameObject, and link them together somehow.
	*/
public:
	Camera();
	~Camera() = default;
	void init(ID3D11Device* device, int width, int height);
	void Free();
	void Update(double deltaTime);
	void Draw(ID3D11DeviceContext* devCon);
	void initCamera(int _width, int _height);
	void cubeCollision(float);

	std::shared_ptr<XMMATRIX> CamView();
	std::shared_ptr<XMMATRIX> CamProjection();
	std::shared_ptr<Transform>& getTF();

	float horizontal = 0;
	float vertical = 0;
private:
	
	std::shared_ptr<XMMATRIX> camView;
	std::shared_ptr<XMMATRIX> camProjection;
	std::shared_ptr<Transform> transform;

	XMVECTOR camPosition;
	XMVECTOR camTarget;
	XMVECTOR camUp;
	XMVECTOR camForward;
	XMVECTOR camRight;
	XMVECTOR camDown;
	XMMATRIX camRotation;

	const float DEFAULT_X = 0.0f;
	const float DEFAULT_Y = 2.0f;
	const float DEFAULT_Z = 0.0f;

	const float GRAVITY = 1.0f;
	const float TERMINAL_VELOCITY = 2.0f;

	XMVECTOR default_forward = XMVectorSet(0.0f,0.0f,1.0f,0.0f);
	XMVECTOR default_right = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	float width;
	float height;
	float moveBackForward = 0.0f;
	float rotateLeftRight = 0.0f;
	float yaw = 0;

	const float FOV = 0.4f * 3.14f;
	const float NEAR_Z = 0.1f;
	const float FAR_Z = 1000.0f;

	bool moveable = true;
};


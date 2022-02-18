#pragma once
#include <DirectXMath.h>
using namespace DirectX;
struct Vector3
{
	Vector3(float _x = 0, float _y = 0, float _z = 0);
	Vector3(const Vector3&) = default;
	~Vector3() = default;

	Vector3& operator=(Vector3& _vt3);
	Vector3& operator=(Vector3&& _vt3) noexcept;
	Vector3& operator+(Vector3& _vt3);
	XMVECTOR XMVectorConverter(); //returns an XMVECTOR
	XMMATRIX XMMatrixConverter();

	float x;
	float y;
	float z;
};


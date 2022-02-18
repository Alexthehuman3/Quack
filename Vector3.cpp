#include "Vector3.h"

Vector3::Vector3(float _x, float _y, float _z)
{
	x = _x;
	y = _y;
	z = _z;
}

Vector3& Vector3::operator=(Vector3& _vt3)
{
	x = _vt3.x;
	y = _vt3.y;
	z = _vt3.z;
	return *this;
}

Vector3& Vector3::operator=(Vector3&& _vt3) noexcept
{
	x = _vt3.x;
	y = _vt3.y;
	z = _vt3.z;
	return *this;
}

Vector3& Vector3::operator+(Vector3& _vt3)
{
	x += _vt3.x;
	y += _vt3.y;
	z += _vt3.z;
	return *this;
}

XMVECTOR Vector3::XMVectorConverter()
{
	return { x,y,z };
}

XMMATRIX Vector3::XMMatrixConverter()
{
	XMMATRIX temp_Matrix = XMMatrixRotationRollPitchYaw(x, y, z);
	return temp_Matrix;
}

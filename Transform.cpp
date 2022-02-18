#include "Transform.h"

Transform::Transform(Vector3 _pos)
{
	position = _pos;
}

Transform::Transform(Vector3 _pos, Vector3 _scale)
{
	position = _pos;
	scale = _scale;
}

Transform::Transform(Vector3 _pos, Vector3 _rot, Vector3 _scale)
{
	position = _pos;
	rotation = _rot;
	scale = _scale;
}

Transform& Transform::operator=(Transform&)
{
	return *this;
}

void Transform::Position(float x, float y, float z)
{
	position = { x,y,z };
}

Vector3 Transform::getPos()
{
	return position;
}

void Transform::Rotation(float x, float y, float z)
{
	rotation = { x,y,z };
}

Vector3 Transform::getRot()
{
	return rotation;
}

void Transform::xRot(float x)
{
	rotation = { x,rotation.y,rotation.z };
}

void Transform::yRot(float y)
{
	rotation = { rotation.x,y,rotation.z };
}

void Transform::zRot(float z)
{
	rotation = { rotation.x,rotation.y,z };
}

void Transform::Scale(float x, float y, float z)
{
	scale = { x, y, z };
}

Vector3 Transform::getScale()
{
	return scale;
}

void Transform::LookAt(float _x, float _y, float _z)
{
	XMMATRIX target = XMMatrixLookAtLH(position.XMVectorConverter(), XMVectorSet(_x, _y, _z, 0.0f), Up.XMVectorConverter());
	XMFLOAT4X4 matrixIn4x4;
	XMStoreFloat4x4(&matrixIn4x4, target);
}

void Transform::LookAt(Transform& transform)
{

}

float Transform::getXPos()
{
	return position.x;
}

float Transform::getYPos()
{
	return position.y;
}

float Transform::getZPos()
{
	return position.z;
}

float Transform::getXRot()
{
	return rotation.x;
}

float Transform::getYRot()
{
	return rotation.y;
}

float Transform::getZRot()
{
	return rotation.z;
}

void Transform::xPos(float _x)
{
	position.x = _x;
}

void Transform::yPos(float _y)
{
	position.y = _y;
}

void Transform::zPos(float _z)
{
	position.z = _z;
}

std::string Transform::Name()
{
	return name;
}

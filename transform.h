#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <Vector3.h>
#include <string>
#include <DirectXMath.h>
using namespace DirectX;
class Transform 
{
public:
	Transform() = default;
	Transform(Transform&) = default;
	Transform(Vector3 _pos);
	Transform(Vector3 _pos, Vector3 _scale);
	Transform(Vector3 _pos, Vector3 _rot, Vector3 _scale);
	~Transform() = default;

	Transform& operator=(Transform&);

	void Position(float x = 0, float y = 0, float z = 0);
	Vector3 getPos();
	void Rotation(float x = 0, float y = 0, float z = 0);
	Vector3 getRot();
	void Scale(float x = 1, float y = 1, float z = 1);
	Vector3 getScale();
	void LookAt(float _x, float _y, float _z);
	void LookAt(Transform& transform);

	float getXPos();
	float getYPos();
	float getZPos();
	float getXRot();
	float getYRot();
	float getZRot();
	void xPos(float _x);
	void yPos(float _y);
	void zPos(float _z);
	void xRot(float x);
	void yRot(float y);
	void zRot(float z);

	std::string Name();
	Vector3 Up;
	XMVECTOR XM_up = XMVectorSet(0, position.y+1, 0, 0);
	Vector3 Left;
	Vector3 Right;
	Vector3 Forward = { 0,0,1 };

private:
	Vector3 position = { 0,0,0 };
	Vector3 rotation = { 0,0,0 };
	Vector3 scale = {1,1,1};
	std::string name = "Transform";
};

#endif

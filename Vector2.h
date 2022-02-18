#pragma once
struct Vector2
{
	Vector2(float _x = 0, float _y = 0);
	Vector2 (const Vector2&) = default;
	~Vector2() = default;

	Vector2& operator=(Vector2& _vt2);
	Vector2& operator=(Vector2&& _vt2);

	float x;
	float y;
};


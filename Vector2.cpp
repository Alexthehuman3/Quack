#include "Vector2.h"

Vector2::Vector2(float _x, float _y)
{
	x = _x;
	y = _y;
}

Vector2& Vector2::operator=(Vector2& _vt2)
{
	x = _vt2.x;
	y = _vt2.y;
	return *this;
}

Vector2& Vector2::operator=(Vector2&& _vt2)
{
	x = _vt2.x;
	y = _vt2.y;
	return *this;
}

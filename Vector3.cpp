// Vector3.cpp
// Created by xiaojl Jan/18/2021
// 三维向量

#include "Vector3.h"

Vector3& Vector3::operator =(const Vector3 &rhs)
{
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;
	return *this;
}

bool Vector3::operator ==(const Vector3 &rhs) const
{
	return x == rhs.x && y == rhs.y && z == rhs.z;
}

bool Vector3::operator !=(const Vector3 &rhs) const
{
	return x != rhs.x || y != rhs.y || z != rhs.z;
}

Vector3 Vector3::operator -() const
{
	return Vector3(-x, -y, -z);
}

Vector3 Vector3::operator +(const Vector3 &rhs) const
{
	return Vector3(x + rhs.x, y + rhs.y, z + rhs.z);
}

Vector3 Vector3::operator -(const Vector3 &rhs) const
{
	return Vector3(x - rhs.x, y - rhs.y, z - rhs.z);
}

Vector3 Vector3::operator *(const float &rhs) const
{
	return Vector3(x * rhs, y * rhs, z * rhs);
}

Vector3 Vector3::operator /(const float &rhs) const
{
	return Vector3(x / rhs, y / rhs, z / rhs);
}

Vector3& Vector3::operator +=(const Vector3 &rhs)
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	return *this;
}

Vector3& Vector3::operator -=(const Vector3 &rhs)
{
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	return *this;
}

Vector3& Vector3::operator *=(const float &rhs)
{
	x *= rhs;
	y *= rhs;
	z *= rhs;
	return *this;
}

Vector3& Vector3::operator /=(const float &rhs)
{
	x /= rhs;
	y /= rhs;
	z /= rhs;
	return *this;
}

float Vector3::Dot(const Vector3 &rhs) const
{
	return (x * rhs.x + y * rhs.y + z * rhs.z);
}

Vector3& Vector3::Cross(const Vector3 &rhs) const
{
	// TODO
	Vector3 v;
	return v;
}

Vector3& Vector3::Normalize()
{
	float magSq = x * x + y * y + z * z;
	if (magSq > 0.0f)
	{
		float oneOverMag = 1.0f / std::sqrt(magSq);
		x *= oneOverMag;
		y *= oneOverMag;
		z *= oneOverMag;
	}

	return *this;
}

void Vector3::Zero()
{
	x = 0;
	y = 0;
	z = 0;
}

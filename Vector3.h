// Vector3.h
// Created by xiaojl Jan/18/2021
// 三维向量

#pragma once

# include <cmath>

class Vector3
{
public:
	Vector3() : x(0), y(0), z(0) { }
	Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) { }

public:
	Vector3& operator =(const Vector3 &rhs);
	bool operator ==(const Vector3 &rhs) const;
	bool operator !=(const Vector3 &rhs) const;
	Vector3 operator -() const;
	Vector3 operator +(const Vector3 &rhs) const;
	Vector3 operator -(const Vector3 &rhs) const;
	Vector3 operator *(const float &rhs) const;
	Vector3 operator /(const float &rhs) const;
	Vector3& operator +=(const Vector3 &rhs);
	Vector3& operator -=(const Vector3 &rhs);
	Vector3& operator *=(const float &rhs);
	Vector3& operator /=(const float &rhs);

public:
	float Dot(const Vector3 &rhs) const;
	Vector3& Cross(const Vector3 &rhs) const;
	Vector3& Normalize();
	void Zero();
	
public:
	static const Vector3 zero;

public:
	float x;
	float y;
	float z;
};

const Vector3 zero = Vector3(0, 0, 0);

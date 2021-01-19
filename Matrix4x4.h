// Matrix4x4.h
// Created by xiaojl Jan/19/2021
// 四阶矩阵

#pragma once

#include <cmath>

#include "Vector3.h"

class Matrix4x4
{
public:
	Matrix4x4& operator *(const Matrix4x4 &rhs);

public:
	static Matrix4x4 LookAtLH(Vector3 eye, Vector3 forward, Vector3 up);
	static Matrix4x4 PerspectiveFovLH(float fov, float aspect, float znear, float zfar);
	static Matrix4x4 Rotation(Vector3 r);
	static Matrix4x4 RotationX(float angle);
	static Matrix4x4 RotationY(float angle);
	static Matrix4x4 RotationZ(float angle);
	static Matrix4x4 Translation(Vector3 &rhs);

public :
	float Values[4][4];
};

static Matrix4x4 Identity =
{
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
};

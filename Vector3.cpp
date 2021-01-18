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

void Vector3::Zero()
{
	x = 0;
	y = 0;
	z = 0;
}

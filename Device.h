// Device.h
// Created by xiaojl Jan/18/2021
// 渲染设备

#pragma once

#include <cstdio>
#include <vector>

#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"

class Device
{
public:
	Device(int _width, int _height);

public:
	void Clear();
	void GeometryStage(Vector4 &vertex1, Vector4 &vertex2, Vector4 &vertex3);
	bool Clipping(const Vector4 &vertex);
	void ScreenMapping(Vector4 &vertex);
	void RasterizerStage(Vector4 &vertex1, Vector4 &vertex2, Vector4 &vertex3);
	std::vector<Vector3> TriangleSetup(const Vector4 &vertex1, const Vector4 &vertex2, const Vector4 &vertex3);
	std::vector<Vector3> TriangleTraversal(const Vector3 &vertex1, const Vector3 &vertex2, const Vector3 &vertex3);
	void PerFragmentOperations(const std::vector<Vector3> &fragments);

public:
	int width;
	int height;
	std::vector<Vector4> frameBuffer; // 帧缓冲
	std::vector<float> depthBuffer; // 深度缓冲
};

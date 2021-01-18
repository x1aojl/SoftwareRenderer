// Device.h
// Created by xiaojl Jan/18/2021
// 渲染设备

#pragma once

#include <cstdio>
#include <vector>

#include "Vector3.h"
#include "Vector4.h"

class Device
{
public:
	Device(int _width, int _height);

public:
	void Clear();
	void Rasterization(std::vector<Vector3> &mesh);
	void TriangleTraversal(Vector3 &v1, Vector3 &v2, Vector3 &v3, std::vector<Vector3> &fragments);
	void PerFragmentOperations(const std::vector<Vector3> &fragments);

public:
	int width;
	int height;
	std::vector<Vector4> frameBuffer; // 帧缓冲
	std::vector<float> depthBuffer; // 深度缓冲
};

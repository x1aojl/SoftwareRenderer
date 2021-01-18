// Device.cpp
// Created by xiaojl Jan/18/2021
// 渲染设备

#include "Device.h"

Device::Device(int _width, int _height) : width(_width), height(_height)
{
	frameBuffer = std::vector<Vector4>(width * height, Vector4());
	depthBuffer = std::vector<float>(width * height, FLT_MAX);
}

void Device::Clear()
{
	for (int i = 0; i < frameBuffer.size(); i++)
		frameBuffer[i].Zero();

	for (int i = 0; i < depthBuffer.size(); i++)
		depthBuffer[i] = FLT_MAX;
}

// 光栅化阶段
void Device::Rasterization(std::vector<Vector3> &mesh)
{
	for (int i = 0; i < mesh.size(); i += 3)
	{
		std::vector<Vector3> fragments;
		TriangleTraversal(mesh[0], mesh[1], mesh[2], fragments);
		PerFragmentOperations(fragments);
	}
}

// 逐片元操作（模板测试，深度测试，颜色混合）
void Device::PerFragmentOperations(const std::vector<Vector3> &fragments)
{
	for (auto fragment : fragments)
	{
		int index = fragment.x + fragment.y * width;
		if (fragment.x < 0 || fragment.x >= width ||
			fragment.y < 0 || fragment.y >= height)
			continue;

		// 深度测试
		if (depthBuffer[index] < fragment.z)
			continue;

		// 更新深度缓冲
		depthBuffer[index] >= fragment.z;

		// 更新颜色缓冲
		frameBuffer[index] = Vector4(0, 1, 1, 1);
	}
}

// 三角形遍历
void Device::TriangleTraversal(Vector3 &v1, Vector3 &v2, Vector3 &v3, std::vector<Vector3> &fragments)
{
	// 扫描线
	//    v3
	// 	  -
	// 	  - -
	// vm -   - v2
	// 	  - -
	//    -
	//    v1
	// 将v1、v2、v3三个点按纵坐标y升序排列
	// 分别扫描v1-v2-vm和v2-vm-v3两个三角形

	// 排序 v1.y <= v2.y <= v3.y
	if (v1.y > v2.y) std::swap(v1, v2);
	if (v2.y > v3.y) std::swap(v2, v3);
	if (v1.y > v3.y) std::swap(v1, v3);

	// 屏幕坐标为整数，将v1、v2、v3的坐标化整
	int x1 = (int)v1.x, y1 = (int)v1.y, z1 = (int)v1.z;
	int x2 = (int)v2.x, y2 = (int)v2.y, z2 = (int)v2.z;
	int x3 = (int)v3.x, y3 = (int)v3.y, z3 = (int)v3.z;

	// v1、v2、v3两两纵坐标差异值，加1防止除零
	int diffY_12 = y2 - y1 + 1;
	int diffY_23 = y3 - y2 + 1;
	int diffY_13 = y3 - y1 + 1;

#pragma region v1-v2-vm
	for (int y = y1; y <= y2; y++)
	{
		// 左右边界步长比例
		float lhsScale = (float)(y - y1) / diffY_13;
		float rhsScale = (float)(y - y1) / diffY_12;

		// 左右边界Z值
		float lhsZ = z1 + (z3 - z1) * lhsScale;
		float rhsZ = z1 + (z2 - z1) * rhsScale;
		int diffZ_lr = rhsZ - lhsZ;

		// 左右边界X值
		int lhsX = (int)(x1 + (x3 - x1) * lhsScale);
		int rhsX = (int)(x1 + (x2 - x1) * rhsScale);
		if (lhsX > rhsX) std::swap(lhsX, rhsX);
		int diffX_lr = rhsX - lhsX;

		// 从左到右扫描
		for (int x = lhsX; x <= rhsX; x++)
		{
			float scale = (float)(x - lhsX) / diffX_lr;
			float z = lhsZ + diffZ_lr * scale;
			fragments.push_back(Vector3(x, y, z));
		}
	}
#pragma endregion

#pragma region v2-vm-v3
	for (int y = y2; y <= y3; y++)
	{
		// 左右边界步长比例
		float lhsScale = (float)(y - y1) / diffY_13;
		float rhsScale = (float)(y - y2) / diffY_23;

		// 左右边界Z值
		float lhsZ = z1 + (z3 - z1) * lhsScale;
		float rhsZ = z2 + (z3 - z2) * rhsScale;
		float diffZ_lr = rhsZ - lhsZ;

		// 左右边界X值
		int lhsX = (int)(x1 + (x3 - x1) * lhsScale);
		int rhsX = (int)(x2 + (x3 - x2) * rhsScale);
		if (lhsX > rhsX) std::swap(lhsX, rhsX);
		float diffX_lr = rhsX - lhsX;

		// 从左到右扫描
		for (int x = lhsX; x <= rhsX; x++)
		{
			float scale = (x - lhsX) / diffX_lr;
			float z = lhsZ + diffZ_lr * scale;
			fragments.push_back(Vector3(x, y, z));
		}
	}
#pragma endregion
}

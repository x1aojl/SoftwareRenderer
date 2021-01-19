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

// 几何阶段
void Device::GeometryStage(Vector4 &vertex1, Vector4 &vertex2, Vector4 &vertex3)
{
	std::vector<Vector4> vertices = std::vector<Vector4> { vertex1, vertex2, vertex3 };
	for (auto &vertex : vertices)
	{
		// 模型视图变换
		Vector3 meshRotation;
		Matrix4x4 rotation = Matrix4x4::Translation(meshRotation);

		Vector3 meshPosition;
		Matrix4x4 translation = Matrix4x4::Translation(meshPosition);

		Matrix4x4 world = rotation * translation;

		Vector3 CameraPosition = Vector3(0, 0, 10);
		Vector3 CameraForward = Vector3(0, 0, -10);
		Vector3 CameraUp = Vector3(0, 1, 0);
		Matrix4x4 view = Matrix4x4::LookAtLH(CameraPosition, CameraForward, CameraUp);
		Matrix4x4 projection = Matrix4x4::PerspectiveFovLH(0.78f, (float)width / height, 0.01f, 1.0f);

		Matrix4x4 mvp = world * view * projection;

		if (Clipping(vertex))
			return;

		ScreenMapping(vertex);
	}
}

// CVV裁剪
bool Device::Clipping(const Vector4 &vertex)
{
	float w = vertex.w;
	if (vertex.z < 0. || vertex.z > w) return true;
	if (vertex.x < -w || vertex.x > w) return true;
	if (vertex.y < -w || vertex.y > w) return true;
	return false;
}

// 屏幕映射
void Device::ScreenMapping(Vector4 &vertex)
{
	float rhw = 1.0f / vertex.w;
	vertex.x = 0.5f * width * (vertex.x * rhw + 1.0f);
	vertex.y = 0.5f * height * (1.0f - vertex.y * rhw);
	vertex.z = vertex.z * rhw;
	vertex.w = 1.0f;
}

// 光栅化阶段
void Device::RasterizerStage(Vector4 &vertex1, Vector4 &vertex2, Vector4 &vertex3)
{
	std::vector<Vector3> vertices = TriangleSetup(vertex1, vertex2, vertex3);
	std::vector<Vector3> fragments = TriangleTraversal(vertices[0], vertices[1], vertices[2]);
	PerFragmentOperations(fragments);
}

// 三角形设置
std::vector<Vector3> Device::TriangleSetup(const Vector4 &vertex1, const Vector4 &vertex2, const Vector4 &vertex3)
{
	return std::vector<Vector3>
	{
		Vector3(vertex1.x, vertex1.y, vertex1.z),
		Vector3(vertex2.x, vertex2.y, vertex2.z),
		Vector3(vertex3.x, vertex3.y, vertex3.z)
	};
}

// 三角形遍历
//    v3
// 	  -
// 	  - -
// vm -   - v2
// 	  - -
//    -
//    v1
// 将v1、v2、v3三个点按纵坐标y升序排列
// 分别扫描v1-v2-vm和v2-vm-v3两个三角形
std::vector<Vector3> Device::TriangleTraversal(const Vector3 &vertex1, const Vector3 &vertex2, const Vector3 &vertex3)
{
	std::vector<Vector3> fragments;

	// 排序 v1.y <= v2.y <= v3.y
	if (vertex1.y > vertex2.y) std::swap(vertex1, vertex2);
	if (vertex2.y > vertex3.y) std::swap(vertex2, vertex3);
	if (vertex1.y > vertex3.y) std::swap(vertex1, vertex3);

	// 屏幕坐标为整数，将v1、v2、v3的坐标化整
	int x1 = (int)vertex1.x, y1 = (int)vertex1.y, z1 = (int)vertex1.z;
	int x2 = (int)vertex2.x, y2 = (int)vertex2.y, z2 = (int)vertex2.z;
	int x3 = (int)vertex3.x, y3 = (int)vertex3.y, z3 = (int)vertex3.z;

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

	return fragments;
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

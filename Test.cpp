// Test.cpp
// Created by xiaojl Jan/18/2021
// 测试：将渲染结果输出到位图

#include "Device.h"
#include "Bitmap.h"

#define WIDTH 100
#define HEIGHT 100

// 网格数据
std::vector<Vector4> mesh
{
	Vector4(0,   0,	  1, 1),
	Vector4(100, 0,	  1, 1),
	Vector4(50,  100, 1, 1),
};

int main()
{
	Device dev(WIDTH, HEIGHT);
	dev.Clear();
	dev.GeometryStage(mesh[0], mesh[1], mesh[2]);
	dev.RasterizerStage(mesh[0], mesh[1], mesh[2]);
	Bitmap().SavaBmp(dev.frameBuffer, WIDTH, HEIGHT, "Screenshot1.bmp");

	return 0;
}

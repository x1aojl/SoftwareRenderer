// Test.cpp
// Created by xiaojl Jan/18/2021
// 测试：将渲染结果输出到位图

#include "Device.h"
#include "Bitmap.h"

#define WIDTH 100
#define HEIGHT 100

// 网格数据
std::vector<Vector3> mesh
{
	Vector3(0, 0,  1),
	Vector3(WIDTH, 0,  1),
	Vector3(WIDTH / 2,  HEIGHT,  1),
};

int main()
{
	Device dev(WIDTH, HEIGHT);
	dev.Clear();
	dev.Rasterization(mesh);
	Bitmap().SavaBmp(dev.frameBuffer, WIDTH, HEIGHT, "Screenshot.bmp");

	return 0;
}

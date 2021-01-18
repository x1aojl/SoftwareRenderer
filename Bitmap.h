// Bitmap.h
// Created by xiaojl Jan/18/2021
// 位图

#pragma once

#include <string>
#include <vector>

#include "Vector4.h"

class Bitmap
{
public:
	Bitmap() { }

public:
	void SavaBmp(std::vector<Vector4> &frameBuffer, int width, int height, std::string file);
};

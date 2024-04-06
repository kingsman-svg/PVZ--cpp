#pragma once
#include <graphics.h>
#include<cstring>
#include<iostream>
#include <fstream>
void putimagePNG(int  picture_x, int picture_y, IMAGE* picture);
int getDelay();
//判断文件是否存在
bool fileExists(const std::string& filename);
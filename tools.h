#pragma once
#include <graphics.h>
#include<cstring>
#include<iostream>
#include <fstream>
void putimagePNG(int  picture_x, int picture_y, IMAGE* picture);
int getDelay();
//�ж��ļ��Ƿ����
bool fileExists(const std::string& filename);
#pragma once
#include<iostream>
#include <fstream>
using namespace std;
#include<stdio.h>
#include"math.h"
#include<graphics.h>//easyx图形库的头文件

#include<mmsystem.h>//音效
#pragma comment(lib,"winmm.lib") 

#include"tools.h"
#include"Sunshine.h"
#include"Zombie.h"
#include"Plant.h"
#include"Bullets.h"
#include"vector2.h"
#include"UI.h"


void gameInit() {
	//初始化游戏背景图片
	loadimage(&imgBg, "res/bg.jpg");
	//初始化植物槽
	loadimage(&imgBar, "res/bar4.png");

	//加载植物卡牌以及其动态帧图片
	memset(imgzhiwu, 0, sizeof(imgzhiwu));
	memset(map, 0, sizeof(map));
	char name[64];
	for (int i = 0; i < zhi_wu_count; i++) {
		sprintf_s(name, sizeof(name), "res/Cards/card_%d.png", i + 1);
		loadimage(&imgCard[i], name);

		for (int j = 0; j < 20; j++) {
			sprintf_s(name, sizeof(name), "res/zhiwu/%d/%d.png", i, j + 1);
			//先判断这个文件是否存在
			if (fileExists(name)) {
				imgzhiwu[i][j] = new IMAGE;
				loadimage(imgzhiwu[i][j], name);
			}
			else break;
		}
	}

	//初始化阳光池
	memset(balls, 0, sizeof(balls));
	for (int i = 0; i < 29; i++) {
		sprintf_s(name, sizeof(name), "res/sunshine/%d.png", i + 1);
		loadimage(&imgSunshine[i], name);
	}

	//初始化随机种子
	srand(time(NULL));

	//初始化阳光值
	sunshine = 50;

	//创建游戏的图形窗口
	initgraph(WIN_Width, WIN_Hight);

	//设置字体
	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 30;
	f.lfWeight = 15;
	strcpy_s(f.lfFaceName, "Segoe UI Black");
	f.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&f);
	setbkmode(TRANSPARENT);
	setcolor(BLACK);

	//初始化僵尸的动态帧图片
	memset(zombies, 0, sizeof(zombies));
	for (int i = 0; i < 22; i++) {
		sprintf_s(name, sizeof(name), "res/zm/%d.png", i + 1);
		loadimage(&imageZombie[i], name);
	}

	//加载正常运动的豌豆子弹
	loadimage(&imgBulletNormal, "res/bullets/bullet_normal.png");
	memset(bullets, 0, sizeof(bullets));

	//加载豌豆子弹的爆炸图片
	loadimage(&imgBallBlast[3], "res/bullets/bullet_blast.png");
	for (int i = 0; i < 3; i++) {
		float k = (i + 1) * 0.2;
		loadimage(&imgBallBlast[i], "res/bullets/bullet_blast.png",
			imgBallBlast[3].getwidth() * k,
			imgBallBlast[3].getheight() * k, true);
	}

	//加载僵尸死亡的图片
	for (int i = 0; i < 20; i++) {
		sprintf_s(name, sizeof(name), "res/zm_dead/%d.png", i + 1);
		loadimage(&imageZmDead[i], name);
	}

	//加载僵尸吃植物的图片
	for (int i = 0; i < 21; i++) {
		sprintf_s(name, sizeof(name), "res/zm_eat/%d.png", i + 1);
		loadimage(&imageZmEat[i], name);
	}

	//加载巡场时僵尸站立的图片
	for (int i = 0; i < 11; i++) {
		sprintf_s(name, sizeof(name), "res/zm_stand/%d.png", i + 1);
		loadimage(&imageZmStand[i], name);
	}

	killCount = 0;
	zmCount = 0;
	gameStatus = GOING;

}

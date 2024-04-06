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
#include"Init.h"



//碰撞检测
void collisionCheck() {
	bullet.checkBullet2Zm();
	plant.checkZm2Plant();
}

void updateWindow() {
	BeginBatchDraw();//开始缓冲

	//加载地图
	putimage(-112, 0, &imgBg);
	//加载植物槽
	putimagePNG(250, 0, &imgBar);
	for (int i = 0; i < zhi_wu_count; i++) {
		int x = 347 + i * 70;
		putimage(x, 8, &imgCard[i]);
	}

	//渲染拖动后的植物卡牌
	if (curPlant > 0) {
		IMAGE* img = imgzhiwu[curPlant - 1][0];
		putimagePNG(curX - img->getwidth() / 2, curY - img->getwidth() / 2, img);
	}

	plant.updatePlant();

	ball.drawSunshine();

	//显示阳光值
	char scoreText[8];
	sprintf_s(scoreText, sizeof(scoreText), "%d", sunshine);
	outtextxy(285, 75, scoreText);


	//渲染僵尸
	zm.drawZombie();

	//渲染豌豆子弹
	bullet.showBullets();//渲染运动
	bullet.showBlast();//渲染爆炸


	EndBatchDraw();//结束双缓冲
}


void userClick() {
	ExMessage msg;
	static int status = 0;

	if (peekmessage(&msg)) {
		if (msg.message == WM_LBUTTONDOWN) {
			//选定植物
			if (msg.x > 347 && msg.x < 347 + 70 * zhi_wu_count && msg.y < 110) {
				map[0][0].choosePlant(msg, status);
			}
			//收集阳光
			else {
				balls[0].collectSunshine(&msg);
			}
		}
		else if (msg.message == WM_MOUSEMOVE) {
			curX = msg.x;
			curY = msg.y;
		}
		else if (msg.message == WM_LBUTTONUP) {
			map[0][0].setPlant(msg, status);
		}
	}
}

void updateGame() {
	//实现植物动态帧图片的更新
	plant.Plant_FrameIndex();

	//*实现阳光的更新
	ball.creatSunshine();//产生阳光的数据
	ball.updateSunshine();//更新阳光的动态帧

	//*实现僵尸的更新
	zm.creatZombie();
	zm.updateZombie();

	//发射豌豆子弹
	bullet.creatBullet();
	bullet.updateBullet();
	//碰撞检测，包括两种检测
	collisionCheck();

}

bool checkOver() {
	int ret = false;
	if (gameStatus == WIN) {
		Sleep(2000);
		loadimage(0, "res/win2.png");
		ret = true;
	}
	else if (gameStatus == FAIL) {
		Sleep(2000);
		loadimage(0, "res/fail2.png");
		ret = true;
	}
	return ret;
}
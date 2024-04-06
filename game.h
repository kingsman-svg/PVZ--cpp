#pragma once

#include<iostream>
#include <fstream>
using namespace std;
#include<stdio.h>
#include"math.h"
#include<graphics.h>//easyxͼ�ο��ͷ�ļ�

#include<mmsystem.h>//��Ч
#pragma comment(lib,"winmm.lib") 

#include"tools.h"
#include"Sunshine.h"
#include"Zombie.h"
#include"Plant.h"
#include"Bullets.h"
#include"vector2.h"
#include"UI.h"
#include"Init.h"



//��ײ���
void collisionCheck() {
	bullet.checkBullet2Zm();
	plant.checkZm2Plant();
}

void updateWindow() {
	BeginBatchDraw();//��ʼ����

	//���ص�ͼ
	putimage(-112, 0, &imgBg);
	//����ֲ���
	putimagePNG(250, 0, &imgBar);
	for (int i = 0; i < zhi_wu_count; i++) {
		int x = 347 + i * 70;
		putimage(x, 8, &imgCard[i]);
	}

	//��Ⱦ�϶����ֲ�￨��
	if (curPlant > 0) {
		IMAGE* img = imgzhiwu[curPlant - 1][0];
		putimagePNG(curX - img->getwidth() / 2, curY - img->getwidth() / 2, img);
	}

	plant.updatePlant();

	ball.drawSunshine();

	//��ʾ����ֵ
	char scoreText[8];
	sprintf_s(scoreText, sizeof(scoreText), "%d", sunshine);
	outtextxy(285, 75, scoreText);


	//��Ⱦ��ʬ
	zm.drawZombie();

	//��Ⱦ�㶹�ӵ�
	bullet.showBullets();//��Ⱦ�˶�
	bullet.showBlast();//��Ⱦ��ը


	EndBatchDraw();//����˫����
}


void userClick() {
	ExMessage msg;
	static int status = 0;

	if (peekmessage(&msg)) {
		if (msg.message == WM_LBUTTONDOWN) {
			//ѡ��ֲ��
			if (msg.x > 347 && msg.x < 347 + 70 * zhi_wu_count && msg.y < 110) {
				map[0][0].choosePlant(msg, status);
			}
			//�ռ�����
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
	//ʵ��ֲ�ﶯ̬֡ͼƬ�ĸ���
	plant.Plant_FrameIndex();

	//*ʵ������ĸ���
	ball.creatSunshine();//�������������
	ball.updateSunshine();//��������Ķ�̬֡

	//*ʵ�ֽ�ʬ�ĸ���
	zm.creatZombie();
	zm.updateZombie();

	//�����㶹�ӵ�
	bullet.creatBullet();
	bullet.updateBullet();
	//��ײ��⣬�������ּ��
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
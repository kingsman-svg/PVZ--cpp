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


void gameInit() {
	//��ʼ����Ϸ����ͼƬ
	loadimage(&imgBg, "res/bg.jpg");
	//��ʼ��ֲ���
	loadimage(&imgBar, "res/bar4.png");

	//����ֲ�￨���Լ��䶯̬֡ͼƬ
	memset(imgzhiwu, 0, sizeof(imgzhiwu));
	memset(map, 0, sizeof(map));
	char name[64];
	for (int i = 0; i < zhi_wu_count; i++) {
		sprintf_s(name, sizeof(name), "res/Cards/card_%d.png", i + 1);
		loadimage(&imgCard[i], name);

		for (int j = 0; j < 20; j++) {
			sprintf_s(name, sizeof(name), "res/zhiwu/%d/%d.png", i, j + 1);
			//���ж�����ļ��Ƿ����
			if (fileExists(name)) {
				imgzhiwu[i][j] = new IMAGE;
				loadimage(imgzhiwu[i][j], name);
			}
			else break;
		}
	}

	//��ʼ�������
	memset(balls, 0, sizeof(balls));
	for (int i = 0; i < 29; i++) {
		sprintf_s(name, sizeof(name), "res/sunshine/%d.png", i + 1);
		loadimage(&imgSunshine[i], name);
	}

	//��ʼ���������
	srand(time(NULL));

	//��ʼ������ֵ
	sunshine = 50;

	//������Ϸ��ͼ�δ���
	initgraph(WIN_Width, WIN_Hight);

	//��������
	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 30;
	f.lfWeight = 15;
	strcpy_s(f.lfFaceName, "Segoe UI Black");
	f.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&f);
	setbkmode(TRANSPARENT);
	setcolor(BLACK);

	//��ʼ����ʬ�Ķ�̬֡ͼƬ
	memset(zombies, 0, sizeof(zombies));
	for (int i = 0; i < 22; i++) {
		sprintf_s(name, sizeof(name), "res/zm/%d.png", i + 1);
		loadimage(&imageZombie[i], name);
	}

	//���������˶����㶹�ӵ�
	loadimage(&imgBulletNormal, "res/bullets/bullet_normal.png");
	memset(bullets, 0, sizeof(bullets));

	//�����㶹�ӵ��ı�ըͼƬ
	loadimage(&imgBallBlast[3], "res/bullets/bullet_blast.png");
	for (int i = 0; i < 3; i++) {
		float k = (i + 1) * 0.2;
		loadimage(&imgBallBlast[i], "res/bullets/bullet_blast.png",
			imgBallBlast[3].getwidth() * k,
			imgBallBlast[3].getheight() * k, true);
	}

	//���ؽ�ʬ������ͼƬ
	for (int i = 0; i < 20; i++) {
		sprintf_s(name, sizeof(name), "res/zm_dead/%d.png", i + 1);
		loadimage(&imageZmDead[i], name);
	}

	//���ؽ�ʬ��ֲ���ͼƬ
	for (int i = 0; i < 21; i++) {
		sprintf_s(name, sizeof(name), "res/zm_eat/%d.png", i + 1);
		loadimage(&imageZmEat[i], name);
	}

	//����Ѳ��ʱ��ʬվ����ͼƬ
	for (int i = 0; i < 11; i++) {
		sprintf_s(name, sizeof(name), "res/zm_stand/%d.png", i + 1);
		loadimage(&imageZmStand[i], name);
	}

	killCount = 0;
	zmCount = 0;
	gameStatus = GOING;

}

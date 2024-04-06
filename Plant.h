#pragma once
#include<graphics.h>//easyxͼ�ο��ͷ�ļ�
#include<iostream>
#include"Sunshine.h"

#define WIN_Width 900
#define WIN_Hight 600


class Plant
{
public:
	int type;//0:û��ѡ�У�1:ѡ�е�һ��ֲ��Դ�����
	int frameIndex;//����֡�����
	bool catched;//�Ƿ����ڱ���
	int deadTime;//����������
	int timer;//���տ��������ⶨʱ��
	int x, y;
	int shootTime;

	void updatePlant();
	void Plant_FrameIndex();
	void choosePlant(ExMessage& msg, static int status);
	void setPlant(ExMessage& msg, static int status);
	void checkZm2Plant();

};

Plant map[3][9];
Plant plant;
extern int sunshine;

int curX, curY;//ѡ��ֲ�￨���ƶ������е�ʵʱλ��
int curPlant;//��ʾѡ�е�����һ��ֲ�￨

//ֲ�￨�۵�ö��
enum { wan_dou, xiang_ri_kui, zhi_wu_count };

//ÿ��ֲ�￨��ͼƬ
IMAGE imgCard[zhi_wu_count];
//ÿ��ֲ�￨�Ķ�̬֡����ά��
IMAGE* imgzhiwu[zhi_wu_count][20];


//�ӿ�����ѡ��ֲ��
void Plant::choosePlant(ExMessage& msg, static int status) {
	int index = (msg.x - 347) / 70;
	if ((index == 0 && sunshine < 100) || (index == 1 && sunshine < 50)) return;
	else {
		status = 1;
		curPlant = index + 1;
	}
}
//��ֲֲ��
void Plant::setPlant(ExMessage& msg, static int status){
	if (msg.x > 253-112 && msg.y > 174 && msg.y < 490) {
		int row = (msg.y - 174) / 100;
		int col = (msg.x - (253-112)) / 77;

		if (map[row][col].type == 0) {
			map[row][col].type = curPlant;
			map[row][col].frameIndex = 0;
			map[row][col].shootTime = 0;

			map[row][col].x = 260-112 + col * 80;
			map[row][col].y = 180 + row * 96;

			if (map[row][col].type == wan_dou+1) sunshine -= 100;
			else if (map[row][col].type == xiang_ri_kui+1) sunshine -= 50;
		}
	}
	curPlant = 0;
	status = 0;
}
//��Ⱦ��ֲ��ֲ��
void Plant::updatePlant() {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 9; j++) {
			if (map[i][j].type > 0) {
				int type = map[i][j].type - 1;
				int index = map[i][j].frameIndex;
				putimagePNG(map[i][j].x, map[i][j].y, imgzhiwu[type][index]);
			}
		}
	}
}
//����ֲ��Ķ�̬֡
void Plant:: Plant_FrameIndex() {
	static int count = 0;
	if (++count < 2)return;
	count = 0;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 9; j++) {
			//����У�����¸�ֲ��Ķ�̬֡ͼƬ
			if (map[i][j].type > 0) {
				map[i][j].frameIndex++;
				int PlantType = map[i][j].type - 1;
				int PlantIndex = map[i][j].frameIndex;
				if (imgzhiwu[PlantType][PlantIndex] == NULL) {
					map[i][j].frameIndex = 0;
				}
			}
		}
	}
}
//��ʬ��ֲ�����ײ
void Plant:: checkZm2Plant() {
	int zCount = sizeof(zombies) / sizeof(zombies[0]);
	for (int i = 0; i < zCount; i++) {
		if (zombies[i].dead) continue;

		int row = zombies[i].row;
		for (int k = 0; k < 9; k++) {
			if (map[row][k].type == 0)continue;

			int PlantX = 260-112 + k * 80;
			int x1 = PlantX + 10;
			int x2 = PlantX + 60;
			int x3 = zombies[i].x + 80;
			if (x3 > x1 && x3 < x2) {
				if (map[row][k].catched) {
					map[row][k].deadTime++;
					if (map[row][k].deadTime > 100) {
						map[row][k].deadTime = 0;
						map[row][k].type = 0;
						zombies[i].eating = false;
						zombies[i].frameIndex = 0;
						zombies[i].speed = 1;
					}
				}
				else {
					map[row][k].catched = true;
					map[row][k].deadTime = 0;
					zombies[i].eating = true;
					zombies[i].speed = 0;
					zombies[i].frameIndex = 0;
				}
			}
			int PlantY = 180 + i * 96;

		}
	}
}




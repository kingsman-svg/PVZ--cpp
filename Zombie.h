#pragma once
#include<graphics.h>//easyxͼ�ο��ͷ�ļ�
#include<iostream>

#define WIN_Width 900
#define WIN_Hight 600
#define ZM_MAX 10


class zombie {

public:
	int x, y;
	int frameIndex;
	bool used;
	int speed;
	int row;
	int blood;
	bool dead;
	bool eating;

	void drawZombie();
	void updateZombie();
	void creatZombie();
	
};

//��ʬ��
zombie zombies[10];
//��ʬ�Ķ�̬֡����
IMAGE imageZombie[22];
IMAGE imageZmDead[20];
IMAGE imageZmEat[21];
IMAGE imageZmStand[11];

zombie zm;

enum { GOING, WIN, FAIL };
int killCount;//�Ѿ���ɱ���Ľ�ʬ����
int zmCount;//�Ѿ����ֵĽ�ʬ
int gameStatus;


//��Ⱦ��ʬ
void zombie::drawZombie() {
	int zmCount = sizeof(zombies) / sizeof(zombies[0]);
	for (int i = 0; i < zmCount; i++) {
		if (zombies[i].used) {
			IMAGE* img = NULL;
			if (zombies[i].dead) img = imageZmDead;
			else if (zombies[i].eating) img = imageZmEat;
			else img = imageZombie;

			img += zombies[i].frameIndex;

			putimagePNG(zombies[i].x, zombies[i].y - img->getheight(), img);
		}
	}
}

//���½�ʬ�Ķ�̬֡
void zombie::updateZombie() {
	int zombieMax = sizeof(zombies) / sizeof(zombies[0]);

	static int count = 0;
	count++;
	if (count > 2) {
		count = 0;
		//���½�ʬ��λ��
		for (int i = 0; i < zombieMax; i++) {
			if (zombies[i].used) {
				zombies[i].x -= zombies[i].speed;
				//��Ϸ�����жϣ����Ż���
				if (zombies[i].x <56){
					gameStatus = FAIL;
				}
			}
		}
	}

	//���½�ʬ�Ķ�̬֡
	static int count2 = 0;
	count2++;
	if (count2 > 3) {
		count2 = 0;
		for (int i = 0; i < zombieMax; i++) {
			if (zombies[i].used) {

				if (zombies[i].dead) {
					zombies[i].frameIndex++;
					if (zombies[i].frameIndex >= 20) {
						zombies[i].used = false;
						killCount++;
						if (killCount == ZM_MAX) {
							gameStatus = WIN;
						}
					}
				}
				else if (zombies[i].eating) {
					zombies[i].frameIndex = (zombies[i].frameIndex + 1) % 21;
				}

				else {
					zombies[i].frameIndex = (zombies[i].frameIndex + 1) % 22;
				}
			}
		}
	}



}

//������ʬ������
void zombie::creatZombie() {
	if (zmCount >= ZM_MAX) {
		return;
	}

	static int count = 0;
	int fre = 200;
	count++;
	if (count >= fre) {
		count = 0;
		fre = rand() % 200 + 300;

		int i;
		int zombieMax = sizeof(zombies) / sizeof(zombies[0]);
		for (i = 0; i < zombieMax && zombies[i].used; i++);
		if (i < zombieMax) {
			
			memset(&zombies[i], 0, sizeof(zombies[i]));

			zombies[i].used = true;
			zombies[i].x = WIN_Width;
			zombies[i].row = rand() % 3;
			zombies[i].y = 180 + (1 + zombies[i].row) * 97;
			zombies[i].speed = 1;
			zombies[i].blood = 100;
			zombies[i].dead = false;
			zombies[i].eating = false;
		}
	}
}


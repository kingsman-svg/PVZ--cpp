#pragma once
#include<graphics.h>//easyxͼ�ο��ͷ�ļ�
#include"vector2.h" 
#include"tools.h"
#include"Zombie.h"
#include"Plant.h"
#include"Bullets.h"



class SunshineBall
{
public:
	int x, y;//Ʈ������е�ʵʱλ��
	int frameIndex;//��ǰ��ʾ��ͼƬ֡�����
	int destY;//Ʈ���Ŀ��λ�õ�y����
	bool used;
	int timer;

	float xoff;
	float yoff;

	float t;//���������ߵ�ʱ��㣬0...1
	vector2 p1, p2, p3, p4;
	vector2 pCur;//��ǰʱ���������λ��
	float speed;
	int status;//��Ӧenum�е�����״̬

	void creatSunshine();
	void updateSunshine();
	void collectSunshine(ExMessage *msg);
	void drawSunshine();

};

//���տ��������������״̬
enum { SUNSHINE_DOWN, SUNSHINE_GROUND, SUNSHINE_COLLECT, SUNSHINE_CREAT };

SunshineBall ball;
SunshineBall balls[10];//�����
IMAGE imgSunshine[29];//������Ķ�̬֡����
int sunshine;//���Ͻ���ʾ������ֵ

//��ʼ�����������
void SunshineBall::creatSunshine() {
	//1.ʵ����������ʱ�����
	static int count = 0;
	int fre = 200;
	count++;
	if (count >= fre) {
		fre = 200 + rand() % 200;
		count = 0;

		//���������ȡһ������ʹ�õ�����
		int ballMax = sizeof(balls) / sizeof(balls[0]);

		int i = 0;
		for (i = 0; i < ballMax && balls[i].used; i++);
		if (i >= ballMax) return;

		balls[i].used = true;
		balls[i].frameIndex = 0;

		balls[i].timer = 0;

		balls[i].status = SUNSHINE_DOWN;
		balls[i].t = 0;
		balls[i].p1 = vector2(260-112 + rand() % (950 - 260 + 112), 60);
		balls[i].p4 = vector2(balls[i].p1.x, 200 + (rand() % 4) * 90);
		int off = 2;
		float distance = balls[i].p4.y - balls[i].p1.y;
		balls[i].speed = 1.0 / (distance / off);
	}

	//2.���տ���������
	int ballCount = sizeof(balls) / sizeof(balls[0]);
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 9; j++) {
			if (map[i][j].type == xiang_ri_kui + 1) {
				map[i][j].timer++;
				if (map[i][j].timer > 400 + rand() % 300 ) {
					map[i][j].timer = 0;

					int k;
					for (k = 0; k < ballCount&&balls[k].used; k++);
					if (k >= ballCount) return;

					balls[k].used = true;
					balls[k].p1 = vector2(map[i][j].x, map[i][j].y);
					int w = 100 + (rand() % 51) * (rand() % 2 ? 1 : -1);
					balls[k].p4 = vector2(map[i][j].x + w, 
						map[i][j].y + imgzhiwu[xiang_ri_kui][0]->getheight() - 
						imgSunshine[0].getheight());
					balls[k].p2 = vector2(balls[k].p1.x + w * 0.3, balls[k].p1.y - 100);
					balls[k].p3 = vector2(balls[k].p1.x + w * 0.7, balls[k].p1.y - 100);
					balls[k].status = SUNSHINE_CREAT;
					balls[k].speed = 0.05;
					balls[k].t = 0;
				}
			}
		}
	}


}
//��������Ķ�̬֡
void SunshineBall::updateSunshine() {
	int ballMax = sizeof(balls) / sizeof(balls[0]);
	for (int i = 0; i < ballMax; i++) {
		if (balls[i].used) {
			balls[i].frameIndex = (balls[i].frameIndex + 1) % 29;
			if (balls[i].status == SUNSHINE_DOWN) {
				//SunshineBall* sun = &balls[i];
				balls[i].t += balls[i].speed;
				balls[i].pCur = balls[i].p1 + balls[i].t * (balls[i].p4 - balls[i].p1);
				if (balls[i].t >= 1) {
					balls[i].status = SUNSHINE_GROUND;
					balls[i].timer = 0;
				}
			}
			else if (balls[i].status == SUNSHINE_GROUND) {
				balls[i].timer++;
				if (balls[i].timer > 100) {
					balls[i].used = false;
					balls[i].timer = 0;
				}
			}
			else if (balls[i].status == SUNSHINE_COLLECT) {
				balls[i].t += balls[i].speed;
				balls[i].pCur = balls[i].p1 + balls[i].t * (balls[i].p4 - balls[i].p1);
				if (balls[i].t > 1) {
					balls[i].used = false;
					sunshine += 25;
				}
			}
			else if (balls[i].status == SUNSHINE_CREAT) {
				balls[i].t += balls[i].speed;
				balls[i].pCur = calcBezierPoint(balls[i].t, balls[i].p1, balls[i].p2, balls[i].p3, balls[i].p4);
				if (balls[i].t > 1) {
					balls[i].status = SUNSHINE_GROUND;
					balls[i].timer = 0;
				}
			}
		}
	}

	
}
//�ռ�����
void SunshineBall::collectSunshine(ExMessage* msg) {
	int count = sizeof(balls) / sizeof(balls[0]);
	int w = imgSunshine[0].getwidth();
	int h = imgSunshine[0].getheight();
	for (int i = 0; i < count; i++) {
		if (balls[i].used) {
			int x = balls[i].pCur.x;
			int y = balls[i].pCur.y;

			if (msg->x > x && msg->x<x + w && msg->y>y && msg->y < y + h) {

				balls[i].status = SUNSHINE_COLLECT;
				mciSendString("play res/sunshine.mp3", 0, 0, 0);

				balls[i].p1 = balls[i].pCur;
				balls[i].p4 = vector2(280, 0);
				balls[i].t = 0;
				float distance = dis(balls[i].p1 - balls[i].p4);
				float off = 16;
				balls[i].speed = 1.0 / (distance / off);
				break;
			}
		}
	}
}
//��������
void SunshineBall::drawSunshine() {
	//��Ⱦ����
	int ballMax = sizeof(balls) / sizeof(balls[0]);
	for (int i = 0; i < ballMax; i++) {
		if (balls[i].used) {
			IMAGE* img = &imgSunshine[balls[i].frameIndex];
			//putimagePNG(balls[i].x, balls[i].y, img);
			putimagePNG(balls[i].pCur.x, balls[i].pCur.y, img);
		}
	}
}

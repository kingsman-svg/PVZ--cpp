#pragma once
#include<graphics.h>//easyx图形库的头文件
#include"vector2.h" 
#include"tools.h"
#include"Zombie.h"
#include"Plant.h"
#include"Bullets.h"



class SunshineBall
{
public:
	int x, y;//飘落过程中的实时位置
	int frameIndex;//当前显示的图片帧的序号
	int destY;//飘落的目标位置的y坐标
	bool used;
	int timer;

	float xoff;
	float yoff;

	float t;//贝塞尔曲线的时间点，0...1
	vector2 p1, p2, p3, p4;
	vector2 pCur;//当前时刻阳光球的位置
	float speed;
	int status;//对应enum中的阳光状态

	void creatSunshine();
	void updateSunshine();
	void collectSunshine(ExMessage *msg);
	void drawSunshine();

};

//向日葵生产阳光的四种状态
enum { SUNSHINE_DOWN, SUNSHINE_GROUND, SUNSHINE_COLLECT, SUNSHINE_CREAT };

SunshineBall ball;
SunshineBall balls[10];//阳光池
IMAGE imgSunshine[29];//阳光球的动态帧数组
int sunshine;//左上角显示的阳光值

//初始化阳光的数据
void SunshineBall::creatSunshine() {
	//1.实现阳光的随机时间产生
	static int count = 0;
	int fre = 200;
	count++;
	if (count >= fre) {
		fre = 200 + rand() % 200;
		count = 0;

		//从阳光池中取一个可以使用的阳光
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

	//2.向日葵生产阳光
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
//更新阳光的动态帧
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
//收集阳光
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
//绘制阳光
void SunshineBall::drawSunshine() {
	//渲染阳光
	int ballMax = sizeof(balls) / sizeof(balls[0]);
	for (int i = 0; i < ballMax; i++) {
		if (balls[i].used) {
			IMAGE* img = &imgSunshine[balls[i].frameIndex];
			//putimagePNG(balls[i].x, balls[i].y, img);
			putimagePNG(balls[i].pCur.x, balls[i].pCur.y, img);
		}
	}
}

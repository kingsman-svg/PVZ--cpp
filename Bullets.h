#pragma once
#include<graphics.h>//easyx图形库的头文件
#include<iostream>

class Bullets {
public:
	int x, y;
	int row;
	bool used;
	int speed;
	bool blast;//是否发生爆炸
	int frameIndex;//爆炸图片的帧序号

	void creatBullet();
	void updateBullet();
	void checkBullet2Zm();
	void showBullets();
	void showBlast();

};

Bullets bullet;
Bullets bullets[200];//子弹池

IMAGE imgBulletNormal;//运动中的子弹图片
IMAGE imgBallBlast[4];//爆炸图片

void Bullets::creatBullet() {
	int lines[3] = { 0 };
	int zombieCount = sizeof(zombies) / sizeof(zombies[0]);
	int bulletNum = sizeof(bullets) / sizeof(bullets[0]);
	int dangerX = WIN_Width - imageZombie[0].getwidth() / 2;

	for (int i = 0; i < zombieCount; i++) {
		if (zombies[i].used && zombies[i].x < dangerX) {
			lines[zombies[i].row] = 1;
		}
	}

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 9; j++) {
			if (map[i][j].type == wan_dou + 1 && lines[i]) {
				//static int count = 0;
				//count++;
				map[i][j].shootTime++;
				if (map[i][j].shootTime > 50) {
					map[i][j].shootTime = 0;

					int k;
					for (k = 0; k < bulletNum && bullets[k].used; k++);
					if (k < bulletNum) {
						bullets[k].used = true;
						bullets[k].row = i;
						bullets[k].speed = 6;
						bullets[k].blast = false;
						bullets[k].frameIndex = 0;

						int PlantX = 260 - 112 + j * 80;
						int PlantY = 180 + i * 96;
						bullets[k].x = PlantX + imgzhiwu[wan_dou][0]->getwidth();
						bullets[k].y = PlantY + 5;
					}
				}

			}
		}
	}
}

void Bullets::updateBullet() {
	int bulletNum = sizeof(bullets) / sizeof(bullets[0]);
	for (int i = 0; i < bulletNum; i++) {
		if (bullets[i].used) {
			//子弹运动
			bullets[i].x += bullets[i].speed;
			//边界处理
			if (bullets[i].x > WIN_Width) {
				bullets[i].used = false;
			}
			//碰撞处理
			if (bullets[i].blast) {
				bullets[i].frameIndex++;
				if (bullets[i].frameIndex >= 4) {
					bullets[i].used = false;
				}
			}
		}
	}
}

void Bullets::checkBullet2Zm() {
	int bulletCount = sizeof(bullets) / sizeof(bullets[0]);
	int zombieCount = sizeof(zombies) / sizeof(zombies[0]);

	for (int i = 0; i < bulletCount; i++) {
		if (bullets[i].used == false || bullets[i].blast == true) continue;
		for (int k = 0; k < zombieCount; k++) {
			if (zombies[k].used == false) continue;
			int x1 = zombies[k].x + 80;
			int x2 = zombies[k].x + 110;
			int x = bullets[i].x;
			if (zombies[k].dead == false && bullets[i].row == zombies[k].row && x > x1 && x < x2) {
				zombies[k].blood -= 15;
				bullets[i].blast = true;
				bullets[i].speed = 0;

				if (zombies[k].blood <= 0) {
					zombies[k].dead = true;
					zombies[k].speed = 0;
					zombies[k].frameIndex = 0;
				}

				break;
			}
		}
	}
}

void Bullets::showBullets() {
	int bulletNum = sizeof(bullets) / sizeof(bullets[0]);
	for (int i = 0; i < bulletNum; i++) {
		if (bullets[i].used) {
			putimagePNG(bullets[i].x, bullets[i].y, &imgBulletNormal);
		}
	}
}

void Bullets::showBlast() {
	int bulletMax = sizeof(bullets) / sizeof(bullets[0]);
	for (int i = 0; i < bulletMax; i++) {
		if (bullets[i].used) {
			if (bullets[i].blast) {
				IMAGE* img = &imgBallBlast[bullets[i].frameIndex];
				putimagePNG(bullets[i].x, bullets[i].y, img);
			}
			else {
				putimagePNG(bullets[i].x, bullets[i].y, &imgBulletNormal);
			}
		}
	}
}
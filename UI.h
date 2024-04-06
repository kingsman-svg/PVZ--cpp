#pragma once

#include<graphics.h>//easyx图形库的头文件
#include"tools.h"
//开始界面
void startUI() {
	IMAGE imgBg, imgMenu1, imgMenu2;
	loadimage(&imgBg, "res/menu.png");
	loadimage(&imgMenu1, "res/menu1.png");
	loadimage(&imgMenu2, "res/menu2.png");

	int flag = 0;
	while (1) {
		BeginBatchDraw();
		putimage(0, 0, &imgBg);
		putimagePNG(460, 75, flag ? &imgMenu1 : &imgMenu2);

		ExMessage msg;
		if (peekmessage(&msg)) {
			if (msg.message == WM_LBUTTONDOWN
				&& msg.x > 460 && msg.x < 460 + 310
				&& msg.y>75 && msg.y < 75 + 140) {
				flag = 1;
			}
			else if (msg.message == WM_LBUTTONUP && flag) {
				EndBatchDraw();
				break;
			}
		}

		EndBatchDraw();
	}
}


IMAGE imgBg;
IMAGE imgBar;
//卡片槽下滑
void barsDown() {
	int height = imgBar.getheight();
	for (int y = -height; y <= 0; y++) {
		BeginBatchDraw();

		putimage(-112, 0, &imgBg);
		putimagePNG(250, y, &imgBar);

		for (int i = 0; i < zhi_wu_count; i++) {
			int x = 347 + i * 70;
			putimage(x, y + 6, &imgCard[i]);
		}

		EndBatchDraw();
		Sleep(10);
	}
}

//场景巡回
void viewScene() {
	int xMin = WIN_Width - imgBg.getwidth();
	vector2 points[9] = {
		{550,80},{530,160},{630,170},{530,200},{690,340}
	};
	int index[5];
	for (int i = 0; i < 5; i++)
	{
		index[i] = rand() % 11;
	}

	//向右移动
	int count = 0;
	for (int x = 0; x >= xMin; x -= 2) {
		BeginBatchDraw();
		putimage(x, 0, &imgBg);

		count++;
		for (int k = 0; k < 5; k++) {
			putimagePNG(points[k].x - xMin + x, points[k].y, &imageZmStand[index[k]]);
			if (count >= 7) {
				index[k] = (index[k] + 1) % 11;
			}
		}
		if (count >= 10)count = 0;

		EndBatchDraw();
		Sleep(5);
	}

	//停留
	for (int i = 0; i < 100; i++) {
		BeginBatchDraw();
		putimage(xMin, 0, &imgBg);
		for (int k = 0; k < 5; k++) {
			putimagePNG(points[k].x, points[k].y, &imageZmStand[index[k]]);
			index[k] = (index[k] + 1) % 11;
		}
		EndBatchDraw();
		Sleep(20);
	}

	//向左切回
	for (int x = xMin; x <= -112; x += 2) {
		BeginBatchDraw();
		putimage(x, 0, &imgBg);

		count++;
		for (int k = 0; k < 5; k++) {
			putimagePNG(points[k].x - xMin + x, points[k].y, &imageZmStand[index[k]]);
			if (count >= 7) {
				index[k] = (index[k] + 1) % 11;
			}
			if (count >= 10)count = 0;
		}
		EndBatchDraw();
		Sleep(5);
	}
}
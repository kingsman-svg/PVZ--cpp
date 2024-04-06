
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
#include"game.h"


#define WIN_Width 900
#define WIN_Hight 600



int main() {
	gameInit();
	startUI();
	viewScene();
	barsDown();

	int timer = 0;
	bool flag = true;
	while (1) {
		userClick();
		timer += getDelay();
		if (timer > 20) {
			flag = true;
			timer = 0;
		}

		if (flag) {
			flag = false;
			updateWindow();
			updateGame();
			if (checkOver()) break;
		}
	}
	system("pause");
	return 0;
}
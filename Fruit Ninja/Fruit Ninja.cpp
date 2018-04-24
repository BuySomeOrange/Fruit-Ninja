#include "EasyXPng.h"
#include <graphics.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <fstream>
#pragma comment(lib,"Winmm.lib")
using namespace std;
//定义全局变量
#define width 640//窗口大小
#define high 480
#define PI 3.14159

IMAGE img_bk;
IMAGE_PNG png_homemask;     //主界面半透明层 640*183
IMAGE_PNG png_logo;     //logo 288*135
IMAGE_PNG png_ninja;     //ninja 244*81
IMAGE_PNG png_apple;     //苹果 66*66
IMAGE_PNG png_apple_;        //加 _ 的为静态图，不加 _ 为旋转后的图
IMAGE_PNG png_apple1, png_apple1_, png_apple2, png_apple2_;   //被切成两半的苹果  66*66
IMAGE_PNG png_sandia;   //西瓜 98*85
IMAGE_PNG png_sandia_;
IMAGE_PNG png_sandia1, png_sandia1_, png_sandia2, png_sandia2_;   //被切成两半的西瓜  98*85
IMAGE_PNG png_peach;   //桃子  62*59
IMAGE_PNG png_peach_;
IMAGE_PNG png_peach1, png_peach1_, png_peach2, png_peach2_;   //被切成两半的桃子  62*59
IMAGE_PNG png_banana;   //香蕉  126*50
IMAGE_PNG png_banana_;
IMAGE_PNG png_banana1, png_banana1_, png_banana2, png_banana2_;   //被切成两半的香蕉  126*50
IMAGE_PNG png_strawberry;   //草莓  68*72
IMAGE_PNG png_strawberry_;
IMAGE_PNG png_strawberry1, png_strawberry1_, png_strawberry2, png_strawberry2_;//被切成两半的草莓  68*72
IMAGE_PNG png_boom;     //炸弹 66*68
IMAGE_PNG png_boom_;
IMAGE_PNG png_newgame;  //新游戏（圈） 195*195
IMAGE_PNG png_newgame_;
IMAGE_PNG png_quit;     //退出游戏（圈） 141*141
IMAGE_PNG png_quit_;
IMAGE_PNG png_dojo;      //dojo  175*175
IMAGE_PNG png_dojo_;
IMAGE_PNG png_apple_flash;  //刀切到苹果时闪烁  200*200
IMAGE_PNG png_sandia_flash; //刀切到西瓜时闪烁  230*230
IMAGE_PNG png_banana_flash;  //刀切到香蕉时闪烁  200*200
IMAGE_PNG png_peach_flash;  //刀切到桃子时闪烁  200*200
IMAGE_PNG png_strawberry_flash;  //刀切到草莓时闪烁  200*200
IMAGE_PNG png_HomeDesc;  //home-desc
IMAGE_PNG png_new;  //new
IMAGE_PNG png_score;  //分数图标 29*31
IMAGE_PNG png_gameover;   //Game-Over

int lose = 0;
IMAGE_PNG png_x;
IMAGE_PNG png_xx;
IMAGE_PNG png_xxx;
IMAGE_PNG png_xf;
IMAGE_PNG png_xxf;
IMAGE_PNG png_xxxf;

int isshow_sandia = 1;  // 西瓜被切到时该值为0
int sandia1_x=0, sandia_y =0;
int sandia2_x=0;
int isshow_peach = 1;   //桃子被切到时该值为0
int peach1_x = 0, peach_y = 0;
int peach2_x = 0;
int score_classicmode = 0;    //记录分数
int BestScore_classicmode = 0;
int score_dojo = 0;
int BestScore_dojomode = 0;
time_t dojo_start;   //禅模式开始时间
int time_now;  //禅模式倒计时

struct {         //定义10个水果对象 0,1表示苹果 2,3西瓜 4,5香蕉 6,7桃子 8,9草莓
	IMAGE_PNG png_fruit;
	double posi_x = 0, posi_y = 0;
	double velo_x = 0, velo_y = 0;
	double angle = 0;
	int direction = 0;//旋转方向，0表示不显示
	int is_flash;      //是否显示闪烁
	int is_show = -1;    //-1不显示且可刷新 0不显示且不可刷新 1正在显示
}show_fruit[10];

struct {         //定义炸弹
	IMAGE_PNG png_boom;
	double posi_x = 0, posi_y = 0;
	double velo_x = 0, velo_y = 0;
	double angle = 0;
	int direction = 0;//旋转方向，0表示不显示
	int is_cut = 0;      //是否被切到
	int is_show = 0;
}show_boom;

struct {            //定义10个被切开的水果对象 0,1表示苹果 2,3西瓜 4,5香蕉 6,7桃子 8,9草莓
	IMAGE_PNG fruit1;
	IMAGE_PNG fruit2;
	double posi_x1 = 0, posi_x2 = 0, posi_y = 0;
	double velo_x = 0.55, velo_y = 0;
	double angle = 0;
	int is_show = 0;
}show_fruit_cut[10];

double rotate_fruit=0, rotate_circle=0;         //主界面水果和圈的旋转速度
int flag = 0; //界面标记 0表示主界面 1表示经典模式 2表示禅模式
int count_fruitknife = 0;  //控制刀光位置
int flag_fruitknife = 0;
double fruitknife_x[4] = { -1,-1,-1,-1 }, fruitknife_y[4] = { -1,-1,-1,-1 };//刀光初始位置

HWND hwnd;
void startup() {
	srand((unsigned)time(NULL));
	initgraphEx();     //初始化界面
	initgraph(width, high);
	hwnd = GetHWnd();
	//*********************以下为读取图片************************//
	loadimage(&png_homemask, ".\\source\\images\\home-mask.png");
	loadimage(&png_logo, ".\\source\\images\\logo.png");
	loadimage(&png_ninja, ".\\source\\images\\ninja.png");
	loadimage(&img_bk, ".\\source\\images\\background.jpg");//加载图片
	loadimage(&png_HomeDesc, ".\\source\\images\\home-desc.png");
	loadimage(&png_new, ".\\source\\images\\new.png");
	loadimage(&png_score, ".\\source\\images\\score.png");
	loadimage(&png_gameover, ".\\source\\images\\game-over.png");
	loadimage(&png_x, ".\\source\\images\\x.png");
	loadimage(&png_xx, ".\\source\\images\\xx.png");
	loadimage(&png_xxx, ".\\source\\images\\xxx.png");
	loadimage(&png_xf, ".\\source\\images\\xf.png");
	loadimage(&png_xxf, ".\\source\\images\\xxf.png");
	loadimage(&png_xxxf, ".\\source\\images\\xxxf.png");

	loadimage(&png_apple_, ".\\source\\images\\apple.png");		//读取苹果
	rotateimage(&png_apple, &png_apple_, 0);
	loadimage(&png_apple1_, ".\\source\\images\\apple-1.png");
	rotateimage(&show_fruit_cut[0].fruit1, &png_apple1_, 0);
	rotateimage(&show_fruit_cut[1].fruit1, &png_apple1_, 0);
	loadimage(&png_apple2_, ".\\source\\images\\apple-2.png");
	rotateimage(&show_fruit_cut[0].fruit2, &png_apple2_, 0);
	rotateimage(&show_fruit_cut[1].fruit2, &png_apple2_, 0);

	loadimage(&png_sandia_, ".\\source\\images\\sandia.png");	//读取西瓜
	rotateimage(&png_sandia, &png_sandia_, 0);
	loadimage(&png_sandia1_, ".\\source\\images\\sandia-1.png");
	rotateimage(&png_sandia1, &png_sandia1_, 0);
	rotateimage(&show_fruit_cut[2].fruit1, &png_sandia1_, 0);
	rotateimage(&show_fruit_cut[3].fruit1, &png_sandia1_, 0);
	loadimage(&png_sandia2_, ".\\source\\images\\sandia-2.png");
	rotateimage(&png_sandia2, &png_sandia2_, 0);
	rotateimage(&show_fruit_cut[2].fruit2, &png_sandia2_, 0);
	rotateimage(&show_fruit_cut[3].fruit2, &png_sandia2_, 0);

	loadimage(&png_banana_, ".\\source\\images\\banana.png");	//读取香蕉
	rotateimage(&png_banana, &png_banana_, 0);
	loadimage(&png_banana1_, ".\\source\\images\\banana-1.png");
	rotateimage(&show_fruit_cut[4].fruit1, &png_banana1_, 0);
	rotateimage(&show_fruit_cut[5].fruit1, &png_banana1_, 0);
	loadimage(&png_banana2_, ".\\source\\images\\banana-2.png");
	rotateimage(&show_fruit_cut[4].fruit2, &png_banana2_, 0);
	rotateimage(&show_fruit_cut[5].fruit2, &png_banana2_, 0);

	loadimage(&png_peach_, ".\\source\\images\\fruit\\peach.png");	//读取桃子
	rotateimage(&png_peach, &png_peach_, 0);
	loadimage(&png_peach1_, ".\\source\\images\\peach-1.png");
	rotateimage(&png_peach1, &png_peach1_, 0);
	rotateimage(&show_fruit_cut[6].fruit1, &png_peach1_, 0);
	rotateimage(&show_fruit_cut[7].fruit1, &png_peach1_, 0);
	loadimage(&png_peach2_, ".\\source\\images\\peach-2.png");
	rotateimage(&png_peach2, &png_peach2_, 0);
	rotateimage(&show_fruit_cut[6].fruit2, &png_peach2_, 0);
	rotateimage(&show_fruit_cut[7].fruit2, &png_peach2_, 0);

	loadimage(&png_strawberry_, ".\\source\\images\\fruit\\strawberry.png");	//读取草莓
	rotateimage(&png_strawberry, &png_strawberry_, 0);
	loadimage(&png_strawberry1_, ".\\source\\images\\strawberry-1.png");
	rotateimage(&show_fruit_cut[8].fruit1, &png_strawberry1_, 0);
	rotateimage(&show_fruit_cut[9].fruit1, &png_strawberry1_, 0);
	loadimage(&png_strawberry2_, ".\\source\\images\\strawberry-2.png");
	rotateimage(&show_fruit_cut[8].fruit2, &png_strawberry2_, 0);
	rotateimage(&show_fruit_cut[9].fruit2, &png_strawberry2_, 0);
	
	loadimage(&png_boom_, ".\\source\\images\\fruit\\boom.png");	//炸弹
	rotateimage(&png_boom, &png_boom_, 0);
	rotateimage(&show_boom.png_boom, &png_boom_, 0);
	loadimage(&png_newgame_, ".\\source\\images\\new-game.png");
	rotateimage(&png_newgame, &png_newgame_, 0);
	loadimage(&png_quit_, ".\\source\\images\\quit.png");
	rotateimage(&png_quit, &png_quit_, 0);
	loadimage(&png_dojo_, ".\\source\\images\\dojo.png");
	rotateimage(&png_dojo, &png_dojo_, 0);

	loadimage(&png_sandia_flash, ".\\source\\images\\sandia_flash.png");  //加载闪烁图片
	loadimage(&png_peach_flash, ".\\source\\images\\peach_flash.png");
	loadimage(&png_apple_flash, ".\\source\\images\\apple_flash.png");
	loadimage(&png_banana_flash, ".\\source\\images\\banana_flash.png");
	loadimage(&png_strawberry_flash, ".\\source\\images\\strawberry_flash.png");

	rotateimage(&show_fruit[0].png_fruit, &png_apple_, 0);     //初始化要显示的水果图片
	rotateimage(&show_fruit[1].png_fruit, &png_apple_, 0);
	rotateimage(&show_fruit[2].png_fruit, &png_sandia_, 0);
	rotateimage(&show_fruit[3].png_fruit, &png_sandia_, 0);
	rotateimage(&show_fruit[4].png_fruit, &png_banana_, 0);
	rotateimage(&show_fruit[5].png_fruit, &png_banana_, 0);
	rotateimage(&show_fruit[6].png_fruit, &png_peach_, 0);
	rotateimage(&show_fruit[7].png_fruit, &png_peach_, 0);
	rotateimage(&show_fruit[8].png_fruit, &png_strawberry_, 0);
	rotateimage(&show_fruit[9].png_fruit, &png_strawberry_, 0);

	mciSendString("open .\\source\\sound\\menu.mp3 alias bkmusic", NULL, 0, NULL);//打开背景音乐
	mciSendString("play bkmusic repeat", NULL, 0, NULL);  // 循环播放
	BeginBatchDraw();  //开始绘图
}

void show_menu() {
	putimage(0, 0, &img_bk);  //显示主界面
	putimage(0, 0, &png_homemask);
	putimage(30, 0, &png_logo);
	putimage(width / 2 + 10, 45, &png_ninja);
	putimage(50, high / 4 + 20, &png_HomeDesc);
	if (isshow_sandia == 1)
		putimage(width / 3 + 62, high / 3 + 30, &png_sandia);         //西瓜
	else {
		putimage(sandia1_x, sandia_y, &png_sandia1);
		putimage(sandia2_x, sandia_y, &png_sandia2);
	}
	if (isshow_peach == 1)
		putimage(width / 7+12, high / 2 + 50, &png_peach);                 //桃子
	else {
		putimage(peach1_x, peach_y, &png_peach1);
		putimage(peach2_x, peach_y, &png_peach2);
	}
	putimage(width / 6 * 5 - 40, high / 4 * 3 - 30, &png_boom);               //炸弹
	putimage(width / 3 - 10, high / 3 - 40, &png_newgame);               //西瓜圈
	putimage(width / 8 - 56.5, high / 2 - 28, &png_dojo);                 //禅境圈
	putimage(width / 6 * 4+15, high / 2+40, &png_quit);               //炸弹圈
	putimage(width / 3+10, high / 2 - 20, &png_new);
	
	if (flag_fruitknife == 1) {             //**********显示刀光
		setlinecolor(LIGHTBLUE);
		if (fruitknife_x[0] != -1 && fruitknife_x[1] != -1) {
			setlinestyle(PS_ENDCAP_SQUARE, 8);
			line(fruitknife_x[0], fruitknife_y[0], fruitknife_x[1], fruitknife_y[1]);
		}
		if (fruitknife_x[1] != -1 && fruitknife_x[2] != -1) {
			setlinestyle(PS_ENDCAP_SQUARE, 7);
			line(fruitknife_x[1], fruitknife_y[1], fruitknife_x[2], fruitknife_y[2]);
		}
		if (fruitknife_x[2] != -1 && fruitknife_x[3] != -1) {
			setlinestyle(PS_ENDCAP_SQUARE, 6);
			line(fruitknife_x[2], fruitknife_y[2], fruitknife_x[3], fruitknife_y[3]);
		}
	}

	FlushBatchDraw();  //绘图
	Sleep(3);
}
void updatewithoutinput_menu() {
	rotateimage(&png_sandia, &png_sandia_, rotate_fruit += PI / 3);   //旋转图片
	rotateimage(&png_peach, &png_peach_, rotate_fruit);
	rotateimage(&png_boom, &png_boom_, rotate_fruit);
	rotateimage(&png_newgame, &png_newgame_, rotate_circle -= PI / 2);
	rotateimage(&png_dojo, &png_dojo_, rotate_circle);
	rotateimage(&png_quit, &png_quit_, rotate_circle);

	if (isshow_sandia == 0) {   //西瓜被切成的两半的下落
		static double rotate_sandia = 0;         //西瓜两半的旋转速度
		static double velo_sandia_x = 1.1;
		static double velo_sandia_y = 0;
		sandia1_x -= velo_sandia_x;
		sandia2_x += velo_sandia_x;
		sandia_y += velo_sandia_y;
		velo_sandia_y += 0.13;
		rotateimage(&png_sandia1, &png_sandia1_, rotate_sandia -= PI / 3);
		rotateimage(&png_sandia2, &png_sandia2_, -rotate_sandia);
		if (sandia_y > high) {
			sandia1_x = width / 3 + 60;
			sandia_y = high / 3 + 40;
			sandia2_x = width / 3 + 100;
			rotate_sandia = 0;
			velo_sandia_x = 1.1;
			velo_sandia_y = 0;
			isshow_sandia = 1;
			flag = 1;
			mciSendString("close bkmusic", NULL, 0, NULL);                   // 关闭背景音乐
			mciSendString("close startmusic", NULL, 0, NULL);                   // 先把前面一次的音乐关闭
			mciSendString("open .\\source\\sound\\start.mp3 alias startmusic", NULL, 0, NULL); // 打开音乐
			mciSendString("play startmusic", NULL, 0, NULL);                    // 仅播放一次
			return;
		}
	}
	else if (isshow_peach == 0) {   //桃子被切成的两半的下落
		static double rotate_peach = 0;         //桃子两半的旋转速度
		static double velo_peach_x = 1.1;
		static double velo_peach_y = 0;
		peach1_x -= velo_peach_x;
		peach2_x += velo_peach_x;
		peach_y += velo_peach_y;
		velo_peach_y += 0.13;
		rotateimage(&png_peach1, &png_peach1_, rotate_peach -= PI / 3);
		rotateimage(&png_peach2, &png_peach2_, -rotate_peach);
		if (peach_y > high) {
			peach1_x = width / 7 + 12;
			peach_y = high / 2 + 60;
			peach2_x = width / 7 + 52;
			rotate_peach = 0;
			velo_peach_x = 1.1;
			velo_peach_y = 0;
			isshow_peach = 1;
			flag = 2;
			mciSendString("close bkmusic", NULL, 0, NULL);                   // 关闭背景音乐
			mciSendString("close startmusic", NULL, 0, NULL);                   // 先把前面一次的音乐关闭
			mciSendString("open .\\source\\sound\\start.mp3 alias startmusic", NULL, 0, NULL); // 打开音乐
			mciSendString("play startmusic", NULL, 0, NULL);                    // 仅播放一次
			dojo_start = time(NULL);
			return;
		}
	}
}
void updatewithinput_menu() {
	if (GetAsyncKeyState(VK_LBUTTON)&0x8000) {
		flag_fruitknife = 1;
		POINT p;
		GetCursorPos(&p);         //获取屏幕鼠标位置
		ScreenToClient(hwnd, &p); //鼠标位置转到客户区
		count_fruitknife++;
		switch (count_fruitknife) {    //更新刀光位置
		case 1:
			fruitknife_x[0] = p.x;
			fruitknife_y[0] = p.y;
			break;
		case 2:
			fruitknife_x[1] = p.x;
			fruitknife_y[1] = p.y;
			break;
		case 3:
			fruitknife_x[2] = p.x;
			fruitknife_y[2] = p.y;
			break;
		case 4:
			fruitknife_x[3] = p.x;
			fruitknife_y[3] = p.y;
			count_fruitknife = 0;
			break;
		}
		// 以下为刀切到西瓜  西瓜左上位置为 width / 3 + 62, high / 3 + 30 西瓜尺寸 98*85
		if ((p.x - width / 3 - 120)*(p.x - width / 3 - 120) + (p.y - high / 3 - 100)*(p.y - high / 3 - 100) < 1500 && isshow_sandia == 1) {
			isshow_sandia = 0;
			putimage(p.x - 115, p.y - 115, &png_sandia_flash);
			FlushBatchDraw();
			Sleep(5);
			sandia1_x = width / 3 + 60;  //西瓜左上角 width / 3 + 62, high / 3 + 30  98*85
			sandia_y = high / 3 + 40;     //西瓜两半的Y坐标相等
			sandia2_x = width / 3 + 100;
			mciSendString("close splattermusic", NULL, 0, NULL);                   // 先把前面一次的音乐关闭
			mciSendString("open .\\source\\sound\\splatter.mp3 alias splattermusic", NULL, 0, NULL); // 打开音乐
			mciSendString("play splattermusic", NULL, 0, NULL);                    // 仅播放一次
		}
		// 以下为刀切到桃子  桃子左上位置为 width / 7+12, high / 2 + 50 桃子尺寸 62*59
		else if ((p.x - width / 7 - 65)*(p.x - width / 7 - 65) + (p.y - high / 2 - 95)*(p.y - high / 2 - 95) < 900 && isshow_peach == 1) {
			isshow_peach = 0;
			putimage(p.x - 120, p.y - 85, &png_peach_flash);
			FlushBatchDraw();
			Sleep(5);
			peach1_x = width / 7 + 12;  //桃子左上角 width / 7+12, high / 2 + 50  62*59
			peach_y = high / 2+60;
			peach2_x = width / 7 + 52;
			mciSendString("close splattermusic", NULL, 0, NULL);                   // 先把前面一次的音乐关闭
			mciSendString("open .\\source\\sound\\splatter.mp3 alias splattermusic", NULL, 0, NULL); // 打开音乐
			mciSendString("play splattermusic", NULL, 0, NULL);                    // 仅播放一次
		}
		// 以下为刀切到炸弹  炸弹左上位置为 width / 6 * 5 - 40, high / 4 * 3 - 30 炸弹尺寸 66*68
		else if ((p.x - width / 6 * 5-10)*(p.x - width / 6 * 5 - 10) + (p.y - high / 4 * 3 - 10)*(p.y - high / 4 * 3 - 10) < 999) {
			exit(0);
		}
	}
	else {
		flag_fruitknife = 0;
		for (int i = 0;i < 4;i++) {
			fruitknife_x[i] = -1;
			fruitknife_y[i] = -1;
		}
	}
}

//**********************************************以下为经典模式***************************************************//
void show_gameover(int score) {                 //*******************显示游戏结束界面
	mciSendString("close overmusic", NULL, 0, NULL);                   // 播放出现水果音乐
	mciSendString("open .\\source\\sound\\over.mp3 alias overmusic", NULL, 0, NULL); // 打开音乐
	mciSendString("play overmusic", NULL, 0, NULL);                    // 仅播放一次
	int IsCut = 0;  //西瓜是否被切到
	double sandia_x = 68, sandia_y = 290;  //西瓜被切到后两半的位置
	double angle = 0;
	double accel = 0;
	rotateimage(&png_sandia1, &png_sandia1_, 0);  //初始化西瓜两半的角度
	rotateimage(&png_sandia2, &png_sandia2_, 0);
	double angle_circle = 0;
	double angle_fruit = 0;
	char s1[8];
	char s[20] = "Your score is:";
	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 45;
	_tcscpy_s(f.lfFaceName, _T("黑体"));
	f.lfQuality = ANTIALIASED_QUALITY;
	settextcolor(YELLOW);
	setbkmode(TRANSPARENT);
	sprintf_s(s1, "%d", score);
	strcat_s(s, s1);
	while (1) {
		putimage(0, 0, &img_bk);  //显示界面
		putimage(0, 220, &png_newgame);
		putimage(475, 327, &png_boom);
		putimage(425, 275, &png_quit);
		putimage(80, 80, &png_gameover);
		if (IsCut) {
			putimage(sandia_x -= 1, sandia_y += (accel += 0.1), &png_sandia1);
			putimage(150 - sandia_x, sandia_y, &png_sandia2);
			rotateimage(&png_sandia1, &png_sandia1_, angle -= 1.5);
			rotateimage(&png_sandia2, &png_sandia2_, -angle);
			if (sandia_y > high) {
				flag = 0;
				mciSendString("open .\\source\\sound\\menu.mp3 alias bkmusic", NULL, 0, NULL);//打开背景音乐
				mciSendString("play bkmusic repeat", NULL, 0, NULL);  // 循环播放
				return;
			}
		}
		else
			putimage(75, 290, &png_sandia);

		settextstyle(&f);
		outtextxy(150, 190, s);

		//********以下为显示刀光
		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
			POINT p;
			GetCursorPos(&p);         //获取屏幕鼠标位置
			ScreenToClient(hwnd, &p); //鼠标位置转到客户区
			if ((p.x - 140)*(p.x - 140) + (p.y - 355)*(p.y - 355) < 2000) {
				mciSendString("close splattermusic", NULL, 0, NULL);                   // 先把前面一次的音乐关闭
				mciSendString("open .\\source\\sound\\splatter.mp3 alias splattermusic", NULL, 0, NULL); // 打开音乐
				mciSendString("play splattermusic", NULL, 0, NULL);                    // 仅播放一次
				IsCut = 1;
			}
			else if ((p.x - 525)*(p.x - 525) + (p.y - 380)*(p.y - 380) < 1150) {
				exit(0);
			}
			count_fruitknife++;    //*******更新刀光位置
			switch (count_fruitknife) {
			case(1):
				fruitknife_x[0] = p.x;
				fruitknife_y[0] = p.y;
				break;
			case(2):
				fruitknife_x[1] = p.x;
				fruitknife_y[1] = p.y;
				break;
			case(3):
				fruitknife_x[2] = p.x;
				fruitknife_y[2] = p.y;
				break;
			case(4):
				fruitknife_x[3] = p.x;
				fruitknife_y[3] = p.y;
				count_fruitknife = 0;
				break;
			}
			setlinecolor(LIGHTBLUE);            //**********显示刀光
			if (fruitknife_x[0] != -1 && fruitknife_x[1] != -1) {
				setlinestyle(PS_ENDCAP_SQUARE, 8);
				line(fruitknife_x[0], fruitknife_y[0], fruitknife_x[1], fruitknife_y[1]);
			}
			if (fruitknife_x[1] != -1 && fruitknife_x[2] != -1) {
				setlinestyle(PS_ENDCAP_SQUARE, 8);
				line(fruitknife_x[1], fruitknife_y[1], fruitknife_x[2], fruitknife_y[2]);
			}
			if (fruitknife_x[2] != -1 && fruitknife_x[3] != -1) {
				setlinestyle(PS_ENDCAP_SQUARE, 8);
				line(fruitknife_x[2], fruitknife_y[2], fruitknife_x[3], fruitknife_y[3]);
			}
			if (fruitknife_x[3] != -1 && fruitknife_x[0] != -1) {
				setlinestyle(PS_ENDCAP_SQUARE, 8);
				line(fruitknife_x[3], fruitknife_y[3], fruitknife_x[0], fruitknife_y[0]);
			}

		}
		else {
			flag_fruitknife = 0;
			for (int i = 0;i < 4;i++) {
				fruitknife_x[i] = -1;
				fruitknife_y[i] = -1;
			}
		}
		//***************以上为显示刀光
		rotateimage(&png_sandia, &png_sandia_, angle_fruit += 1);  //更新图片
		rotateimage(&png_boom, &png_boom_, angle_fruit);
		rotateimage(&png_newgame, &png_newgame_, angle_circle -= 1);
		rotateimage(&png_quit, &png_quit_, angle_circle);
		FlushBatchDraw();
		Sleep(3);
	}
}
void show_boomexplode(double posi_x, double posi_y) {
	putimage(0, 0, &img_bk);
	int cnt = 0;
	int x = posi_x + 51, y = posi_y + 54;//x，y表示炸弹中心坐标
	long start= clock(), now = clock();
	mciSendString("close BoomExplodeMusic", NULL, 0, NULL);
	mciSendString("open .\\source\\sound\\boom.mp3 alias BoomExplodeMusic", NULL, 0, NULL);
	mciSendString("play BoomExplodeMusic", NULL, 0, NULL);
	setlinecolor(WHITE);
	int i = 0;
	while (now-start<1620) {
		if (now - start > 150&&now-start<160) {   //横向0~160
			for (i=0;i < 160;i++) {
				line(0, i, x, y);
				line(x, y, 640, 480 - i);
			}
			putimage(posi_x, posi_y, &show_boom.png_boom);
			FlushBatchDraw();
		}
		if (now - start > 300 && now - start<310) {      //纵向160~320
			for (i=160;i < 320;i++) {
				line(i,0, x, y);
				line(x, y, 640-i,480);
			}
			putimage(posi_x, posi_y, &show_boom.png_boom);
			FlushBatchDraw();
		}
		if (now - start > 450 && now - start<460) {     //横向320~480
			for (i = 320;i <= 480;i++) {
				line(0, i, x, y);
				line(x, y, 640, 480 - i);
			}
			putimage(posi_x, posi_y, &show_boom.png_boom);
			FlushBatchDraw();
		}
		if (now - start > 600 && now - start<610) {        //纵向480~640
			for (i = 480;i <= 640;i++) {
				line(i, 0, x, y);
				line(x, y, 640-i, 480);
			}
			putimage(posi_x, posi_y, &show_boom.png_boom);
			FlushBatchDraw();
		}
		if (now - start > 750 && now - start<760) {        //纵向480~640
			for (i = 0;i <= 160;i++) {
				line(i, 0, x, y);
				line(x, y, 640 - i, 480);
			}
			putimage(posi_x, posi_y, &show_boom.png_boom);
			FlushBatchDraw();
		}
		if (now - start > 900 && now - start<910) {        //横向160~320
			for (i = 160;i <= 320;i++) {
				line(0, i, x, y);
				line(x, y, 640, 480-i);
			}
			putimage(posi_x, posi_y, &show_boom.png_boom);
			FlushBatchDraw();
		}
		if (now - start > 1050 && now - start<1060) {        //纵向320~480
			for (i = 320;i <= 480;i++) {
				line(i, 0, x, y);
				line(x, y, 640 - i, 480);
			}
			putimage(posi_x, posi_y, &show_boom.png_boom);
			FlushBatchDraw();
		}
		if (now - start > 1200 && now - start<1610) {        //纵向320~480
			setfillcolor(WHITE);
			solidrectangle(0, 0, width, high);
			FlushBatchDraw();
		}
		now =clock();
	}
}
void show_classicmode() {
	putimage(0, 0, &img_bk);  //显示主界面
	putimage(8, 12, &png_score);   //**********显示分数
	if (lose) {
		if (lose == 1) {
			putimage(600, 10, &png_xxx);
			putimage(567, 11, &png_xx);
			putimage(540, 12, &png_xf);
		}
		else if (lose == 2) {
			putimage(600, 10, &png_xxx);
			putimage(567, 11, &png_xxf);
			putimage(540, 12, &png_xf);
		}
		else if (lose == 3) {
			putimage(600, 10, &png_xxxf);
			putimage(567, 11, &png_xxf);
			putimage(540, 12, &png_xf);
		}
	}
	else {
		putimage(600, 10, &png_xxx);
		putimage(567, 11, &png_xx);
		putimage(540, 12, &png_x);
	}

	fstream fp;        //*************读取最好成绩
	fp.open(".\\source\\classicmode.txt");
	fp >> BestScore_classicmode;

	char s[5];           //***************输出得分
	char s1[5];
	LOGFONT f;
	gettextstyle(&f);                     // 获取当前字体设置
	f.lfHeight = 30;                      // 设置字体高度为 30
	_tcscpy_s(f.lfFaceName, _T("黑体"));    // 设置字体为“黑体”(高版本 VC	或者VS推荐使用 _tcscpy_s 函数)
	f.lfQuality = ANTIALIASED_QUALITY;    // 设置输出效果为抗锯齿  
	setcolor(YELLOW);						 //作图色为黄色
	settextstyle(&f);                     // 设置字体样式
	sprintf_s(s, "%d", score_classicmode);
	setbkmode(TRANSPARENT);
	outtextxy(45, 13, s);
	sprintf_s(s1, "%d", BestScore_classicmode);
	f.lfHeight = 22;
	settextstyle(&f);
	outtextxy(5, 53, "best:");
	outtextxy(60, 53, s1);
	fp.close();

	for (auto &fruit : show_fruit) {    //***************显示水果图片
		if (fruit.is_show==1) {
			putimage(fruit.posi_x, fruit.posi_y, &fruit.png_fruit);
		}
	}

	if (show_boom.is_show)
		putimage(show_boom.posi_x, show_boom.posi_y, &show_boom.png_boom);
	if (flag_fruitknife == 1) {             //**********显示刀光
		setlinecolor(LIGHTBLUE);
		if (fruitknife_x[0] != -1 && fruitknife_x[1] != -1) {
			setlinestyle(PS_ENDCAP_SQUARE, 8);
			line(fruitknife_x[0], fruitknife_y[0], fruitknife_x[1], fruitknife_y[1]);
		}
		if (fruitknife_x[1] != -1 && fruitknife_x[2] != -1) {
			setlinestyle(PS_ENDCAP_SQUARE, 8);
			line(fruitknife_x[1], fruitknife_y[1], fruitknife_x[2], fruitknife_y[2]);
		}
		if (fruitknife_x[2] != -1 && fruitknife_x[3] != -1) {
			setlinestyle(PS_ENDCAP_SQUARE, 8);
			line(fruitknife_x[2], fruitknife_y[2], fruitknife_x[3], fruitknife_y[3]);
		}
		if (fruitknife_x[3] != -1 && fruitknife_x[0] != -1) {
			setlinestyle(PS_ENDCAP_SQUARE, 8);
			line(fruitknife_x[3], fruitknife_y[3], fruitknife_x[0], fruitknife_y[0]);
		}
	}
	for (int i = 0;i < 10;i++) {          //********显示闪烁
		if (show_fruit[i].is_flash) {
			if (i == 0 || i == 1)
				putimage(show_fruit[i].posi_x-30, show_fruit[i].posi_y-30, &png_apple_flash);
			else if (i == 2 || i == 3)
				putimage(show_fruit[i].posi_x-50, show_fruit[i].posi_y-35, &png_sandia_flash);
			else if (i == 4 || i == 5)
				putimage(show_fruit[i].posi_x - 20, show_fruit[i].posi_y - 20, &png_banana_flash);
			else if (i == 6 || i == 7)
				putimage(show_fruit[i].posi_x - 50, show_fruit[i].posi_y - 45, &png_peach_flash);
			else if (i == 8 || i == 9)
				putimage(show_fruit[i].posi_x - 55, show_fruit[i].posi_y - 40, &png_strawberry_flash);
			show_fruit[i].is_flash++;
			if (show_fruit[i].is_flash > 4)
				show_fruit[i].is_flash = 0;
		}
		if (show_fruit_cut[i].is_show == 1) {    //********显示被切开的水果
			putimage(show_fruit_cut[i].posi_x1, show_fruit_cut[i].posi_y, &show_fruit_cut[i].fruit1);
			putimage(show_fruit_cut[i].posi_x2, show_fruit_cut[i].posi_y, &show_fruit_cut[i].fruit2);
		}
	}
	FlushBatchDraw();
	Sleep(3);
}
void updatewithoutinput_classicmode() {
	int IsPlayMusic = 0;                //是否播放音乐
	static int cnt = 1;                 //cnt为计数器
	static unsigned int count_cnt = 1;  //count_cnt是控制计数器增加的计数器
	static int fruit_number = 0;        //出现水果的个数
	static double velo_x[4] = { 0.26,0.36,-0.26,-0.36 };  //四种初始速度
	static int direct[] = { 1,-1 };      //旋转方向
	if (count_cnt % 600 == 0 && cnt < 5)
		cnt++;
	if (count_cnt % 2000 == 0) {
		show_boom.is_show = 1;
		show_boom.direction = direct[rand() % 2];
		show_boom.velo_x = velo_x[rand() % 4];
		show_boom.velo_y = -3;
		if (show_boom.velo_x > 0)
			show_boom.posi_x = rand() % 250 + 50;
		else
			show_boom.posi_x = rand() % 250 + 320;
		show_boom.posi_y = high;

	}
	if (count_cnt % 500 == 0) {         //**********出现新的水果
		mciSendString("close throwmusic", NULL, 0, NULL);                   // 播放出现水果音乐
		mciSendString("open .\\source\\sound\\throw.mp3 alias throwmusic", NULL, 0, NULL); // 打开音乐
		mciSendString("play throwmusic", NULL, 0, NULL);                    // 仅播放一次
		fruit_number = rand() % cnt + 1;
		int i = 0;      //已初始化的水果数量
		while (1) {   //******初始化新水果的数据
			int j = rand() % 10;   //show_fruit下标
			if (show_fruit[j].is_show==-1) {
				i++;
				show_fruit[j].is_show = 1;
				show_fruit[j].direction = direct[rand() % 2];
				show_fruit[j].velo_x = velo_x[rand() % 4];
				show_fruit[j].velo_y = -2.7;
				show_fruit[j].posi_y = high+30;
				if (show_fruit[j].velo_x > 0) {
					show_fruit[j].posi_x = rand() % 300 + 35;
				}
				else {
					show_fruit[j].posi_x = rand() % 300 + 230;
				}
				if (i >= fruit_number)
					break;
			}
		}
	}
	for (int i = 0;i < 10;i++) {
		//*****************更新水果位置和旋转角度**************//
		if (show_fruit[i].velo_y > 2.1&&show_fruit[i].posi_y > high - 10) {  //水果落下
			show_fruit[i].direction = 0;
			show_fruit[i].angle = 0;
			show_fruit[i].is_show = -1;
			show_fruit[i].posi_x = 0;
			show_fruit[i].posi_y = 0;
 			lose++;
			if (lose >= 3) {
				if (score_classicmode > BestScore_classicmode) {
					fstream fp(".\\source\\classicmode.txt");
					fp << score_classicmode;
					fp.close();
				}
				lose = 0;
				for (int i = 0;i < 10;i++) {
					show_fruit[i].is_show = -1;
					show_fruit[i].posi_y = 0;
					show_fruit_cut[i].is_show = 0;
				}
				show_boom.is_show = 0;
				cnt = 1;
				show_gameover(score_classicmode);
				return;
			}
		}
		else if (show_fruit[i].is_show ==1) {
			if (i == 0 || i == 1)   //******以下为水果旋转和位置更新
				rotateimage(&show_fruit[i].png_fruit, &png_apple_, show_fruit[i].direction*(show_fruit[i].angle += 1));
			else if (i == 2 || i == 3)
				rotateimage(&show_fruit[i].png_fruit, &png_sandia_, show_fruit[i].direction*(show_fruit[i].angle += 1));
			else if (i == 4 || i == 5)
				rotateimage(&show_fruit[i].png_fruit, &png_banana_, show_fruit[i].direction*(show_fruit[i].angle += 1));
			else if (i == 6 || i == 7)
				rotateimage(&show_fruit[i].png_fruit, &png_peach_, show_fruit[i].direction*(show_fruit[i].angle += 1));
			else if (i == 8 || i == 9)
				rotateimage(&show_fruit[i].png_fruit, &png_strawberry_, show_fruit[i].direction*(show_fruit[i].angle += 1));
			show_fruit[i].posi_x += show_fruit[i].velo_x;
			show_fruit[i].posi_y += show_fruit[i].velo_y;
			show_fruit[i].velo_y += 0.01;

			if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {     //判断鼠标是否切到水果
				POINT p;
				GetCursorPos(&p);
				ScreenToClient(hwnd, &p);
				if (i == 0 || i == 1) {
					if ((show_fruit[i].posi_x + 45 - p.x)*(show_fruit[i].posi_x + 45 - p.x) + (show_fruit[i].posi_y + 61 - p.y)*(show_fruit[i].posi_y + 61 - p.y) < 961) {
						show_fruit[i].direction = 0;
						show_fruit[i].angle = 0;
						IsPlayMusic = 1;
						show_fruit[i].is_flash = 1;
						show_fruit[i].is_show = 0;
						show_fruit_cut[i].is_show = 1;
						show_fruit_cut[i].posi_x1 = show_fruit[i].posi_x - 13;
						show_fruit_cut[i].posi_x2 = show_fruit[i].posi_x + 13;
						show_fruit_cut[i].posi_y = show_fruit[i].posi_y;
						score_classicmode++;
					}
				}
				else if (i == 2 || i == 3) {
					if ((show_fruit[i].posi_x + 45 - p.x)*(show_fruit[i].posi_x + 45 - p.x) + (show_fruit[i].posi_y + 70 - p.y)*(show_fruit[i].posi_y + 70 - p.y) < 1500) {
						show_fruit[i].direction = 0;
						show_fruit[i].angle = 0;
						IsPlayMusic = 1;
						show_fruit[i].is_flash = 1;
						show_fruit[i].is_show = 0;
						show_fruit_cut[i].is_show = 1;
						show_fruit_cut[i].posi_x1 = show_fruit[i].posi_x - 20;
						show_fruit_cut[i].posi_x2 = show_fruit[i].posi_x + 20;
						show_fruit_cut[i].posi_y = show_fruit[i].posi_y;
						score_classicmode++;
					}
				}
				else if (i == 4 || i == 5) {
					if ((show_fruit[i].posi_x + 45 - p.x)*(show_fruit[i].posi_x + 45 - p.x) + (show_fruit[i].posi_y + 61 - p.y)*(show_fruit[i].posi_y + 61 - p.y) < 961) {
						show_fruit[i].direction = 0;
						show_fruit[i].angle = 0;
						IsPlayMusic = 1;
						show_fruit[i].is_flash = 1;
						show_fruit[i].is_show = 0;
						show_fruit_cut[i].is_show = 1;
						show_fruit_cut[i].posi_x1 = show_fruit[i].posi_x - 13;
						show_fruit_cut[i].posi_x2 = show_fruit[i].posi_x + 13;
						show_fruit_cut[i].posi_y = show_fruit[i].posi_y;
						score_classicmode++;
					}
				}
				else if (i == 6 || i == 7) {
					if ((show_fruit[i].posi_x + 45 - p.x)*(show_fruit[i].posi_x + 45 - p.x) + (show_fruit[i].posi_y + 61 - p.y)*(show_fruit[i].posi_y + 61 - p.y) < 961) {
						show_fruit[i].direction = 0;
						show_fruit[i].angle = 0;
						IsPlayMusic = 1;
						show_fruit[i].is_flash = 1;
						show_fruit[i].is_show = 0;
						show_fruit_cut[i].is_show = 1;
						show_fruit_cut[i].posi_x1 = show_fruit[i].posi_x - 13;
						show_fruit_cut[i].posi_x2 = show_fruit[i].posi_x + 13;
						show_fruit_cut[i].posi_y = show_fruit[i].posi_y;
						score_classicmode++;
					}
				}
				else if (i == 8 || i == 9) {
					if ((show_fruit[i].posi_x + 45 - p.x)*(show_fruit[i].posi_x + 45 - p.x) + (show_fruit[i].posi_y + 61 - p.y)*(show_fruit[i].posi_y + 61 - p.y) < 961) {
						show_fruit[i].direction = 0;
						show_fruit[i].angle = 0;
						IsPlayMusic = 1;
						show_fruit[i].is_flash = 1;
						show_fruit[i].is_show = 0;
						show_fruit_cut[i].is_show = 1;
						show_fruit_cut[i].posi_x1 = show_fruit[i].posi_x - 13;
						show_fruit_cut[i].posi_x2 = show_fruit[i].posi_x + 13;
						show_fruit_cut[i].posi_y = show_fruit[i].posi_y;
						score_classicmode++;
					}
				}
				if (show_boom.is_show) {
					if ((show_boom.posi_x + 33 - p.x)*(show_boom.posi_x + 33 - p.x) + (show_boom.posi_y + 48 - p.y)*(show_boom.posi_y + 48 - p.y) < 1089) {
						if (score_classicmode > BestScore_classicmode) {
							fstream fp(".\\source\\classicmode.txt");
							fp << score_classicmode;
							fp.close();
						}
						lose = 0;
						for (int i = 0;i < 10;i++) {
							show_fruit[i].is_show = -1;
							show_fruit[i].posi_y = 0;
							show_fruit_cut[i].is_show = 0;
						}
						show_boom.is_show = 0;
						cnt = 1;
						show_boomexplode(show_boom.posi_x, show_boom.posi_y);
						show_gameover(score_classicmode);
						score_classicmode = 0;
						return;
					}
				}
			}
		}
		//***********************以下为水果落下
		if (show_fruit_cut[i].posi_y > high - 30) {
			show_fruit_cut[i].posi_x1 = 0;
			show_fruit_cut[i].posi_x2 = 0;
			show_fruit_cut[i].posi_y = 0;
			show_fruit_cut[i].velo_y = 0;
			show_fruit_cut[i].angle = 0;
			show_fruit_cut[i].is_show = 0;
			show_fruit[i].is_show = -1;
		}
		//*****************以下为更新被切开的水果位置和旋转角度
		else if (show_fruit_cut[i].is_show == 1) {
			if (i == 0 || i == 1) {
				rotateimage(&show_fruit_cut[i].fruit1, &png_apple1_, -(show_fruit_cut[i].angle += 1));
				rotateimage(&show_fruit_cut[i].fruit2, &png_apple2_, show_fruit_cut[i].angle);
			}
			else if (i == 2 || i == 3) {
				rotateimage(&show_fruit_cut[i].fruit1, &png_sandia1_, -(show_fruit_cut[i].angle += 1));
				rotateimage(&show_fruit_cut[i].fruit2, &png_sandia2_, show_fruit_cut[i].angle);
			}
			else if (i == 4 || i == 5) {
				rotateimage(&show_fruit_cut[i].fruit1, &png_banana1_, -(show_fruit_cut[i].angle += 1));
				rotateimage(&show_fruit_cut[i].fruit2, &png_banana2_, show_fruit_cut[i].angle);
			}
			else if (i == 6 || i == 7) {
				rotateimage(&show_fruit_cut[i].fruit1, &png_peach1_, -(show_fruit_cut[i].angle += 1));
				rotateimage(&show_fruit_cut[i].fruit2, &png_peach2_, show_fruit_cut[i].angle);
			}
			else if (i == 8 || i == 9) {
				rotateimage(&show_fruit_cut[i].fruit1, &png_strawberry1_, -(show_fruit_cut[i].angle += 1));
				rotateimage(&show_fruit_cut[i].fruit2, &png_strawberry2_, show_fruit_cut[i].angle);
			}
			show_fruit_cut[i].posi_x1 -= show_fruit_cut[i].velo_x;
			show_fruit_cut[i].posi_x2 += show_fruit_cut[i].velo_x;
			show_fruit_cut[i].posi_y += show_fruit_cut[i].velo_y;
			show_fruit_cut[i].velo_y += 0.02;
		}
	}
	count_cnt++;
	if (IsPlayMusic) {
		mciSendString("close splattermusic", NULL, 0, NULL);                   // 播放出现水果音乐
		mciSendString("open .\\source\\sound\\splatter.mp3 alias splattermusic", NULL, 0, NULL); // 打开音乐
		mciSendString("play splattermusic", NULL, 0, NULL);                    // 仅播放一次
	}
	if (show_boom.is_show == 1) {
		if(show_boom.velo_y > 2 && show_boom.posi_y > high - 15) {  //炸弹落下
			show_boom.is_show = 0;
		}
		else {
			rotateimage(&show_boom.png_boom, &png_boom_, show_boom.direction*(show_boom.angle += 1));
			show_boom.posi_x += show_boom.velo_x;
			show_boom.posi_y += show_boom.velo_y;
			show_boom.velo_y += 0.012;
		}
	}
}
void updatewithinput_classicmode() {
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
		flag_fruitknife = 1;
		POINT p;
		GetCursorPos(&p);         //获取屏幕鼠标位置
		ScreenToClient(hwnd, &p); //鼠标位置转到客户区
		count_fruitknife++;
		switch (count_fruitknife) {    //更新刀光位置
		case(1):
			fruitknife_x[0] = p.x;
			fruitknife_y[0] = p.y;
			break;
		case(2):
			fruitknife_x[1] = p.x;
			fruitknife_y[1] = p.y;
			break;
		case(3):
			fruitknife_x[2] = p.x;
			fruitknife_y[2] = p.y;
			break;
		case(4):
			fruitknife_x[3] = p.x;
			fruitknife_y[3] = p.y;
			count_fruitknife = 0;
			break;
		}
	}
	else {
		flag_fruitknife = 0;
		for (int i = 0;i < 4;i++) {
			fruitknife_x[i] = -1;
			fruitknife_y[i] = -1;
		}
	}
}
//**********************************************以下为禅模式*****************************************************//
void show_dojo() {
	putimage(0, 0, &img_bk);  //显示主界面
	putimage(8, 12, &png_score);   //**********显示分数

	fstream fp;        //*************读取最好成绩
	fp.open(".\\source\\dojomode.txt");
	fp >> BestScore_dojomode;

	char s[5];           //***************输出得分
	char s1[5];
	char s2[5];
	LOGFONT f;
	gettextstyle(&f);                     // 获取当前字体设置
	f.lfHeight = 30;                      // 设置字体高度为 30
	_tcscpy_s(f.lfFaceName, _T("黑体"));    // 设置字体为“黑体”
	f.lfQuality = ANTIALIASED_QUALITY;    // 设置输出效果为抗锯齿  
	setcolor(YELLOW);						 //作图色为黄色
	settextstyle(&f);                     // 设置字体样式
	sprintf_s(s, "%d", score_dojo);
	setbkmode(TRANSPARENT);
	outtextxy(45, 13, s);
	sprintf_s(s1, "%d", BestScore_dojomode);
	f.lfHeight = 22;
	settextstyle(&f);
	outtextxy(5, 53, "best:");
	outtextxy(60, 53, s1);
	fp.close();

	time_now = 30 + dojo_start - time(NULL);   //显示时间
	sprintf_s(s2, "%d", time_now);
	f.lfHeight = 30;
	settextstyle(&f);
	outtextxy(600, 10, s2);

	for (auto &fruit : show_fruit) {    //***************显示水果图片
		if (fruit.is_show == 1) {
			putimage(fruit.posi_x, fruit.posi_y, &fruit.png_fruit);
		}
	}

	if (flag_fruitknife == 1) {             //**********显示刀光
		setlinecolor(LIGHTBLUE);
		if (fruitknife_x[0] != -1 && fruitknife_x[1] != -1) {
			setlinestyle(PS_ENDCAP_SQUARE, 8);
			line(fruitknife_x[0], fruitknife_y[0], fruitknife_x[1], fruitknife_y[1]);
		}
		if (fruitknife_x[1] != -1 && fruitknife_x[2] != -1) {
			setlinestyle(PS_ENDCAP_SQUARE, 8);
			line(fruitknife_x[1], fruitknife_y[1], fruitknife_x[2], fruitknife_y[2]);
		}
		if (fruitknife_x[2] != -1 && fruitknife_x[3] != -1) {
			setlinestyle(PS_ENDCAP_SQUARE, 8);
			line(fruitknife_x[2], fruitknife_y[2], fruitknife_x[3], fruitknife_y[3]);
		}
		if (fruitknife_x[3] != -1 && fruitknife_x[0] != -1) {
			setlinestyle(PS_ENDCAP_SQUARE, 8);
			line(fruitknife_x[3], fruitknife_y[3], fruitknife_x[0], fruitknife_y[0]);
		}
	}
	for (int i = 0;i < 10;i++) {          //********显示闪烁
		if (show_fruit[i].is_flash) {
			if (i == 0 || i == 1)
				putimage(show_fruit[i].posi_x - 30, show_fruit[i].posi_y - 30, &png_apple_flash);
			else if (i == 2 || i == 3)
				putimage(show_fruit[i].posi_x - 50, show_fruit[i].posi_y - 35, &png_sandia_flash);
			else if (i == 4 || i == 5)
				putimage(show_fruit[i].posi_x - 20, show_fruit[i].posi_y - 20, &png_banana_flash);
			else if (i == 6 || i == 7)
				putimage(show_fruit[i].posi_x - 50, show_fruit[i].posi_y - 45, &png_peach_flash);
			else if (i == 8 || i == 9)
				putimage(show_fruit[i].posi_x - 55, show_fruit[i].posi_y - 40, &png_strawberry_flash);
			show_fruit[i].is_flash++;
			if (show_fruit[i].is_flash > 4)
				show_fruit[i].is_flash = 0;
		}
		if (show_fruit_cut[i].is_show == 1) {    //********显示被切开的水果
			putimage(show_fruit_cut[i].posi_x1, show_fruit_cut[i].posi_y, &show_fruit_cut[i].fruit1);
			putimage(show_fruit_cut[i].posi_x2, show_fruit_cut[i].posi_y, &show_fruit_cut[i].fruit2);
		}
	}
	FlushBatchDraw();
	Sleep(3);
}
void updatewithoutinput_dojo() {
	static int cnt = 1;                 //cnt为计数器
	static unsigned int count_cnt = 1;  //count_cnt是控制计数器增加的计数器
	static int fruit_number = 0;        //出现水果的个数
	static double velo_x[4] = { 0.26,0.36,-0.26,-0.36 };  //四种初始速度
	static double velo_y[4] = { -3.5,-3.6,-3.8,-4 };
	static int direct[] = { 1,-1 };      //旋转方向
	if (count_cnt % 600 == 0 && cnt < 5)
		cnt++;

	if (time_now <= 0) {      //时间用尽，退出禅模式
		if (score_dojo > BestScore_dojomode) {
			fstream fp;
			fp.open(".\\source\\dojomode.txt");
			fp << score_dojo;
			fp.close();
		}
		for (int i = 0;i < 10;i++) {
			show_fruit[i].is_show = -1;
			show_fruit_cut[i].is_show = 0;
		}
		show_gameover(score_dojo);
		cnt = 1;
		score_dojo = 0;
		return;
	}
	
	if (count_cnt % 400 == 0) {         //**********出现新的水果
		mciSendString("close throwmusic", NULL, 0, NULL);                   // 播放出现水果音乐
		mciSendString("open .\\source\\sound\\throw.mp3 alias throwmusic", NULL, 0, NULL); // 打开音乐
		mciSendString("play throwmusic", NULL, 0, NULL);                    // 仅播放一次
		fruit_number = rand() % cnt + 1;
		int i = 0;      //已初始化的水果数量
		while (1) {   //******初始化新水果的数据
			int j = rand() % 10;   //show_fruit下标
			if (show_fruit[j].is_show == -1) {
				i++;
				show_fruit[j].is_show = 1;
				show_fruit[j].direction = direct[rand() % 2];
				show_fruit[j].velo_x = velo_x[rand() % 4];
				show_fruit[j].velo_y = velo_y[rand()%4];
				show_fruit[j].posi_y = high + 30;
				if (show_fruit[j].velo_x > 0) {
					show_fruit[j].posi_x = rand() % 300 + 35;
				}
				else {
					show_fruit[j].posi_x = rand() % 300 + 230;
				}
				if (i >= fruit_number)
					break;
			}
		}
	}
	for (int i = 0;i < 10;i++) {
		//*****************更新水果位置和旋转角度**************//
		if (show_fruit[i].velo_y > 2.1&&show_fruit[i].posi_y > high - 10) {  //水果落下
			show_fruit[i].direction = 0;
			show_fruit[i].angle = 0;
			show_fruit[i].is_show = -1;
			show_fruit[i].posi_x = 0;
			show_fruit[i].posi_y = 0;
		}
		else if (show_fruit[i].is_show == 1) {
			if (i == 0 || i == 1)   //******以下为水果旋转和位置更新
				rotateimage(&show_fruit[i].png_fruit, &png_apple_, show_fruit[i].direction*(show_fruit[i].angle += 1));
			else if (i == 2 || i == 3)
				rotateimage(&show_fruit[i].png_fruit, &png_sandia_, show_fruit[i].direction*(show_fruit[i].angle += 1));
			else if (i == 4 || i == 5)
				rotateimage(&show_fruit[i].png_fruit, &png_banana_, show_fruit[i].direction*(show_fruit[i].angle += 1));
			else if (i == 6 || i == 7)
				rotateimage(&show_fruit[i].png_fruit, &png_peach_, show_fruit[i].direction*(show_fruit[i].angle += 1));
			else if (i == 8 || i == 9)
				rotateimage(&show_fruit[i].png_fruit, &png_strawberry_, show_fruit[i].direction*(show_fruit[i].angle += 1));
			show_fruit[i].posi_x += show_fruit[i].velo_x;
			show_fruit[i].posi_y += show_fruit[i].velo_y;
			show_fruit[i].velo_y += 0.017;
		}
		//***********************以下为水果落下
		if (show_fruit_cut[i].posi_y > high - 30) {
			show_fruit_cut[i].posi_x1 = 0;
			show_fruit_cut[i].posi_x2 = 0;
			show_fruit_cut[i].posi_y = 0;
			show_fruit_cut[i].velo_y = 0;
			show_fruit_cut[i].angle = 0;
			show_fruit_cut[i].is_show = 0;
			show_fruit[i].is_show = -1;
		}
		//*****************以下为更新被切开的水果位置和旋转角度
		else if (show_fruit_cut[i].is_show == 1) {
			if (i == 0 || i == 1) {
				rotateimage(&show_fruit_cut[i].fruit1, &png_apple1_, -(show_fruit_cut[i].angle += 1));
				rotateimage(&show_fruit_cut[i].fruit2, &png_apple2_, show_fruit_cut[i].angle);
			}
			else if (i == 2 || i == 3) {
				rotateimage(&show_fruit_cut[i].fruit1, &png_sandia1_, -(show_fruit_cut[i].angle += 1));
				rotateimage(&show_fruit_cut[i].fruit2, &png_sandia2_, show_fruit_cut[i].angle);
			}
			else if (i == 4 || i == 5) {
				rotateimage(&show_fruit_cut[i].fruit1, &png_banana1_, -(show_fruit_cut[i].angle += 1));
				rotateimage(&show_fruit_cut[i].fruit2, &png_banana2_, show_fruit_cut[i].angle);
			}
			else if (i == 6 || i == 7) {
				rotateimage(&show_fruit_cut[i].fruit1, &png_peach1_, -(show_fruit_cut[i].angle += 1));
				rotateimage(&show_fruit_cut[i].fruit2, &png_peach2_, show_fruit_cut[i].angle);
			}
			else if (i == 8 || i == 9) {
				rotateimage(&show_fruit_cut[i].fruit1, &png_strawberry1_, -(show_fruit_cut[i].angle += 1));
				rotateimage(&show_fruit_cut[i].fruit2, &png_strawberry2_, show_fruit_cut[i].angle);
			}
			show_fruit_cut[i].posi_x1 -= show_fruit_cut[i].velo_x;
			show_fruit_cut[i].posi_x2 += show_fruit_cut[i].velo_x;
			show_fruit_cut[i].posi_y += show_fruit_cut[i].velo_y;
			show_fruit_cut[i].velo_y += 0.02;
		}
	}
	count_cnt++;
	
}
void updatewithinput_dojo() {
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
		flag_fruitknife = 1;
		POINT p;
		GetCursorPos(&p);         //获取屏幕鼠标位置
		ScreenToClient(hwnd, &p); //鼠标位置转到客户区
		count_fruitknife++;
		switch (count_fruitknife) {    //*******更新刀光位置
		case(1):
			fruitknife_x[0] = p.x;
			fruitknife_y[0] = p.y;
			break;
		case(2):
			fruitknife_x[1] = p.x;
			fruitknife_y[1] = p.y;
			break;
		case(3):
			fruitknife_x[2] = p.x;
			fruitknife_y[2] = p.y;
			break;
		case(4):
			fruitknife_x[3] = p.x;
			fruitknife_y[3] = p.y;
			count_fruitknife = 0;
			break;
		}
		int IsPlayMusic = 0;                //是否播放音乐
		for (int i = 0;i < 10;i++) {
			if ((i == 0 || i == 1)&&show_fruit[i].is_show==1) {
				if ((show_fruit[i].posi_x + 45 - p.x)*(show_fruit[i].posi_x + 45 - p.x) + (show_fruit[i].posi_y + 61 - p.y)*(show_fruit[i].posi_y + 61 - p.y) < 961) {
					show_fruit[i].direction = 0;
					show_fruit[i].angle = 0;
					IsPlayMusic = 1;
					show_fruit[i].is_flash = 1;
					show_fruit[i].is_show = 0;
					show_fruit_cut[i].is_show = 1;
					show_fruit_cut[i].posi_x1 = show_fruit[i].posi_x - 13;
					show_fruit_cut[i].posi_x2 = show_fruit[i].posi_x + 13;
					show_fruit_cut[i].posi_y = show_fruit[i].posi_y;
					score_dojo++;
				}
			}
			else if ((i == 2 || i == 3) && show_fruit[i].is_show == 1) {
				if ((show_fruit[i].posi_x + 45 - p.x)*(show_fruit[i].posi_x + 45 - p.x) + (show_fruit[i].posi_y + 70 - p.y)*(show_fruit[i].posi_y + 70 - p.y) < 1500) {
					show_fruit[i].direction = 0;
					show_fruit[i].angle = 0;
					IsPlayMusic = 1;
					show_fruit[i].is_flash = 1;
					show_fruit[i].is_show = 0;
					show_fruit_cut[i].is_show = 1;
					show_fruit_cut[i].posi_x1 = show_fruit[i].posi_x - 20;
					show_fruit_cut[i].posi_x2 = show_fruit[i].posi_x + 20;
					show_fruit_cut[i].posi_y = show_fruit[i].posi_y;
					score_dojo++;
				}
			}
			else if ((i == 4 || i == 5) && show_fruit[i].is_show == 1) {
				if ((show_fruit[i].posi_x + 45 - p.x)*(show_fruit[i].posi_x + 45 - p.x) + (show_fruit[i].posi_y + 61 - p.y)*(show_fruit[i].posi_y + 61 - p.y) < 961) {
					show_fruit[i].direction = 0;
					show_fruit[i].angle = 0;
					IsPlayMusic = 1;
					show_fruit[i].is_flash = 1;
					show_fruit[i].is_show = 0;
					show_fruit_cut[i].is_show = 1;
					show_fruit_cut[i].posi_x1 = show_fruit[i].posi_x - 13;
					show_fruit_cut[i].posi_x2 = show_fruit[i].posi_x + 13;
					show_fruit_cut[i].posi_y = show_fruit[i].posi_y;
					score_dojo++;
				}
			}
			else if ((i == 6 || i == 7) && show_fruit[i].is_show == 1) {
				if ((show_fruit[i].posi_x + 45 - p.x)*(show_fruit[i].posi_x + 45 - p.x) + (show_fruit[i].posi_y + 61 - p.y)*(show_fruit[i].posi_y + 61 - p.y) < 961) {
					show_fruit[i].direction = 0;
					show_fruit[i].angle = 0;
					IsPlayMusic = 1;
					show_fruit[i].is_flash = 1;
					show_fruit[i].is_show = 0;
					show_fruit_cut[i].is_show = 1;
					show_fruit_cut[i].posi_x1 = show_fruit[i].posi_x - 13;
					show_fruit_cut[i].posi_x2 = show_fruit[i].posi_x + 13;
					show_fruit_cut[i].posi_y = show_fruit[i].posi_y;
					score_dojo++;
				}
			}
			else if ((i == 8 || i == 9) && show_fruit[i].is_show == 1) {
				if ((show_fruit[i].posi_x + 45 - p.x)*(show_fruit[i].posi_x + 45 - p.x) + (show_fruit[i].posi_y + 61 - p.y)*(show_fruit[i].posi_y + 61 - p.y) < 961) {
					show_fruit[i].direction = 0;
					show_fruit[i].angle = 0;
					IsPlayMusic = 1;
					show_fruit[i].is_flash = 1;
					show_fruit[i].is_show = 0;
					show_fruit_cut[i].is_show = 1;
					show_fruit_cut[i].posi_x1 = show_fruit[i].posi_x - 13;
					show_fruit_cut[i].posi_x2 = show_fruit[i].posi_x + 13;
					show_fruit_cut[i].posi_y = show_fruit[i].posi_y;
					score_dojo++;
				}
			}
		}
		if (IsPlayMusic) {
			mciSendString("close splattermusic", NULL, 0, NULL);                   // 播放出现水果音乐
			mciSendString("open .\\source\\sound\\splatter.mp3 alias splattermusic", NULL, 0, NULL); // 打开音乐
			mciSendString("play splattermusic", NULL, 0, NULL);                    // 仅播放一次
		}
	}
	else {
		flag_fruitknife = 0;
		for (int i = 0;i < 4;i++) {
			fruitknife_x[i] = -1;
			fruitknife_y[i] = -1;
		}
	}
}

int main() {
	startup();
	while (1) {
		while (flag == 0) {
			show_menu();
			updatewithoutinput_menu();
			updatewithinput_menu();
		}
		while (flag == 1) {
			show_classicmode();
			updatewithoutinput_classicmode();
			updatewithinput_classicmode();
		}
		while (flag == 2) {
			show_dojo();
			updatewithoutinput_dojo();
			updatewithinput_dojo();
		}
	}
	_getch();
	EndBatchDraw();
	closegraph();
}
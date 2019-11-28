#include<windows.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include <time.h>


HINSTANCE hInst;
HBITMAP dra, bg1,bg,ban[7],brokeban,menu1,menu2,z,moveban,bg2,mon;
HDC hdc, mdc, bufdc;
HWND hWnd;
DWORD tPre, tNow;
PAINTSTRUCT ps;
TEXTMETRIC tm;

int X,Y,num2=0;
int num, x, y, direction = 1, flag = 0, dd = 0; //x y为每次人物图案的贴图坐标；direction为角色的面向；flag为左右脚的标志；dd为是否行走的标志
int a = 5;//纵向的加速度
int v = -10;//纵向的速度，2为初始值
int maxy;//y当前能达到的最大的高度

//窗口大小
int winX = 350;
int winY = 720;

int h = 300;
int score = 0;//分数
char s[100];//显示分数所用的字符串
int mode = 0;//0为开始菜单，1为正常模式

int banban = 0;//默认为脆板子没有断掉
int movebanx=3;//可移动板子的位移

int mousex, mousey;//鼠标坐标

int zqt = 0;//默认不显示竹蜻蜓
int usezqt = 0;//默认不使用竹蜻蜓
int zqtscore;

//子弹
int bullet=0,bulletx,bullety;



int monis = 0;//0为怪兽不显示，1为显示怪兽

//板子的坐标
struct point {
	int x, y;
};
point plat[7];


void MyPaint(HDC);
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void MyPaint(HDC hdc);


int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{
	MSG msg;

	MyRegisterClass(hInstance);

	//运行初始化函数
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	//游戏循环
	PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			tNow = GetTickCount();
			if (tNow - tPre >= 55)
			{
				MyPaint(hdc);
			}
		}

	}

	return msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = (WNDPROC)WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = NULL;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = "canvas";
	wcex.hIconSm = NULL;

	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	char filename[20] = "";
	HBITMAP bmp;
	hInst = hInstance;

	hWnd = CreateWindow("canvas", "Doodle Jump", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	MoveWindow(hWnd, 100, 10, winX, winY, true);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	hdc = GetDC(hWnd);
	mdc = CreateCompatibleDC(hdc);
	bufdc = CreateCompatibleDC(mdc);
	bmp = CreateCompatibleBitmap(hdc, 350, 720);

	SelectObject(mdc, bmp);
	
	mon=(HBITMAP)LoadImage(NULL, "mon.bmp", IMAGE_BITMAP, 100, 110, LR_LOADFROMFILE);

	z = (HBITMAP)LoadImage(NULL, "z.bmp", IMAGE_BITMAP, 200, 100, LR_LOADFROMFILE);
	dra = (HBITMAP)LoadImage(NULL, "4.bmp", IMAGE_BITMAP, 564, 190, LR_LOADFROMFILE);
	for (int i = 0; i < 7; i++) {
		if(i==3)
			ban[3] = (HBITMAP)LoadImage(NULL, "ban2.bmp", IMAGE_BITMAP, 80, 15, LR_LOADFROMFILE);
		else if (i == 6) {
			ban[6] = (HBITMAP)LoadImage(NULL, "moveban.bmp", IMAGE_BITMAP, 80, 15, LR_LOADFROMFILE);
		}
		else
		ban[i]= (HBITMAP)LoadImage(NULL, "ban1.bmp", IMAGE_BITMAP, 80, 15, LR_LOADFROMFILE);
	}

	brokeban = (HBITMAP)LoadImage(NULL, "ban3.bmp", IMAGE_BITMAP, 80, 60, LR_LOADFROMFILE);

	bg = (HBITMAP)LoadImage(NULL, "bg3.bmp", IMAGE_BITMAP, winX, winY, LR_LOADFROMFILE);
	bg1 = (HBITMAP)LoadImage(NULL, "bg5.bmp", IMAGE_BITMAP, winX, winY, LR_LOADFROMFILE);
	bg2 = (HBITMAP)LoadImage(NULL, "bg4.bmp", IMAGE_BITMAP, winX, winY, LR_LOADFROMFILE);

	num = 0;
	x = 180; //贴图起始X坐标
	y = 330;    //贴图起始Y坐标
	maxy = y;
	X = 50;Y = 400;

	//初始化7个板板
	for (int i2 = 0; i2 < 7; i2++)
	{
		srand((unsigned)time(NULL)*i2);
	
		plat[i2].x = rand() % 5;
		plat[i2].y = 500-80*i2;
	}
	plat[6].x = 100;

	//plat[8].x = rand() % 300;
	//plat[8].y = rand() % 500;

	plat[8].x =  3000;
	plat[8].y =  5000;

	MyPaint(hdc);

	return TRUE;
}


HPEN hPen;
void createban(HDC hdc, int bany, int n);
void createban2(HDC hdc, int bany, int n);
void fresh(HDC hdc);
void MyPaint(HDC hdc)
{
	if (mode == 3) {
		//在mdc中贴上背景图
		SelectObject(bufdc, bg2);
		BitBlt(mdc, 0, 0, winX, winY, bufdc, 0, 0, SRCCOPY);

		




		BitBlt(hdc, 0, 0, winX, winY, mdc, 0, 0, SRCCOPY);
		tPre = GetTickCount();     //记录此次绘图时间
	}
	else if (mode == 0) {//开始菜单
		//在mdc中贴上背景图
		SelectObject(bufdc, bg1);
		BitBlt(mdc, 0, 0, winX, winY, bufdc, 0, 0, SRCCOPY);

		//板板
		SelectObject(bufdc, ban[1]);
		BitBlt(mdc,  40, 440 + 95, 80, 15, bufdc, 0, 0, SRCAND);
		BitBlt(mdc, 40 , 440+ 95, 80, 15, bufdc, 0, 0, SRCPAINT);

		

		//人物的贴图
		SelectObject(bufdc, dra);
		BitBlt(mdc, X, Y, 47, 95, bufdc, num * 47, 95, SRCAND);
		BitBlt(mdc, X, Y, 47, 95, bufdc, num * 47, 0, SRCPAINT);


		//竹蜻蜓
		SelectObject(bufdc, z);
		BitBlt(mdc, X, Y-25, 50, 50, bufdc, num2*50, 50, SRCAND);
		BitBlt(mdc, X, Y-25, 50, 50, bufdc, num2*50, 0, SRCPAINT);
		num2++;
		if (num2 == 4) num2 = 0;



		BitBlt(hdc, 0, 0, winX, winY, mdc, 0, 0, SRCCOPY);
		tPre = GetTickCount();     //记录此次绘图时间
		//重力代码
		v = v + a;
		Y = Y + v;
		x = 180; //贴图起始X坐标
		y = 330;    //贴图起始Y坐标
		score = 0;




		if (Y >= 440 && Y <= 440 + 20  && v >= 0)
		{
			//说明碰到板板了
			v = -40;
			
		}
	}

	else if (mode == 1) {


		//在mdc中贴上背景图
		SelectObject(bufdc, bg);
		BitBlt(mdc, 0, 0, winX, winY, bufdc, 0, 0, SRCCOPY);


		//各种板板
		for (int i = 0; i < 7; i++) {
			if (i == 3 && banban == 1) {
				SelectObject(bufdc, brokeban);
				BitBlt(mdc, 350 / 6 * plat[i].x + 25, plat[i].y + 95, 80, 30, bufdc, 0, 30, SRCAND);
				BitBlt(mdc, 350 / 6 * plat[i].x + 25, plat[i].y + 95, 80, 30, bufdc, 0, 0, SRCPAINT);
				continue;
			}

			if (i == 6) {
				SelectObject(bufdc, ban[i]);
				BitBlt(mdc, plat[i].x + 25, plat[i].y + 95, 80, 15, bufdc, 0, 0, SRCAND);
				BitBlt(mdc, plat[i].x + 25, plat[i].y + 95, 80, 15, bufdc, 0, 0, SRCPAINT);
				continue;
			}
			SelectObject(bufdc, ban[i]);
			BitBlt(mdc, 350 / 6 * plat[i].x + 25, plat[i].y + 85, 80, 15, bufdc, 0, 0, SRCAND);
			BitBlt(mdc, 350 / 6 * plat[i].x + 25, plat[i].y + 85, 80, 15, bufdc, 0, 0, SRCPAINT);
		}

		

		//人物的贴图
		SelectObject(bufdc, dra);
		BitBlt(mdc, x, y, 47, 95, bufdc, num * 47, 95, SRCAND);
		BitBlt(mdc, x, y, 47, 95, bufdc, num * 47, 0, SRCPAINT);

		if (monis == 1) {
			//怪兽的显示
			SelectObject(bufdc, mon);
			BitBlt(mdc, 350 / 6 * plat[5].x + 25, plat[5].y + 35, 100, 55, bufdc, 0, 55, SRCAND);
			BitBlt(mdc, 350 / 6 * plat[5].x + 25, plat[5].y + 35, 100, 55, bufdc, 0, 0, SRCPAINT);
		}

		if (zqt == 1) {
			//竹蜻蜓
			SelectObject(bufdc, z);
			BitBlt(mdc, 350 / 6 * plat[8].x + 25, plat[8].y + 40, 50, 50, bufdc, 0 * 50, 50, SRCAND);
			BitBlt(mdc, 350 / 6 * plat[8].x + 25, plat[8].y + 40, 50, 50, bufdc, 0 * 50, 0, SRCPAINT);
		}
		else if (usezqt == 1) {
			SelectObject(bufdc, z);
			BitBlt(mdc, x, y - 25, 50, 50, bufdc, num2 * 50, 50, SRCAND);
			BitBlt(mdc, x, y - 25, 50, 50, bufdc, num2 * 50, 0, SRCPAINT);
			num2++;
			if (num2 == 4) num2 = 0;
		}

	

		//最后将图画显示在窗口中
		BitBlt(hdc, 0, 0, winX, winY, mdc, 0, 0, SRCCOPY);


		tPre = GetTickCount();     //记录此次绘图时间
		flag++;//表示左右脚
		if (direction == 1) {

			if (flag % 6 == 1) num = 1;
			else if (flag % 6 == 5) num = 2;
			if (dd == 0) num = 0;
		}
		else if (direction == 2) {

			if (flag % 6 == 1) num = 4;
			else if (flag % 6 == 5)num = 5;
			if (dd == 0) num = 3;
		}
		else if (direction == 3) {

			if (flag % 6 == 1) num = 6;
			else if (flag % 6 == 5) num = 7;
			if (dd == 0) num = 8;
		}
		else if (direction == 4) {

			if (flag % 6 == 1) num = 10;
			else if (flag % 6 == 5) num = 11;
			if (dd == 0) num = 9;
		}

		//重力代码
		v = v + a;
		y = y + v;


		//可移动的板板
		plat[6].x+= movebanx;
		if (plat[6].x >= 250) movebanx = -movebanx;
		if (plat[6].x <= 0) movebanx = -movebanx;


		//左右可穿越的代码
		if (x + 30 > 350)x = 0;
		if (x + 30 < 0)x = 320;

		fresh(hdc);

		//生成怪兽
		if (score % 13 == 0 && score != 0) {
			monis = 1; score += 1;
		}

		//she子弹
		if (bullet == 1) {
			hPen = CreatePen(PS_SOLID, 16, RGB(0, 0, 0));	//创新画笔.
			SelectObject(hdc, hPen);					//选入画笔.

			bullety -=15;
			MoveToEx(hdc, bulletx, bullety, NULL);
			LineTo(hdc, bulletx, bullety);
			if (bullety <= 0) { bullet = 0; }
			if (monis == 1) {
				//350 / 6 * plat[5].x + 25, plat[5].y + 35
				if (bulletx <= 350 / 6 * plat[5].x + 25+100 && bulletx >= 350 / 6 * plat[5].x + 25 && bullety <= plat[5].y + 35+25 && bullety >= plat[5].y + 35-25) {
					bullet = 0;
					monis = 0;
				}
			}
		}


		//生成竹蜻蜓
		if (score % 20 == 0 && usezqt==0) {
			zqt = 1;
			plat[8].x = plat[0].x ;
			plat[8].y = plat[0].y;
			score++;
		}

		//使用竹蜻蜓中()
		if (usezqt == 1) {
			v = -30;
			if (score >= zqtscore + 50) {
				usezqt = 0;
				zqt = 0;

			}
		}

		//死亡
		if (y >= 600) {
			Sleep(300);
			mode = 0;
			X = 50; Y = 400;
			v = -10;
			banban = 0;
			mode = 3;
			for (int i = 0; i < 7; i++)
			{
				srand((unsigned)time(NULL)*i);

				plat[i].x = rand() % 5;
				plat[i].y = 500 - 80 * i;
			}
			plat[6].x = 100;
			
		}

	}
}



void fresh(HDC hdc){



	//让纵向连接起来的代码
	if (y < 250) {
		for (int i = 0; i < 7; i++)
		{
			y = 250;
			plat[i].y = plat[i].y - v;
			plat[8].y = plat[0].y;
			if (plat[i].y > 540)
			{
				if (i == 0)zqt = 0;
				if (i == 5) monis = 0;
				score++;
				if (i == 3 ) banban=0;
				plat[i].y = -40;
				plat[i].x = rand() % 5;
			}

		}
	}
	for (int i = 0; i < 7; i++)
	{
		if (i == 3 ) {
			if (banban == 1) {
			
				continue;
			}
			else if (banban == 0) {

				createban2(hdc, plat[i].y, plat[i].x);
			}
		}
		else if (i == 6) {
			if (y >= plat[i].y - 20 && y <= plat[i].y + 20 && x >= plat[i].x && x <= plat[i].x + 80 && v >= 0)
			{
				//说明碰到板板了
				v = -40;
				y = plat[i].y;
			}
		}
		else if (i == 5) {
			if (monis==1 && y >= plat[i].y - 20 && y <= plat[i].y + 20 && x >= plat[i].x && x <= plat[i].x + 80 && v >= 0) {
				Sleep(300);
				mode = 0;
				X = 50; Y = 400;
				v = -10;
				banban = 0;
				mode = 3;
				for (int i = 0; i < 7; i++)
				{
					srand((unsigned)time(NULL)*i);

					plat[i].x = rand() % 5;
					plat[i].y = 500 - 80 * i;
				}
				plat[6].x = 100;
			}
		}
		//触发竹蜻蜓的使用
		 if ( zqt==1&&y >= plat[8].y -25 && y <= plat[8].y + 25 && x >= 350 / 6 * plat[8].x + 25 && x <= 350 / 6 * plat[8].x + 25 + 80 && v >= 0) {
			if (zqt = 1) {
				usezqt = 1;
				zqt = 0;
				zqtscore = score;

			}
		}
		createban(hdc, plat[i].y, plat[i].x);

	}

}

void createban2(HDC hdc, int bany, int n) {
	//画板子
	int banx = 350 / 6 * n;
	hPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));	//创新画笔.
	SelectObject(hdc, hPen);					//选入画笔.
	//MoveToEx(hdc, banx+25, bany + 95, NULL);
	//LineTo(hdc, banx+80+25, bany + 95);




	if (y >= bany - 25 && y <= bany + 25 && x >= banx && x <= banx + 80 && v >= 0)
	{
		//说明碰到板板了
		banban = 1;
		v = -40;
		y = bany;
	}
}

void createban(HDC hdc, int bany,int n) {
	//画板子
	int banx = 350 / 6 * n;
	hPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));	//创新画笔.
	SelectObject(hdc, hPen);					//选入画笔.
	//MoveToEx(hdc, banx+25, bany + 95, NULL);
	//LineTo(hdc, banx+80+25, bany + 95);
	

	if (y >= bany-20 && y<= bany+20 && x >= banx && x <= banx+80  &&v>=0 )
	{
		//说明碰到平台
			v = -40;
			y = bany;
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{


	switch (message)
	{

	case WM_KEYUP:
		dd = 0;
		
		break;
	case WM_LBUTTONDOWN:

		mousex = LOWORD(lParam);
		mousey = HIWORD(lParam);
		if (mode==0 && mousex >= 175 && mousex <= 328 && mousey >= 293 && mousey <= 342) {
			mode = 1;
		}
		if (mode == 3 && mousex >= 72 && mousex <= 240 && mousey >= 460 && mousey <= 523) {
			mode = 0;
		}
		break;

	case WM_KEYDOWN:
		switch (wParam) {
		case VK_UP:
			if (mode == 3) mode = 0;
			if(mode==2) mode = 1;
			if (mode == 1 && bullet==0) {
				bullet = 1;
				bulletx = x + 25;
				bullety = y - 45;
			}
			
			dd = 1;
			//direction = 2;
			//y -= 5;
			
			break;

		case VK_DOWN:
			dd = 1;
			//direction = 1;
			//y += 5;
			a = 0;
			v = 0;
			y++;
			/*v = -50;*/
			break;

		case VK_LEFT:
			if (mode == 1) {
				dd = 1;
				direction = 3;
				x -= 20;
			}
			break;

		case VK_RIGHT:
			if (mode == 1) {
				dd = 1;
				direction = 4;
				x += 20;
			}
			break;
		}
		break;

	case WM_DESTROY:
		DeleteDC(mdc);
		DeleteDC(bufdc);
		DeleteObject(dra);
		DeleteObject(bg);
		ReleaseDC(hWnd, hdc);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}



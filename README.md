# GdiDoodleJump

gdi编程实现涂鸦跳跃小游戏。

# 功能
在游戏中玩家要让你的角色在一个永无休止且不断生成的平台间不停地往上跳跃。在跳跃中要小心破碎的平台、移动的绿色平台和怪物，途中有快速上升的竹蜻蜓等道具，这是一个富有趣味的技巧性游戏。如果您的角色堕下，则游戏结束，跳得越高，分数就越高。玩家可使用键盘的“← →”来控制小人移动，使用键盘的“↑”来发射子弹以击倒怪物。

![enter image description here](https://lh3.googleusercontent.com/viz3CP9E1SxX9kulvzwn8hZiv1lmnUA4SM883sIcXx3tn4Nk9nnr-C8rU2ofHrLZtblgnPBaITI)

![enter image description here](https://lh3.googleusercontent.com/sH65BT9kKsbln7XsF8Xh9k4JZ8GZQSafXTREoCoBQhk9hB6mogUSmguY91PA57gx3LRA_Ym-dHU)

# 实现


**欢迎界面：**

1、`(HBITMAP)LoadImage(NULL, "bg3.bmp", IMAGE_BITMAP, winX, winY, LR_LOADFROMFILE);`使用该方法来加载各类所需的图片。

2、在message中来捕获鼠标左键的按下与按下时的坐标，使得玩家按下开始按钮后能开始游戏。

    case  WM_LBUTTONDOWN:
    mousex = LOWORD(lParam);
    mousey = HIWORD(lParam);
    if (mousex >= 175 && mousex <= 328 && mousey >= 293 && mousey <= 342) {
    mode = 1;
    }
    break;

3、定义一个变量名为mode，分别代表着开始画面，游戏状态，死亡画面。

![enter image description here](https://lh3.googleusercontent.com/v8rKaALe2BVaQecbDKAlqi3hrlB5d_B0Q_PTWnaqdUaNdl0PEMMxoIEzlUAihnaI0MBwaC405wE)

**数据初始化：**

1、利用双缓冲技术与掩图技术来实现不闪烁以及透明的贴图。

    SelectObject(bufdc, bg);
    
    BitBlt(mdc, 0, 0, winX, winY, bufdc, 0, 0, SRCCOPY);
    
    SelectObject(bufdc, dra);
    
    BitBlt(mdc, x, y, 47, 95, bufdc, num * 47, 95, SRCAND);
    
    BitBlt(mdc, x, y, 47, 95, bufdc, num * 47, 0, SRCPAINT);
    
    BitBlt(hdc, 0, 0, winX, winY, mdc, 0, 0, SRCCOPY);

2、 利用游戏循环机制，把循环代码放在MyPaint函数当中以解决游戏运行卡顿的问题。

3、 重力系统的实现

定义一个变量a表示重力加速度，再定义一个变量v表示角色纵向的速度。加速度a为恒定值。

    v = v + a;
    
    y = y + v;

每次循环都执行以上代码，当角色处于空中时（未与任何平台接触），角色将做加速下坠运动。

4、 平台初始化

    for (int i = 0; i < 7; i++){
    
    srand((unsigned)time(NULL)*i);
    
    plat[i].x = rand() % 5;
    
    plat[i].y = 500-80*i;
    
    }

Plat为平台所在的数组，从上到下依次随机生成每个平台的坐标。

5、与平台碰撞的实现

if

     (y >= bany-20 && y<= bany+20 && x >= banx && x <= banx+80  &&v>=0 )
    
    {
    
    //说明碰到平台
    
    v = -40;
    
    y = bany;
    
    }

5、 平台循环生成的实现

    if (y < 250) {
    
    for (int i = 0; i < 7; i++)
    
    {
    
    y = 250;
    
    plat[i].y = plat[i].y - v;
    
    if (plat[i].y > 540)
    
    {
    
    if (i == 3 ) banban=0;
    
    plat[i].y = -40;
    
    plat[i].x = rand() % 5;
    
    }
    
    }
    
    }




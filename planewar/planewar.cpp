// planewar.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include<graphics.h>
#include<conio.h>
#include <time.h>
#include<stdio.h>
#include<stdlib.h>
#include<WinBase.h>
#include<Windows.h>
#pragma comment(lib,"Winmm.lib")
#define _CRT_SECURE_NO_WARNINGS
#define Width_bk 1080
#define Height_bk 640
#define MAXSTAR 300 // 星星总数
#define winscore 100
IMAGE background;//银河背景
IMAGE startbk,butt[2][3], willsmith[2],
enemy[4][3],plane[1][3],bullet[3][2],missl[2],
sup[2],imgnum[10][2],enemybullet[2][2],boompic[2],
win[3], lose[3],yesbutt[3],nobutt[3],wannaplay[2];//图片与蒙版
ExMessage msg;
char gamemode='P';
bool game_go_on=true;//确定是否结束游戏

struct buttoninfo
{
    int x;
    int y;
    int length;
    int width;
}Button1, Button2, WinButton1, LoseButton2;

struct STAR
{
    double y;
    int x;
    double step;
    int color;
}star[MAXSTAR];

struct plane
{
    int x= Width_bk / 2;
    int y= Height_bk - 200;
    int length1=115;
    int width1=98;
    int length2 = 238;
    int width2 = 190;
    bool live=true;
    bool gothit = false;//产生闪烁效果
    int score=0;
    int blood=10;
}player;

struct Enemybull
{
    int x = 0;
    int y = 0;
    int width;
    int length;
    bool live = false;
}*bullp1,*lbullp2,*rbullp3;

struct Enemy
{
    int x;
    int y;
    int width;
    int length;
    bool live;
    int blood;//big10,middle5,small3,tiny1.
    char kind;//以B,M,S,T分别代表大，中，小,极小敌人（big,middle,small,tiny)
    bool gothit = false;
    struct Enemybull enembull, enembullleft, enembullright;
}enem[10];

struct Supply
{
    int x=0;
    int y=0;
    int width=50;
    int length=70;
    bool live;
}supply;

struct Bul_let
{
    int x;
    int y;
    bool live;
}bull[10],bullleft[5],bullright[5];
//敌机子弹


struct Missile
{
    int x;
    int y;
    int length=105;
    int width=30;
    bool live;
    int num;
}missile;

struct scorenum
{
    int x=25;
    int y=20;
}numberpic;

void startload() {
    loadimage(&startbk, "resources\\startbkground.jpg");
    loadimage(&butt[0][0], "resources\\button00.jpg");//
    loadimage(&butt[0][1], "resources\\button01.jpg");//
    loadimage(&butt[0][2], "resources\\button02.jpg");
    loadimage(&butt[1][0], "resources\\button10.jpg");//
    loadimage(&butt[1][1], "resources\\button11.jpg");//
    loadimage(&butt[1][2], "resources\\button12.jpg");
    loadimage(&willsmith[0], "resources\\willsmith0.jpg");//
    loadimage(&willsmith[1], "resources\\willsmith1.jpg");
}

void startmenu() {
    Button1.x = 700;
    Button1.y = 120;
    Button1.width = 100;
    Button1.length = 100;
    Button2.x = 700;
    Button2.y = 480;
    Button2.width = 100;
    Button2.length = 100;
    startload();
    bool Position1 = false, Position2 = false;
    mciSendString("close bkmusic", NULL, 0, NULL);
    mciSendString("open resources\\start.mp3 alias bkmusic", NULL, 0, NULL);
    mciSendString("play bkmusic repeat", NULL, 0, NULL);
    while (1) {
        BeginBatchDraw();
        putimage(0, 0, &startbk);       
        putimage(250, 150, &willsmith[1], NOTSRCERASE);
        putimage(250, 150, &willsmith[0], SRCINVERT);
        putimage(Button1.x, Button1.y, &butt[0][0], NOTSRCERASE);
        putimage(Button2.x, Button2.y, &butt[1][0], NOTSRCERASE);
        if (!Position1)
        {
            putimage(Button1.x, Button1.y, &butt[0][1], SRCINVERT);

        }
        if (!Position2)
        {
            putimage(Button2.x, Button2.y, &butt[1][1], SRCINVERT);
        }
        ExMessage msg;
        peekmessage(&msg);
        if (msg.x >= Button1.x && msg.x <= Button1.x + Button1.width &&
            msg.y >= Button1.y && msg.y <= Button1.y + Button1.length)
        {
            Position1 = true;
            if (Position1)
            {
                putimage(Button1.x, Button1.y, &butt[0][2], SRCINVERT);
            }
            if (msg.message == WM_LBUTTONDOWN)
            {
                gamemode = 'P';
                break;
            }
        }
        else if (msg.x < Button1.x || msg.x > Button1.x + Button1.width ||
            msg.y < Button1.y || msg.y > Button1.y + Button1.length)
        {
            Position1 = false;
        }
        if (msg.x >= Button2.x && msg.x <= Button2.x + Button2.width &&
            msg.y >= Button2.y && msg.y <= Button2.y + Button2.length)
        {
            Position2 = true;
            if (Position2)
            {
                putimage(Button2.x, Button2.y, &butt[1][2], SRCINVERT);
            }
            if (msg.message == WM_LBUTTONDOWN)
            {
                gamemode = 'D';
                break;
            }
        }
        else if (msg.x < Button2.x || msg.x > Button2.x + Button2.width ||
            msg.y < Button2.y || msg.y > Button2.y + Button2.length)
        {
            Position2 = false;
        }
        FlushBatchDraw();
    }
    EndBatchDraw();
}

//void StartMenu(char gamemode);

void ImageLoad(char gamemode)
{
    if(gamemode=='P')
    {
        loadimage(&background, "resources\\universebackground.jpg");//加载背景图片
        loadimage(&plane[0][0], "resources\\plane00.jpg",115,98,true);//加载战机0蒙版。。注：这里因第一次所用图片顺序有误修改后可能文件名和数组不是一一对应
        loadimage(&plane[0][1], "resources\\plane01.jpg", 115, 98, true);//原图
        loadimage(&plane[0][2], "resources\\plane02.jpg", 115, 98, true);//闪烁
        loadimage(&enemy[0][0], "resources\\enemy00.jpg",111,50,true);//加载敌人1
        loadimage(&enemy[0][1], "resources\\enemy01.jpg", 111, 50, true);
        loadimage(&enemy[0][2], "resources\\enemy02.jpg", 111, 50, true);
        loadimage(&enemy[1][0], "resources\\enemy10.jpg", 113, 97, true);//加载敌人2
        loadimage(&enemy[1][1], "resources\\enemy11.jpg",113,97,true);
        loadimage(&enemy[1][2], "resources\\enemy12.jpg", 113, 97, true);
        loadimage(&enemy[2][0], "resources\\enemy20.jpg", 128, 108, true);//加载敌人3
        loadimage(&enemy[2][1], "resources\\enemy21.jpg",128,108,true);
        loadimage(&enemy[2][2], "resources\\enemy22.jpg", 128, 108, true);
        loadimage(&enemy[3][0], "resources\\enemy30.jpg", 168, 154, true);//加载敌人4
        loadimage(&enemy[3][1], "resources\\enemy31.jpg", 168, 154, true);
        loadimage(&enemy[3][2], "resources\\enemy32.jpg", 168, 154, true);
        loadimage(&bullet[0][0], "resources\\bullet00.jpg",10,32,true);//普通子弹
        loadimage(&bullet[0][1], "resources\\bullet01.jpg", 10, 32, true);
        loadimage(&bullet[1][0], "resources\\bullet10.jpg",26,40,true);//左飞子弹
        loadimage(&bullet[1][1], "resources\\bullet11.jpg", 26, 40, true);
        loadimage(&bullet[2][0], "resources\\bullet20.jpg", 26, 40, true);//右飞子弹
        loadimage(&bullet[2][1], "resources\\bullet21.jpg", 26, 40, true);
        loadimage(&missl[0], "resources\\missl0.jpg",30,105,true);//导弹
        loadimage(&missl[1], "resources\\missl1.jpg", 30, 105, true);
        loadimage(&sup[0], "resources\\supply2.jpg",50,70,true);//补给
        loadimage(&sup[1], "resources\\supply1.jpg", 50, 70, true);
        loadimage(&enemybullet[0][0], "resources\\enembull00.jpg",40,40,true);//敌人子弹
        loadimage(&enemybullet[0][1], "resources\\enembull01.jpg", 40, 40, true);
        loadimage(&enemybullet[1][0], "resources\\enembull10.jpg",35,35,true);//敌人左右子弹
        loadimage(&enemybullet[1][1], "resources\\enembull11.jpg", 35, 35, true);
        loadimage(&boompic[0], "resources\\boom0.jpg");
        loadimage(&boompic[1], "resources\\boom1.jpg");
    }
    else if(gamemode=='D')
    {
        loadimage(&background, "resources\\sky.jpg");//加载背景图片
        loadimage(&plane[0][0], "resources\\dingzhen0.jpg",110,115,true);//加载战机0蒙版
        loadimage(&plane[0][1], "resources\\dingzhen1.jpg", 110, 115, true);//闪烁
        loadimage(&plane[0][2], "resources\\dingzhen2.jpg", 110, 115, true);//原图
        loadimage(&enemy[0][0], "resources\\watermelon0.jpg",50,89,true);//加载敌人1
        loadimage(&enemy[0][1], "resources\\watermelon1.jpg",50,89,true);
        loadimage(&enemy[0][2], "resources\\watermelon2.jpg",50,89,true);
        loadimage(&enemy[1][0], "resources\\gege0.jpg",100,85,true);//加载敌人2
        loadimage(&enemy[1][1], "resources\\gege1.jpg", 100, 85, true);
        loadimage(&enemy[1][2], "resources\\gege2.jpg", 100, 85, true);
        loadimage(&enemy[2][0], "resources\\homo0.jpg",78,78,true);//加载敌人3
        loadimage(&enemy[2][1], "resources\\homo1.jpg", 78, 78, true);
        loadimage(&enemy[2][2], "resources\\homo2.jpg", 78, 78, true);
        loadimage(&enemy[3][0], "resources\\sxc0.jpg",100,100,true);//加载敌人4
        loadimage(&enemy[3][1], "resources\\sxc1.jpg", 100, 100, true);
        loadimage(&enemy[3][2], "resources\\sxc2.jpg", 100, 100, true);
        loadimage(&bullet[0][0], "resources\\cigr01.jpg",7,13,true);//普通子弹
        loadimage(&bullet[0][1], "resources\\cigr00.jpg", 7, 13, true);
        loadimage(&bullet[1][0], "resources\\bullet10.jpg",26,40,true);//左飞子弹
        loadimage(&bullet[1][1], "resources\\bullet11.jpg", 26, 40, true);
        loadimage(&bullet[2][0], "resources\\bullet20.jpg", 26, 40, true);//右飞子弹
        loadimage(&bullet[2][1], "resources\\bullet21.jpg", 26, 40, true);
        loadimage(&missl[0], "resources\\ruike00.jpg",8,25,true);//导弹
        loadimage(&missl[1], "resources\\ruike01.jpg", 8, 25, true);
        loadimage(&sup[0], "resources\\supply2.jpg",50,70,true);//补给
        loadimage(&sup[1], "resources\\supply1.jpg", 50, 70, true);
        loadimage(&enemybullet[0][0], "resources\\enembull00.jpg",40, 40,true);//敌人子弹
        loadimage(&enemybullet[0][1], "resources\\enembull01.jpg", 40, 40,true);
        loadimage(&enemybullet[1][0], "resources\\sunxiaochuan0.jpg", 35, 35, true);//敌人左右子弹
        loadimage(&enemybullet[1][1], "resources\\sunxiaochuan1.jpg", 35, 35, true);
    }
}

// 初始化星星
void InitStar(int i)
{
    star[i].y = 0;
    star[i].x = rand() % Width_bk;
    star[i].step = (rand() % 5000) / 1000.0 + 1;//step即为speed
    star[i].color = (int)(star[i].step * 255 / 6.0 + 0.5); // 速度越快，颜色越亮
    star[i].color = RGB(star[i].color, star[i].color, star[i].color);
}

// 移动星星
void MoveStar(int i)
{
    // 擦掉原来的星星
    putpixel((int)star[i].x, star[i].y, 0);

    // 计算新位置
    star[i].y += star[i].step;
    if (star[i].y > Width_bk) InitStar(i);

    // 画新星星
    putpixel((int)star[i].x, star[i].y, star[i].color);
}

void starinit()
{
    // 初始化所有星星
    for (int i = 0; i < MAXSTAR; i++)
    {
        InitStar(i);
        star[i].y = rand() % Width_bk;
    }//绘制星星
}

void Starmake()
{
    for (int i = 0; i < MAXSTAR; i++)
        MoveStar(i);
    Sleep(20);
}

void EnemyKind(int i,char gamemode)
{
    if(gamemode=='P')
    {
        if (rand() % 20 == 0)//看返回0-19(20个数)间的随机数是多少（比例为1:3:5:11)
        {
            enem[i].kind = 'B';
            enem[i].blood = 50;
            enem[i].width = 168;
            enem[i].length = 154;
        }
        else if ((rand() % 20 >= 1) && (rand() % 20 <= 3))
        {
            enem[i].kind = 'M';
            enem[i].blood = 30;
            enem[i].width = 128;
            enem[i].length = 108;
        }
        else if ((rand() % 20 >= 4) && (rand() % 20 <= 8))
        {
            enem[i].kind = 'S';
            enem[i].blood = 10;
            enem[i].width = 113;
            enem[i].length = 97;
        }
        else
        {
            enem[i].kind = 'T';
            enem[i].blood = 10;
            enem[i].width = 111;
            enem[i].length = 50;
        }
    }
    else if (gamemode == 'D')
    {
        if (rand() % 20 == 0)//看返回0-19(20个数)间的随机数是多少（比例为1:3:5:11)
        {
            enem[i].kind = 'B';
            enem[i].blood = 50;
            enem[i].width = 100;
            enem[i].length = 100;
        }
        else if ((rand() % 20 >= 1) && (rand() % 20 <= 3))
        {
            enem[i].kind = 'M';
            enem[i].blood = 30;
            enem[i].width = 50;
            enem[i].length = 89;
        }
        else if ((rand() % 20 >= 4) && (rand() % 20 <= 8))
        {
            enem[i].kind = 'S';
            enem[i].blood = 10;
            enem[i].width = 78;
            enem[i].length = 78;
        }
        else
        {
            enem[i].kind = 'T';
            enem[i].blood = 10;
            enem[i].width = 100;
            enem[i].length = 85;
        }
    }
}

void GameInitialize(char gamemode)
{
    if(gamemode=='P')
    {
        mciSendString("close bkmusic", NULL, 0, NULL);
        mciSendString("open resources\\bgm1.mp3 alias bkmusic", NULL, 0, NULL);
        mciSendString("play bkmusic repeat", NULL, 0, NULL);
    }
    else if (gamemode == 'D')
    {
        mciSendString("close bkmusic", NULL, 0, NULL);
        mciSendString("open resources\\igs.mp3 alias bkmusic", NULL, 0, NULL);
        mciSendString("play bkmusic repeat", NULL, 0, NULL);
    }
        for (int i = 0; i < 10; i++)
        {
            bull[i].x = 0;
            bull[i].y = 0;
            bull[i].live = false;
        }
        for (int i = 0; i < 5; i++)
        {
            bullleft[i].x = 0;
            bullleft[i].y = 0;
            bullleft[i].live = false;
            bullright[i].x = 0;
            bullright[i].y = 0;
            bullright[i].live = false;
        }
        for (int i = 0; i < 10; i++)
        {
            enem[i].live = false;
            EnemyKind(i,gamemode);
        }
        player.blood = 10;
        player.score = 0;
        player.x = Width_bk / 2;
        player.y = Height_bk - 200;
        missile.x = 0;
        missile.y = 0;
        missile.live = false;
        missile.num = 10;
        supply.x = 0;
        supply.y = 0;
        supply.live = false;
}

void Drawpic()
{
    putimage(0, 0, &background);//显示背景图片
    putimage(player.x,player.y, &plane[0][0], NOTSRCERASE);//显示飞机图片
    putimage(player.x, player.y, player.gothit ? &plane[0][2] : &plane[0][1], SRCINVERT);//显示蒙版图片
    for (int i = 0; i < 10; i++)
    {
        if (bull[i].live)//确认应当生成子弹再放图
        {
            putimage(bull[i].x , bull[i].y, &bullet[0][0], NOTSRCERASE);//显示子弹图片
            putimage(bull[i].x , bull[i].y, &bullet[0][1], SRCINVERT);//显示蒙版图片
        }
    }
    for (int i = 0; i < 5; i++)
    {
        if (bullleft[i].live) //确认应当生成左右子弹再放图
        {
            putimage(bullleft[i].x, bullleft[i].y, &bullet[1][0], NOTSRCERASE);//显示左子弹图片
            putimage(bullleft[i].x, bullleft[i].y, &bullet[1][1], SRCINVERT);//显示蒙版图片
        }
        if (bullright[i].live)
        {
            putimage(bullright[i].x, bullright[i].y, &bullet[2][0], NOTSRCERASE);//显示右子弹图片
            putimage(bullright[i].x, bullright[i].y, &bullet[2][1], SRCINVERT);//显示蒙版图片
        }
    }
    for (int j = 0; j < 10; j++)
    {
        if (enem[j].live)//确认应当生成敌机再放图
        {
            if (enem[j].kind == 'B')
            {
                putimage(enem[j].x, enem[j].y, &enemy[3][0], NOTSRCERASE);//big
                putimage(enem[j].x, enem[j].y, enem[j].gothit ? &enemy[3][2] : &enemy[3][1], SRCINVERT);
           }
            else if (enem[j].kind == 'M')
            {
                putimage(enem[j].x, enem[j].y, & enemy[2][0], NOTSRCERASE);//middle
                putimage(enem[j].x, enem[j].y, enem[j].gothit ? &enemy[2][2] : &enemy[2][1], SRCINVERT);
            }
            else if (enem[j].kind == 'S')
            {
                putimage(enem[j].x, enem[j].y,  &enemy[1][0], NOTSRCERASE);//small
                putimage(enem[j].x, enem[j].y, enem[j].gothit ? &enemy[1][2] : &enemy[1][1], SRCINVERT);
            }
            else
            {
                putimage(enem[j].x, enem[j].y, &enemy[0][0], NOTSRCERASE);//tiny
                putimage(enem[j].x, enem[j].y, enem[j].gothit ? &enemy[0][2] : &enemy[0][1], SRCINVERT);
            }
        }     
    }
    for (int j = 0; j < 10; j++)
    {
        if (enem[j].enembull.live) //确认应当生成敌机左右子弹再放图
        {
            putimage(enem[j].enembull.x, enem[j].enembull.y, &enemybullet[0][0], NOTSRCERASE);//显示敌机子弹图片
            putimage(enem[j].enembull.x, enem[j].enembull.y, &enemybullet[0][1], SRCINVERT);//显示蒙版图片
        }
    }
    for (int j = 0; j < 10; j++)
    {
        if (enem[j].enembullleft.live) //确认应当生成敌机左右子弹再放图
        {
            putimage(enem[j].enembullleft.x, enem[j].enembullleft.y, &enemybullet[1][0], NOTSRCERASE);//显示左子弹图片
            putimage(enem[j].enembullleft.x, enem[j].enembullleft.y, &enemybullet[1][1], SRCINVERT);//显示蒙版图片
        }
    }
    for (int j = 0; j < 10; j++)
    {
        if (enem[j].enembullright.live)
        {
            putimage(enem[j].enembullright.x, enem[j].enembullright.y, &enemybullet[1][0], NOTSRCERASE);//显示右子弹图片
            putimage(enem[j].enembullright.x, enem[j].enembullright.y, &enemybullet[1][1], SRCINVERT);//显示蒙版图片
        }
    }
    if (missile.live)//确认应当生成导弹再放图
    {
        putimage(missile.x, missile.y, &missl[0], NOTSRCERASE);//显示导弹图片
        putimage(missile.x, missile.y, &missl[1], SRCINVERT);//显示蒙版图片
    }
    if (supply.live)//确认应当生成导弹再放图
    {
        putimage(supply.x, supply.y, &sup[0], NOTSRCERASE);//显示导弹图片
        putimage(supply.x, supply.y, &sup[1], SRCINVERT);//显示蒙版图片
    }
}

void BullCreat(char gamemode)
{
    for (int i = 0; i < 10; i++)
    {
        if (!bull[i].live)//如果本来就没有子弹生成再进行子弹的生成
        {
            bull[i].x = player.x+player.width1/2;
            bull[i].y = player.y;
            bull[i].live = true;
            if(gamemode=='P')
            {
                mciSendString("close bull1", NULL, 0, NULL);
                mciSendString("open resources\\sound\\bullet1.mp3 alias bull1", NULL, 0, NULL);
                mciSendString("play bull1", NULL, 0, NULL);
            }
            else if(gamemode=='D')
            {
                mciSendString("close bull1", NULL, 0, NULL);
                mciSendString("open resources\\sound\\zanghua.mp3 alias bull1", NULL, 0, NULL);
                mciSendString("play bull1", NULL, 0, NULL);
            }
            break;
        }
    }
    for (int i = 0; i < 5; i++)
    {
        if ((!bullleft[i].live) && (!bullright[i].live) && (player.score >= 20))//
            //如果本来就没有子弹生成再进行子弹的生成,且射左右子弹时分数需要大于20
        {
            bullright[i].x = player.x + player.width1 / 2;
            bullright[i].y = player.y;
            bullright[i].live = true;
            bullleft[i].x = player.x + player.width1 / 2;
            bullleft[i].y = player.y;
            bullleft[i].live = true;
            mciSendString("close bull2", NULL, 0, NULL);
            mciSendString("open resources\\sound\\bullet2.mp3 alias bull2", NULL, 0, NULL);
            mciSendString("play bull2", NULL, 0, NULL);
            break;
        }
    }
}

void BullMove()
{
    for (int i = 0; i < 10; i++)
    {
        if (bull[i].live)//子弹应当移动时再移动
        {
            bull[i].y -=10;
            if (bull[i].y < 0)
                bull[i].live = false;
        }
    }
    for (int i = 0; i < 5; i++)
    {
        if (bullleft[i].live)//子弹应当移动时再移动
        {
            bullleft[i].y -= 10;
            bullleft[i].x-= 10;
            if (bullleft[i].y < 0||bullleft[i].x<0)
                bullleft[i].live = false;
        }
        if (bullright[i].live)//子弹应当移动时再移动
        {
            bullright[i].y -= 10;
            bullright[i].x += 10;
            if (bullright[i].y < 0||bullright[i].x>Width_bk)
                bullright[i].live = false;
        }
    }
}

void EnemyBullet(char kind, struct Enemybull *p1, struct Enemybull* p2, struct Enemybull* p3
    ,int x,int y,int width,int length)
{//用指针对敌机子弹进行处理
        if (kind == 'B' || kind == 'M')
        {
            if (!p1->live)//如果本来就没有子弹生成再进行子弹的生成
            {
                p1->x = x + width / 2;
                p1->y = y + length;
                p1->live = true;
                p1->length = 40;
                p1->width = 40;
            }
        }
        if (kind == 'B')
        {
            if (!p2->live)//如果本来就没有子弹生成再进行子弹的生成
            {
                p2->x = x + width / 2;
                p2->y = y + length;
                p2->live = true;
                p2->length = 35;
                p2->width = 35;
            }
            if (!p3->live)//如果本来就没有子弹生成再进行子弹的生成
            {
                p3->x = x + width / 2;
                p3->y = y + length;
                p3->live = true;
                p3->length = 35;
                p3->width = 35;
            }
        }
}

void EnemyBulletMove()
{
    for (int i = 0; i < 10; i++)
    {
        if (enem[i].enembull.live)//子弹应当移动时再移动
        {
            enem[i].enembull.y += 8;
            if (enem[i].enembull.y > Height_bk)
                enem[i].enembull.live = false;
        }
        if (enem[i].enembullleft.live)//子弹应当移动时再移动
        {
            enem[i].enembullleft.y += 8;
            enem[i].enembullleft.x -= 8;
            if (enem[i].enembullleft.y > Height_bk|| enem[i].enembullleft.x<0)
                enem[i].enembullleft.live = false;
        }
        if (enem[i].enembullright.live)//子弹应当移动时再移动
        {
            enem[i].enembullright.y += 8;
            enem[i].enembullright.x += 8;
            if (enem[i].enembullright.y > Height_bk|| enem[i].enembullright.x>Width_bk)
                enem[i].enembullright.live = false;
        }
    }
}

void EnemyCreat()
{
    for(int i=0;i<10;i++)
    {
        if (!enem[i].live)
        {
            enem[i].live = true;
            enem[i].x = rand() % 900;//因最大飞机宽度小于180，不至于出界
            enem[i].y = 0;
            enem[i].gothit = false;
            bullp1 = &enem[i].enembull;
            lbullp2 = &enem[i].enembullleft;
            rbullp3 = &enem[i].enembullright;
            EnemyKind(i,gamemode);//实现飞机无限生成
            EnemyBullet(enem[i].kind,bullp1,lbullp2,rbullp3,
                enem[i].x,enem[i].y, enem[i].width, enem[i].length);
            break;
        }
    }
}

void EnemyMove(int speed)//不同飞机速度不同
{
    for (int i = 0; i < 10; i++)
    {
        if (enem[i].live)//飞机应当移动时再移动
        {
            enem[i].y += speed;
            if (enem[i].y > Height_bk)
                enem[i].live = false;
        }
    }
}

void SupplyUse(int speed)
{
    if (rand() % 600 == 0)
    {
        if(!supply.live)
        {
            supply.live = true;
            supply.x = rand() % 900;//因最大飞机宽度小于180，不至于出界
            supply.y = 0;
        }    
    }
    if (supply.live)//补给应当移动时再移动
    {
        supply.y += speed;
        if (supply.y > 640)
            supply.live = false;
    }
    if (!supply.live);
    else if ((player.x > supply.x && player.x  <supply.x + supply.width
        && player.y >supply.y && player.y < supply.y + supply.length) ||
        (player.x + player.width1 < supply.x + supply.width && player.x + player.width1 >supply.x
            && player.y + player.length1 > supply.y && player.y + player.length1 < supply.y + supply.length))
    {
        missile.num+=5;
        player.blood++;
        supply.live = false;
    }
}

void MissileUse(char gamemode)
{
    static int status=0;
    if(missile.num>0)//保证剩余有导弹
    {
        if(peekmessage(&msg))
        {
            bool destroy=false;
            if (msg.message == WM_LBUTTONDOWN)
            {
                if (msg.x >= player.x && msg.x <= player.x + player.width1
                    && msg.y >= player.y && msg.y <= player.y + player.length1 && missile.live == false)
                {
                    missile.live = true;//放置导弹
                    status = 1;       
                }              
            }
            else if (msg.message == WM_MOUSEMOVE && status == 1)
            {
                missile.x = msg.x;
                missile.y = msg.y;
            }
            else if (missile.live == true &&
                (msg.message == WM_LBUTTONUP || msg.x < 0 || msg.x>1080 || msg.y < 0 || msg.x>640))
                //导弹不伤害敌人而自爆。。条件：鼠标松开或者时间到或者离开窗口 
            {
                missile.live = false;//导弹自毁
                missile.num--;
                if (destroy)
                {
                    putimage(missile.x + missile.width / 4.0, missile.y + missile.length / 4.0, &boompic[0], NOTSRCERASE);
                    putimage(missile.x + missile.width / 4.0, missile.y + missile.length / 4.0, &boompic[1], SRCINVERT);
                    if (gamemode == 'P')
                    {
                        mciSendString("close bomb", NULL, 0, NULL);
                        mciSendString("open resources\\sound\\bomb.mp3 alias bomb", NULL, 0, NULL);
                        mciSendString("play bomb", NULL, 0, NULL);
                    }
                    else if (gamemode == 'D')
                    {
                        mciSendString("close bomb", NULL, 0, NULL);
                        mciSendString("open resources\\sound\\xuebao.mp3 alias bomb", NULL, 0, NULL);
                        mciSendString("play bomb", NULL, 0, NULL);
                    }
                }
                destroy = false;
            }
        }
    }
}

void PlayerMove(int speed,char gamemode)//飞机移动
{
    if (GetAsyncKeyState(VK_UP) || GetAsyncKeyState('W'))
    {
        if (player.y > 0)
            player.y -= speed;
    }
    if (GetAsyncKeyState(VK_DOWN)|| GetAsyncKeyState('S'))
    {
        if (player.y < Height_bk - player.length1)
            player.y += speed;
    }
    if (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState('A'))
    {
        if (player.x > 0)
            player.x -= speed;
    }
    if (GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState('D'))
    {
        if (player.x <Width_bk- player.width1)
        player.x += speed;
    }
    if (GetAsyncKeyState(VK_SPACE))
        BullCreat(gamemode);
}

void Hit(char gamemode)
{
    bool crush = false;
    for (int i = 0; i < 10; i++)
    {
        if (enem[i].gothit)
            enem[i].gothit = false;
        if (!enem[i].live)
            continue;
        for (int j = 0; j < 10; j++)
        {
            if (!bull[j].live)
                continue;
            if (bull[j].x > enem[i].x && bull[j].x <enem[i].x + enem[i].width
                && bull[j].y>enem[i].y && bull[j].y < enem[i].y + enem[i].length)
            {
                bull[j].live = false;
                enem[i].blood--;
                if (enem[i].kind == 'B')
                {
                    if(gamemode=='P')
                    {
                        mciSendString("close bigdown", NULL, 0, NULL);
                        mciSendString("open resources\\sound\\bigPlaneDown.mp3 alias bigdown", NULL, 0, NULL);
                        mciSendString("play bigdown", NULL, 0, NULL);
                    }
                    else if (gamemode == 'D')
                    {
                        mciSendString("close bigdown", NULL, 0, NULL);
                        mciSendString("open resources\\sound\\nizaima.mp3 alias bigdown", NULL, 0, NULL);
                        mciSendString("play bigdown", NULL, 0, NULL);
                    }
                }
                else if (enem[i].kind == 'M')
                {
                    if(gamemode=='P')
                    {
                        mciSendString("close middown", NULL, 0, NULL);
                        mciSendString("open resources\\sound\\middlePlaneDown.mp3 alias middown", NULL, 0, NULL);
                        mciSendString("play middown", NULL, 0, NULL);
                    }
                    else if (gamemode == 'D')
                    {
                        mciSendString("close middown", NULL, 0, NULL);
                        mciSendString("open resources\\sound\\homo.mp3 alias middown", NULL, 0, NULL);
                        mciSendString("play middown", NULL, 0, NULL);
                    }
                }
                else if (enem[i].kind == 'S')
                {
                    if(gamemode=='P')
                    {
                        mciSendString("play smalldown", NULL, 0, NULL);
                        mciSendString("open resources\\sound\\smallPlaneDown.mp3 alias smalldown", NULL, 0, NULL);
                        mciSendString("play smalldown", NULL, 0, NULL);
                    }
                    else if (gamemode == 'D')
                    {
                        mciSendString("close middown", NULL, 0, NULL);
                        mciSendString("open resources\\sound\\gege.mp3 alias middown", NULL, 0, NULL);
                        mciSendString("play middown", NULL, 0, NULL);
                    }
                }
                else if (enem[i].kind == 'T')
                {
                    if(gamemode=='P')
                    {
                        mciSendString("close tinydown", NULL, 0, NULL);
                        mciSendString("open resources\\sound\\tinyPlaneDown.mp3 alias middown", NULL, 0, NULL);
                        mciSendString("play tinydown", NULL, 0, NULL);
                    }
                    else if (gamemode == 'D')
                    {
                        mciSendString("close middown", NULL, 0, NULL);
                        mciSendString("open resources\\sound\\watermelon.mp3 alias middown", NULL, 0, NULL);
                        mciSendString("play middown", NULL, 0, NULL);
                    }
                }
                enem[i].gothit = true;
            }
        }
        for (int j = 0; j < 5; j++)
        {
            if (!bullleft[j].live)
                continue;
            if (bullleft[j].x > enem[i].x && bullleft[j].x <enem[i].x + enem[i].width
                && bullleft[j].y>enem[i].y && bullleft[j].y < enem[i].y + enem[i].length)
            {
                bullleft[j].live = false;
                enem[i].blood--;
                if (enem[i].kind == 'B')
                {
                    if (gamemode == 'P')
                    {
                        mciSendString("close bigdown", NULL, 0, NULL);
                        mciSendString("open resources\\sound\\bigPlaneDown.mp3 alias bigdown", NULL, 0, NULL);
                        mciSendString("play bigdown", NULL, 0, NULL);
                    }
                    else if (gamemode == 'D')
                    {
                        mciSendString("close bigdown", NULL, 0, NULL);
                        mciSendString("open resources\\sound\\nizaima.mp3 alias bigdown", NULL, 0, NULL);
                        mciSendString("play bigdown", NULL, 0, NULL);
                    }
                }
                else if (enem[i].kind == 'M')
                {
                    if (gamemode == 'P')
                    {
                        mciSendString("close middown", NULL, 0, NULL);
                        mciSendString("open resources\\sound\\middlePlaneDown.mp3 alias middown", NULL, 0, NULL);
                        mciSendString("play middown", NULL, 0, NULL);
                    }
                    else if (gamemode == 'D')
                    {
                        mciSendString("close middown", NULL, 0, NULL);
                        mciSendString("open resources\\sound\\homo.mp3 alias middown", NULL, 0, NULL);
                        mciSendString("play middown", NULL, 0, NULL);
                    }
                }
                else if (enem[i].kind == 'S')
                {
                    if (gamemode == 'P')
                    {
                        mciSendString("play smalldown", NULL, 0, NULL);
                        mciSendString("open resources\\sound\\smallPlaneDown.mp3 alias smalldown", NULL, 0, NULL);
                        mciSendString("play smalldown", NULL, 0, NULL);
                    }
                    else if (gamemode == 'D')
                    {
                        mciSendString("close middown", NULL, 0, NULL);
                        mciSendString("open resources\\sound\\gege.mp3 alias middown", NULL, 0, NULL);
                        mciSendString("play middown", NULL, 0, NULL);
                    }
                }
                else if (enem[i].kind == 'T')
                {
                    if (gamemode == 'P')
                    {
                        mciSendString("close tinydown", NULL, 0, NULL);
                        mciSendString("open resources\\sound\\tinyPlaneDown.mp3 alias middown", NULL, 0, NULL);
                        mciSendString("play tinydown", NULL, 0, NULL);
                    }
                    else if (gamemode == 'D')
                    {
                        mciSendString("close middown", NULL, 0, NULL);
                        mciSendString("open resources\\sound\\watermelon.mp3 alias middown", NULL, 0, NULL);
                        mciSendString("play middown", NULL, 0, NULL);
                    }
                }
                enem[i].gothit = true;
            }
        }
        for (int j = 0; j < 5; j++)
        {
            if (!bullright[j].live)
                continue;
            if (bullright[j].x > enem[i].x && bullright[j].x <enem[i].x + enem[i].width
                && bullright[j].y>enem[i].y && bullright[j].y < enem[i].y + enem[i].length)
            {
                bullright[j].live = false;
                enem[i].blood--;
                if (enem[i].kind == 'B')
                {
                    if (gamemode == 'P')
                    {
                        mciSendString("close bigdown", NULL, 0, NULL);
                        mciSendString("open resources\\sound\\bigPlaneDown.mp3 alias bigdown", NULL, 0, NULL);
                        mciSendString("play bigdown", NULL, 0, NULL);
                    }
                    else if (gamemode == 'D')
                    {
                        mciSendString("close bigdown", NULL, 0, NULL);
                        mciSendString("open resources\\sound\\nizaima.mp3 alias bigdown", NULL, 0, NULL);
                        mciSendString("play bigdown", NULL, 0, NULL);
                    }
                }
                else if (enem[i].kind == 'M')
                {
                    if (gamemode == 'P')
                    {
                        mciSendString("close middown", NULL, 0, NULL);
                        mciSendString("open resources\\sound\\middlePlaneDown.mp3 alias middown", NULL, 0, NULL);
                        mciSendString("play middown", NULL, 0, NULL);
                    }
                    else if (gamemode == 'D')
                    {
                        mciSendString("close middown", NULL, 0, NULL);
                        mciSendString("open resources\\sound\\homo.mp3 alias middown", NULL, 0, NULL);
                        mciSendString("play middown", NULL, 0, NULL);
                    }
                }
                else if (enem[i].kind == 'S')
                {
                    if (gamemode == 'P')
                    {
                        mciSendString("play smalldown", NULL, 0, NULL);
                        mciSendString("open resources\\sound\\smallPlaneDown.mp3 alias smalldown", NULL, 0, NULL);
                        mciSendString("play smalldown", NULL, 0, NULL);
                    }
                    else if (gamemode == 'D')
                    {
                        mciSendString("close middown", NULL, 0, NULL);
                        mciSendString("open resources\\sound\\gege.mp3 alias middown", NULL, 0, NULL);
                        mciSendString("play middown", NULL, 0, NULL);
                    }
                }
                else if (enem[i].kind == 'T')
                {
                    if (gamemode == 'P')
                    {
                        mciSendString("close tinydown", NULL, 0, NULL);
                        mciSendString("open resources\\sound\\tinyPlaneDown.mp3 alias middown", NULL, 0, NULL);
                        mciSendString("play tinydown", NULL, 0, NULL);
                    }
                    else if (gamemode == 'D')
                    {
                        mciSendString("close middown", NULL, 0, NULL);
                        mciSendString("open resources\\sound\\watermelon.mp3 alias middown", NULL, 0, NULL);
                        mciSendString("play middown", NULL, 0, NULL);
                    }
                }
                enem[i].gothit = true;
            }
        } 
        if (((missile.x > enem[i].x && missile.x  <enem[i].x + enem[i].width
            && missile.y >enem[i].y && missile.y < enem[i].y + enem[i].length) ||
            (missile.x + missile.width < enem[i].x + enem[i].width && missile.x + missile.width >enem[i].x
                && missile.y + missile.length > enem[i].y && missile.y + missile.length < enem[i].y + enem[i].length))&&missile.live)
        {
            missile.live = false;
            missile.num--;
            if(gamemode=='P')
            {
                mciSendString("close bomb", NULL, 0, NULL);
                mciSendString("open resources\\sound\\bomb.mp3 alias bomb", NULL, 0, NULL);
                mciSendString("play bomb", NULL, 0, NULL);
            }
            else if (gamemode == 'D')
            {
                mciSendString("close bomb", NULL, 0, NULL);
                mciSendString("open resources\\sound\\xuebao.mp3 alias bomb", NULL, 0, NULL);
                mciSendString("play bomb", NULL, 0, NULL);
            }
            enem[i].blood -= 50;
        }
        if (enem[i].blood > 0 && enem[i].blood <= 4)
        {
            putimage(enem[i].x + enem[i].width / 4.0, enem[i].y + enem[i].length / 4.0, &boompic[0], NOTSRCERASE);
            putimage(enem[i].x + enem[i].width / 4.0, enem[i].y + enem[i].length / 4.0, &boompic[1], SRCINVERT);
        }
        if (enem[i].blood <= 0)
        {
            enem[i].live = false;
            crush = true;     
            if (enem[i].kind == 'B')
            {
                player.score += 5;//大飞机加5分
            }
            else if (enem[i].kind == 'M')
            {
                player.score += 3;//大飞机加3分
            }
            else if (enem[i].kind == 'S')
            {
                player.score += 2;//大飞机加2分
            }
            else
            {
                player.score += 1;//大飞机加1分
            }
        }
    }
}

void PlaneCrush()
{
    bool crush = false;
    if(player.gothit)
    player.gothit = false;
    for (int i = 0; i < 10; i++)
    {
        if (!enem[i].live)
            continue;
        if ((player.x > enem[i].x && player.x  <enem[i].x + enem[i].width
            && player.y >enem[i].y && player.y < enem[i].y + enem[i].length)||
            (player.x + player.width1 < enem[i].x+ enem[i].width && player.x+player.width1 >enem[i].x
            && player.y + player.length1 >enem[i].y && player.y+player.length1 < enem[i].y + enem[i].length))
        {
            player.blood-=2;
            player.gothit = true;
            enem[i].live = false;
        } 
    }
    for (int i = 0; i < 10; i++)
    {
        if (!enem[i].enembull.live)
            continue;
        if ((player.x > enem[i].enembull.x && player.x  <enem[i].enembull.x + enem[i].enembull.width
            && player.y >enem[i].enembull.y && player.y < enem[i].enembull.y + enem[i].enembull.length) ||
            (player.x + player.width1 < enem[i].enembull.x + enem[i].enembull.width && player.x + player.width1 >enem[i].enembull.x
                && player.y + player.length1 > enem[i].enembull.y && player.y + player.length1 < enem[i].enembull.y + enem[i].enembull.length))
        {
            player.blood -= 1;
            player.gothit = true;
            enem[i].enembull.live = false;
        }
    }
    for (int i = 0; i < 10; i++)
    {
        if (!enem[i].enembullleft.live)
            continue;
        if ((player.x > enem[i].enembullleft.x && player.x  <enem[i].enembullleft.x + enem[i].enembullleft.width
            && player.y >enem[i].enembullleft.y && player.y < enem[i].enembullleft.y + enem[i].enembullleft.length) ||
            (player.x + player.width1 < enem[i].enembullleft.x + enem[i].enembullleft.width && player.x + player.width1 >enem[i].enembullleft.x
                && player.y + player.length1 > enem[i].enembullleft.y && player.y + player.length1 < enem[i].enembullleft.y + enem[i].enembullleft.length))
        {
            player.blood -= 1;
            player.gothit = true;
            enem[i].enembullleft.live = false;
        }
    }
    for(int i=0;i<10;i++)
    {
        if (!enem[i].enembullright.live)
            continue;
        if ((player.x > enem[i].enembullright.x && player.x  <enem[i].enembullright.x + enem[i].enembullright.width
            && player.y >enem[i].enembullright.y && player.y < enem[i].enembullright.y + enem[i].enembullright.length) ||
            (player.x + player.width1 < enem[i].enembullright.x + enem[i].enembullright.width && player.x + player.width1 >enem[i].enembullright.x
                && player.y + player.length1 > enem[i].enembullright.y && player.y + player.length1 < enem[i].enembullright.y + enem[i].enembullright.length))
        {
            player.blood -= 1;
            player.gothit = true;
            enem[i].enembullright.live = false;
        }
        if (player.blood <= 0)
        {
            player.live = false;
            crush = true;
            if (crush)
            {
                putimage(player.x + player.width1 / 4.0, player.y + player.length1 / 4.0, &boompic[0],NOTSRCERASE );
                putimage(player.x + player.width1 / 4.0, player.y + player.length1 / 4.0, &boompic[1], SRCINVERT);
            }
            crush = false;
        }
    }
}

void outtextxy_int(int x, int y, int num)
{
    char text[100] = _T("");
    sprintf_s(text, "%d", num);
    outtextxy(x, y, text);
}

void numcount()
{
    setbkmode(TRANSPARENT);
    settextstyle(20, 0, "STXINGKA");//设置字体大小，格式
    settextcolor(RED);//设置字体颜色
    outtextxy(10, 10, "分数: ");//设置文字位置，内容
    outtextxy_int(70, 10, player.score);
    settextstyle(20, 0, "微软雅黑");//设置字体大小，格式
    settextcolor(BLUE);//设置字体颜色
    outtextxy(10, 30, "导弹数: ");//设置文字位置，内容
    outtextxy_int(70, 30, missile.num);
    settextstyle(20, 0, "YouYuan");//设置字体大小，格式
    settextcolor(YELLOW);//设置字体颜色
    outtextxy(10, 50, "血量: ");//设置文字位置，内容
    outtextxy_int(70, 50, player.blood);
    //printf("$");
}

void GamePause()
{
    if (GetAsyncKeyState('P'))
    {
        while (1)
        {
            if (GetAsyncKeyState('O'))
                break;
        }
    }
}

void EndMenu(char gamemode) {//两个按钮图片加设计//game_go_on
    bool Position1 = false,Position2=false;
    WinButton1.x = 300;
    WinButton1.y = 500;
    WinButton1.width = 180;
    WinButton1.length = 59;
    LoseButton2.x = 600;
    LoseButton2.y = 500;
    LoseButton2.width = 180;
    LoseButton2.length = 59;
    cleardevice();//刷新
    if (gamemode == 'P')
        loadimage(&background, "resources\\universebackground.jpg");//加载背景图片
    else if(gamemode=='D')
        loadimage(&background, "resources\\sky.jpg");//加载背景图片
    loadimage(&win[0], "resources\\win0.jpg");
    loadimage(&win[1], "resources\\win1.jpg");
    loadimage(&lose[0], "resources\\lose0.jpg");
    loadimage(&lose[1], "resources\\lose1.jpg"); 
    loadimage(&wannaplay[0], "resources\\wannaplay0.jpg");
    loadimage(&wannaplay[1], "resources\\wannaplay1.jpg");
    loadimage(&yesbutt[0], "resources\\yesbutton0.jpg");
    loadimage(&yesbutt[1], "resources\\yesbutton1.jpg");
    loadimage(&yesbutt[2], "resources\\yesbutton2.jpg");
    loadimage(&nobutt[0], "resources\\nobutton0.jpg");
    loadimage(&nobutt[1], "resources\\nobutton1.jpg");
    loadimage(&nobutt[2], "resources\\nobutton2.jpg");
    if (player.score >= winscore)
    {
        mciSendString("close bkmusic", NULL, 0, NULL);
        mciSendString("open resources\\winmusic.mp3 alias bkmusic", NULL, 0, NULL);
        mciSendString("play bkmusic repeat", NULL, 0, NULL);
    }
    else if (player.blood<=0)
    {
        mciSendString("close bkmusic", NULL, 0, NULL);
        mciSendString("open resources\\losemusic.mp3 alias bkmusic", NULL, 0, NULL);
        mciSendString("play bkmusic repeat", NULL, 0, NULL);
    }
    while(1)
    {
        BeginBatchDraw();
        putimage(0, 0, &background);
        putimage(WinButton1.x, WinButton1.y, &yesbutt[0], NOTSRCERASE);
        putimage(LoseButton2.x, LoseButton2.y, &nobutt[0], NOTSRCERASE);
        putimage(429, 382, &wannaplay[0], NOTSRCERASE);//222*68
        putimage(429, 382, &wannaplay[1], SRCINVERT);
        if (player.score >= winscore) {
            putimage(280, 50, &win[0], NOTSRCERASE);
            putimage(280, 50, &win[1], SRCINVERT);
        }
        else if (player.blood <= 0)
        {
            putimage(280, 50, &lose[0], NOTSRCERASE);
            putimage(280, 50, &lose[1], SRCINVERT);
        }
        if (!Position1)
        {
            putimage(WinButton1.x, WinButton1.y, &yesbutt[1], SRCINVERT);

        }
        if (!Position2)
        {
            putimage(LoseButton2.x, LoseButton2.y, &nobutt[1], SRCINVERT);
        }
        ExMessage msg;
        peekmessage(&msg);
        if (msg.x >= WinButton1.x && msg.x <= WinButton1.x + WinButton1.width &&
            msg.y >= WinButton1.y && msg.y <= WinButton1.y + WinButton1.length)
        {
            Position1 = true;
            if (Position1)
            {
                putimage(WinButton1.x, WinButton1.y, &yesbutt[2], SRCINVERT);
            }
            if (msg.message == WM_LBUTTONDOWN)
            {
                game_go_on = true;
                break;
            }
        }
        else if (msg.x < WinButton1.x || msg.x > WinButton1.x + WinButton1.width ||
            msg.y < WinButton1.y || msg.y > WinButton1.y + WinButton1.length)
        {
            Position1 = false;
        }
        if (msg.x >= LoseButton2.x && msg.x <= LoseButton2.x + LoseButton2.width &&
            msg.y >= LoseButton2.y && msg.y <= LoseButton2.y + LoseButton2.length)
        {
            Position2 = true;
            if (Position2)
            {
                putimage(LoseButton2.x, LoseButton2.y, &nobutt[2], SRCINVERT);
            }
            if (msg.message == WM_LBUTTONDOWN)
            {
                game_go_on = false;
                break;
            }
        }
        else if (msg.x < LoseButton2.x || msg.x > LoseButton2.x + LoseButton2.width ||
            msg.y < LoseButton2.y || msg.y > LoseButton2.y + LoseButton2.length)
        {
            Position2 = false;
        }
        FlushBatchDraw();
    }
    EndBatchDraw();
}

bool RandTime(int t)//计时器,使得飞机能够不每次循环同时生成
{
    static int time1, time2;//static防止下次调用又归为零
    if (time2 - time1 >t)//每t毫秒执行一次
    {
        time1 = time2;
        return true;
    }
    time2 = clock();//获取时间
    return false;
}

int main()
{
    srand((unsigned)time(NULL)); // 随机种子
    initgraph(Width_bk, Height_bk,SHOWCONSOLE);
    while(game_go_on)
    {
        startmenu();
        GameInitialize(gamemode);
        ImageLoad(gamemode);
        starinit();
        BeginBatchDraw();
        while (player.blood>0&&player.score<winscore)
        {
            Drawpic();
            Starmake();
            numcount();
            PlayerMove(5,gamemode);
            BullMove();
            if (RandTime(640))
            {
                EnemyCreat();
            }
            EnemyMove(rand() % 5);
            EnemyBulletMove();
            Hit(gamemode);
            PlaneCrush();
            MissileUse(gamemode);
            SupplyUse(4);
            GamePause();
            FlushBatchDraw();
        }
        EndBatchDraw();
        EndMenu(gamemode);
    }
    return 0;
}


/*任务
1.实现声音修改*/

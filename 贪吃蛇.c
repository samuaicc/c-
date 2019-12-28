//游戏主要实现原理是通过控制蛇头的方向和坐标实现控制整条蛇的移动
//开始会在中随机生成一个食物，然后控制蛇移动去吃食物
//如果吃到食物，蛇总长度加 1，蛇的原头部的位置变为蛇身，蛇的新头部在原头部向某一方向移动后的第一个位置
//如果没有吃到食物，抹去蛇的最后一节身体，然后前边的身体坐标依次等于其之前一个的蛇身坐标，蛇头向前移动一个位置
//如果碰到围墙或身体蛇死亡，游戏结束
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>
#include <string.h>
#include <time.h>
#define MAXRANDOMVALUE 16
//声明全局函数
void food();
void show();
void move();
void turn();
void check();
void ini();
void print();
void gotoxy(int x,int y);
void HideCursor();
//声明全局变量
int map[17][17];   //定义一个二维数组存放地图，其中存放数字0.1.2.3.4   0代表空 1代表围墙 2代表蛇的身体 3代表蛇头 4代表食物
int  dy[4] = { 0, 1, 0, -1 };    //用于改变蛇头的移动方向和坐标
int  dx[4] = { -1, 0, 1, 0 };
int sum = 5;  //蛇总长度
int over = 0;  //判断是否结束
int speed;  //控制速度（通过控制界面刷新速度）
int score;  //记录分数，一个食物的分数为 1 分
//定义一个结构体用于存放蛇的数据，其中头部有单独的方向数据
struct snake {
    int x, y;  //身体坐标
    int dir;  //方向（只有蛇头的方向是有用的）
} A[100];

void ini() {  //初始化输出界面
    speed = 500;  //初始速度
    over = 0;   //判断是否结束
    sum = 5;   //初始化蛇长为5
    score = 0; //初始化分数为0
    int i, j;
    for (i = 0; i < 100; i++)
    {  //初始化蛇头和蛇身的数据
        A[i].dir = 0;
        A[i].x = 0;
        A[i].y = 0;
    }
    A[0].x = 1; A[0].y = 1;  //地图左上角设置一条长度为5的蛇
    A[1].x = 1; A[1].y = 2;
    A[2].x = 1; A[2].y = 3;
    A[3].x = 1; A[3].y = 4;
    A[4].x = 1; A[4].y = 5; A[4].dir = 1;  //初始化蛇头的位置
    for (i = 0; i < 17; i++)           //设置地图
    {
        for (j = 0; j < 17; j++)
        {
            map[i][j] = 1;
        }
    }
    for (i = 1; i < 16; i++)          //设置地图
    {
        for (j = 1; j < 16; j++)
        {
            map[i][j] = 0;
        }
    }
    map[A[4].x][A[4].y] = 3;         //设置蛇头
    for (i = 0; i < sum - 1; i++)    //设置蛇身
    {
        map[A[i].x][A[i].y] = 2;
    }
    food();
}


void print()    //显示界面
{
    gotoxy(0,0);    //将光标定位到(0,0)的位置然后输出(刷新)
    int i, j;
    int all_long = sum;    //设置一个变量记录蛇长
    printf("\t\t\t   SCORE:%3d\t\t     LONG:%3d\n\n",score,all_long);
    for (i = 0; i < 17; i++)    //循环判断输出界面
    {
        printf("\t\t\t   ");
        for (j=0;j<17;j++)
        {
            if(map[i][j] == 1)
            {
                printf("■");    //围墙
            }
            else if(map[i][j] == 2)
            {
                printf("◆");    //蛇身
            }
            else if(map[i][j] == 3)
            {
                printf("●");    //蛇头
            }
            else if(map[i][j] == 4)
            {
                printf("★");    //食物
            }
            else
            {
                printf("  ");
            }
        }
        printf("\n");
    }
}

void HideCursor()  //隐藏光标，防止干扰视线
{
	CONSOLE_CURSOR_INFO cursor;
	cursor.bVisible = FALSE;
	cursor.dwSize = sizeof(cursor);
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorInfo(handle, &cursor);
}


void gotoxy(int x,int y)  //光标移动函数
{
    //调用win32 API 去设置控制台的光标位置
    //1.找到控制台的窗口
    HANDLE handle=GetStdHandle(STD_OUTPUT_HANDLE);
    //2.光标结构体
    COORD coord;
    //3.设置坐标
    coord.X=x;
    coord.Y=y;
    //4.同步到控制台/设置控制台光标的位置
    SetConsoleCursorPosition(handle,coord);
}

void show() {  //显示界面
    int x, y;
    print();  //s输出界面
    while (1)
    {
        Sleep(speed);  //界面刷新速度,用来控制蛇的移动速度
        turn();  //改变蛇头方向
        move();  //移动整条蛇
        if (over)  //判断蛇是否死亡
        {  //设置蛇死掉后可以进行的操作
            while (1)
            {
                char ch = _getch();
                if (ch == 113)    //输入‘q’结束
                {
                    return;
                }
                else if (ch == 114)    //输入‘r’重新开始
                {
                    system("cls");
                    ini();
                    break;
                }
            }
        }
        print();
    }
}

void food() {  //生成食物
    srand(time(NULL));   //利用系统时间返回一个随机种子
    int x,y;
    while(1)
    {
        x=(rand())%(MAXRANDOMVALUE+1);  //通过取余数来得到一个整数，切随机整数的最大值不会超过MAXRANDOMVALUE
        y=(rand())%(MAXRANDOMVALUE+1);
        if (map[x][y] == 0)       //如果是空格则在该处生成食物
        {
            map[x][y] = 4;
            break;
        }
    }
}


void move()
{
    //蛇移动
    int i, x, y;
    int t = sum;  //t记录当前蛇总长度
    check();  //移动前检查按当前方向移动一步后的情况,如果死亡游戏结束，如果吃到食物，长度会+1
    if (t == sum)
    {   //没有吃到苹果（长度没有改变）
        for (i = 0; i < sum - 1; i++)
        {
            if (i == 0)
            {  //蛇尾坐标处变成空格，把蛇尾坐标变成前一个蛇身的坐标
                map[A[i].x][A[i].y] = 0;
                A[i].x = A[i + 1].x;
                A[i].y = A[i + 1].y;
            }
            else
            {  //每个蛇身坐标都变为它前一个蛇身的坐标
                A[i].x = A[i + 1].x;
                A[i].y = A[i + 1].y;
            }
            map[A[i].x][A[i].y] = 2;  //把地图上蛇身坐标处设置为蛇身
        }
        A[sum - 1].x = A[sum - 1].x + dx[A[sum - 1].dir];
        A[sum - 1].y = A[sum - 1].y + dy[A[sum - 1].dir];     //蛇头按当前方向移动一格
        map[A[sum - 1].x][A[sum - 1].y] = 3;  //把地图上蛇头坐标处设置为蛇头
    }
    else
    {   //吃到苹果（sum会加1，长度发生改变）
        map[A[sum - 2].x][A[sum - 2].y] = 2;  //把地图上原蛇头坐标处设置为蛇身
        A[sum - 1].x = A[sum - 2].x + dx[A[sum - 2].dir];  //新蛇头的坐标是原蛇头沿当前方向移动一格后的坐标
        A[sum - 1].y = A[sum - 2].y + dy[A[sum - 2].dir];
        A[sum - 1].dir = A[sum - 2].dir;  //新蛇头方向为原蛇头的方向
        map[A[sum - 1].x][A[sum - 1].y] = 3;  //把地图上蛇头坐标处设置为蛇头
        food();
    }
}

void check()
{  //检查是否死亡或者吃到食物
    int x, y, i, j;
    x = A[sum - 1].x + dx[A[sum - 1].dir];  //记录按当前方向移动一格后蛇头的坐标
    y = A[sum - 1].y + dy[A[sum - 1].dir];
    if (map[x][y] == 1 || map[x][y] == 2)
    {  //如果地图上该坐标处字符为‘*’或‘X’就死亡
        if (x != A[0].x || y != A[0].y)
        {
            //蛇尾除外
            print();
            printf("\n\t\t\t     GAME OVER !!! YOU SCORE IS %3d;\n\n",score);
            printf("\t\t\t\t   INPUT <R> TO RESET\n\n\t\t\t\t   Input <Q> TO QUIT\n");
            over = 1;
        }
    }
    else if (map[x][y] == 4) //吃到苹果
    {
        score += 1;
        sum++;  //蛇身总长加 1
        speed = ((600 - sum * 20)>100) ? (600 - sum * 20) : 100; //（刷新）速度加快,最快的（刷新）速度为100ms
    }
}

void turn()  //蛇头方向控制函数
{
    if (kbhit())    //_kbhit()检查当前是否有键盘输入，若有则返回一个非0值，否则返回0,是非阻塞函数
    {
        int ch = getch(); //读取输入的键的键值,通过键值控制方向
        switch (ch)
        {
            //改变方向
            case 119: A[sum - 1].dir = (A[sum - 1].dir == 2)?2:0; break;  //上
            case 100: A[sum - 1].dir = (A[sum - 1].dir == 3)?3:1; break;  //右
            case 115: A[sum - 1].dir = (A[sum - 1].dir == 0)?0:2; break;  //下
            case 97: A[sum - 1].dir = (A[sum - 1].dir == 1)?1:3; break;   //左
        }
    }
}

int main() {
    HideCursor();
    printf("\n\n\n\n\n\t\t   ###'w''s''a''d'控制上下左右|开始前将键盘改为英文输入###\n\n\n");
    printf("\t\t\t###每吃到食物一次加一分|撞到围墙或身体死亡###\n\n\n");
    printf("\t\t\t  ###蛇的速度会不断加快|努力控制好方向哦###\n\n\n");
    printf("\t\t\t\t     >>>按任意键开始<<<\n");
    char ch = _getch(); //读取一个按键后开始游戏
    system("cls");    //清屏
    ini();
    show();
    return 0;
}


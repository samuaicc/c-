//��Ϸ��Ҫʵ��ԭ����ͨ��������ͷ�ķ��������ʵ�ֿ��������ߵ��ƶ�
//��ʼ�������������һ��ʳ�Ȼ��������ƶ�ȥ��ʳ��
//����Ե�ʳ����ܳ��ȼ� 1���ߵ�ԭͷ����λ�ñ�Ϊ�����ߵ���ͷ����ԭͷ����ĳһ�����ƶ���ĵ�һ��λ��
//���û�гԵ�ʳ�Ĩȥ�ߵ����һ�����壬Ȼ��ǰ�ߵ������������ε�����֮ǰһ�����������꣬��ͷ��ǰ�ƶ�һ��λ��
//�������Χǽ����������������Ϸ����
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>
#include <string.h>
#include <time.h>
#define MAXRANDOMVALUE 16
//����ȫ�ֺ���
void food();
void show();
void move();
void turn();
void check();
void ini();
void print();
void gotoxy(int x,int y);
void HideCursor();
//����ȫ�ֱ���
int map[17][17];   //����һ����ά�����ŵ�ͼ�����д������0.1.2.3.4   0����� 1����Χǽ 2�����ߵ����� 3������ͷ 4����ʳ��
int  dy[4] = { 0, 1, 0, -1 };    //���ڸı���ͷ���ƶ����������
int  dx[4] = { -1, 0, 1, 0 };
int sum = 5;  //���ܳ���
int over = 0;  //�ж��Ƿ����
int speed;  //�����ٶȣ�ͨ�����ƽ���ˢ���ٶȣ�
int score;  //��¼������һ��ʳ��ķ���Ϊ 1 ��
//����һ���ṹ�����ڴ���ߵ����ݣ�����ͷ���е����ķ�������
struct snake {
    int x, y;  //��������
    int dir;  //����ֻ����ͷ�ķ��������õģ�
} A[100];

void ini() {  //��ʼ���������
    speed = 500;  //��ʼ�ٶ�
    over = 0;   //�ж��Ƿ����
    sum = 5;   //��ʼ���߳�Ϊ5
    score = 0; //��ʼ������Ϊ0
    int i, j;
    for (i = 0; i < 100; i++)
    {  //��ʼ����ͷ�����������
        A[i].dir = 0;
        A[i].x = 0;
        A[i].y = 0;
    }
    A[0].x = 1; A[0].y = 1;  //��ͼ���Ͻ�����һ������Ϊ5����
    A[1].x = 1; A[1].y = 2;
    A[2].x = 1; A[2].y = 3;
    A[3].x = 1; A[3].y = 4;
    A[4].x = 1; A[4].y = 5; A[4].dir = 1;  //��ʼ����ͷ��λ��
    for (i = 0; i < 17; i++)           //���õ�ͼ
    {
        for (j = 0; j < 17; j++)
        {
            map[i][j] = 1;
        }
    }
    for (i = 1; i < 16; i++)          //���õ�ͼ
    {
        for (j = 1; j < 16; j++)
        {
            map[i][j] = 0;
        }
    }
    map[A[4].x][A[4].y] = 3;         //������ͷ
    for (i = 0; i < sum - 1; i++)    //��������
    {
        map[A[i].x][A[i].y] = 2;
    }
    food();
}


void print()    //��ʾ����
{
    gotoxy(0,0);    //����궨λ��(0,0)��λ��Ȼ�����(ˢ��)
    int i, j;
    int all_long = sum;    //����һ��������¼�߳�
    printf("\t\t\t   SCORE:%3d\t\t     LONG:%3d\n\n",score,all_long);
    for (i = 0; i < 17; i++)    //ѭ���ж��������
    {
        printf("\t\t\t   ");
        for (j=0;j<17;j++)
        {
            if(map[i][j] == 1)
            {
                printf("��");    //Χǽ
            }
            else if(map[i][j] == 2)
            {
                printf("��");    //����
            }
            else if(map[i][j] == 3)
            {
                printf("��");    //��ͷ
            }
            else if(map[i][j] == 4)
            {
                printf("��");    //ʳ��
            }
            else
            {
                printf("  ");
            }
        }
        printf("\n");
    }
}

void HideCursor()  //���ع�꣬��ֹ��������
{
	CONSOLE_CURSOR_INFO cursor;
	cursor.bVisible = FALSE;
	cursor.dwSize = sizeof(cursor);
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorInfo(handle, &cursor);
}


void gotoxy(int x,int y)  //����ƶ�����
{
    //����win32 API ȥ���ÿ���̨�Ĺ��λ��
    //1.�ҵ�����̨�Ĵ���
    HANDLE handle=GetStdHandle(STD_OUTPUT_HANDLE);
    //2.���ṹ��
    COORD coord;
    //3.��������
    coord.X=x;
    coord.Y=y;
    //4.ͬ��������̨/���ÿ���̨����λ��
    SetConsoleCursorPosition(handle,coord);
}

void show() {  //��ʾ����
    int x, y;
    print();  //s�������
    while (1)
    {
        Sleep(speed);  //����ˢ���ٶ�,���������ߵ��ƶ��ٶ�
        turn();  //�ı���ͷ����
        move();  //�ƶ�������
        if (over)  //�ж����Ƿ�����
        {  //��������������Խ��еĲ���
            while (1)
            {
                char ch = _getch();
                if (ch == 113)    //���롮q������
                {
                    return;
                }
                else if (ch == 114)    //���롮r�����¿�ʼ
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

void food() {  //����ʳ��
    srand(time(NULL));   //����ϵͳʱ�䷵��һ���������
    int x,y;
    while(1)
    {
        x=(rand())%(MAXRANDOMVALUE+1);  //ͨ��ȡ�������õ�һ����������������������ֵ���ᳬ��MAXRANDOMVALUE
        y=(rand())%(MAXRANDOMVALUE+1);
        if (map[x][y] == 0)       //����ǿո����ڸô�����ʳ��
        {
            map[x][y] = 4;
            break;
        }
    }
}


void move()
{
    //���ƶ�
    int i, x, y;
    int t = sum;  //t��¼��ǰ���ܳ���
    check();  //�ƶ�ǰ��鰴��ǰ�����ƶ�һ��������,���������Ϸ����������Ե�ʳ����Ȼ�+1
    if (t == sum)
    {   //û�гԵ�ƻ��������û�иı䣩
        for (i = 0; i < sum - 1; i++)
        {
            if (i == 0)
            {  //��β���괦��ɿո񣬰���β������ǰһ�����������
                map[A[i].x][A[i].y] = 0;
                A[i].x = A[i + 1].x;
                A[i].y = A[i + 1].y;
            }
            else
            {  //ÿ���������궼��Ϊ��ǰһ�����������
                A[i].x = A[i + 1].x;
                A[i].y = A[i + 1].y;
            }
            map[A[i].x][A[i].y] = 2;  //�ѵ�ͼ���������괦����Ϊ����
        }
        A[sum - 1].x = A[sum - 1].x + dx[A[sum - 1].dir];
        A[sum - 1].y = A[sum - 1].y + dy[A[sum - 1].dir];     //��ͷ����ǰ�����ƶ�һ��
        map[A[sum - 1].x][A[sum - 1].y] = 3;  //�ѵ�ͼ����ͷ���괦����Ϊ��ͷ
    }
    else
    {   //�Ե�ƻ����sum���1�����ȷ����ı䣩
        map[A[sum - 2].x][A[sum - 2].y] = 2;  //�ѵ�ͼ��ԭ��ͷ���괦����Ϊ����
        A[sum - 1].x = A[sum - 2].x + dx[A[sum - 2].dir];  //����ͷ��������ԭ��ͷ�ص�ǰ�����ƶ�һ��������
        A[sum - 1].y = A[sum - 2].y + dy[A[sum - 2].dir];
        A[sum - 1].dir = A[sum - 2].dir;  //����ͷ����Ϊԭ��ͷ�ķ���
        map[A[sum - 1].x][A[sum - 1].y] = 3;  //�ѵ�ͼ����ͷ���괦����Ϊ��ͷ
        food();
    }
}

void check()
{  //����Ƿ��������߳Ե�ʳ��
    int x, y, i, j;
    x = A[sum - 1].x + dx[A[sum - 1].dir];  //��¼����ǰ�����ƶ�һ�����ͷ������
    y = A[sum - 1].y + dy[A[sum - 1].dir];
    if (map[x][y] == 1 || map[x][y] == 2)
    {  //�����ͼ�ϸ����괦�ַ�Ϊ��*����X��������
        if (x != A[0].x || y != A[0].y)
        {
            //��β����
            print();
            printf("\n\t\t\t     GAME OVER !!! YOU SCORE IS %3d;\n\n",score);
            printf("\t\t\t\t   INPUT <R> TO RESET\n\n\t\t\t\t   Input <Q> TO QUIT\n");
            over = 1;
        }
    }
    else if (map[x][y] == 4) //�Ե�ƻ��
    {
        score += 1;
        sum++;  //�����ܳ��� 1
        speed = ((600 - sum * 20)>100) ? (600 - sum * 20) : 100; //��ˢ�£��ٶȼӿ�,���ģ�ˢ�£��ٶ�Ϊ100ms
    }
}

void turn()  //��ͷ������ƺ���
{
    if (kbhit())    //_kbhit()��鵱ǰ�Ƿ��м������룬�����򷵻�һ����0ֵ�����򷵻�0,�Ƿ���������
    {
        int ch = getch(); //��ȡ����ļ��ļ�ֵ,ͨ����ֵ���Ʒ���
        switch (ch)
        {
            //�ı䷽��
            case 119: A[sum - 1].dir = (A[sum - 1].dir == 2)?2:0; break;  //��
            case 100: A[sum - 1].dir = (A[sum - 1].dir == 3)?3:1; break;  //��
            case 115: A[sum - 1].dir = (A[sum - 1].dir == 0)?0:2; break;  //��
            case 97: A[sum - 1].dir = (A[sum - 1].dir == 1)?1:3; break;   //��
        }
    }
}

int main() {
    HideCursor();
    printf("\n\n\n\n\n\t\t   ###'w''s''a''d'������������|��ʼǰ�����̸�ΪӢ������###\n\n\n");
    printf("\t\t\t###ÿ�Ե�ʳ��һ�μ�һ��|ײ��Χǽ����������###\n\n\n");
    printf("\t\t\t  ###�ߵ��ٶȻ᲻�ϼӿ�|Ŭ�����ƺ÷���Ŷ###\n\n\n");
    printf("\t\t\t\t     >>>���������ʼ<<<\n");
    char ch = _getch(); //��ȡһ��������ʼ��Ϸ
    system("cls");    //����
    ini();
    show();
    return 0;
}


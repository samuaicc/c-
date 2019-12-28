#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>  //按键操作
#include <time.h>  //时间函数，用于获得时间从而获得时间种子
#define MAXRANDOMVALUE 3  //设置一个最大随机值，(每次得到的最大随机数)，因为4*4二维数组的最大下标值为3，所以设置最大随机值为3
#define TXT_FILE "max.txt" //定义一个文件用于保存历史最高分

//全局声明文件中所包括的功能模块，便于调用，避免多次声明
void print();  //屏幕输出函数，输出整个界面和刷新输出界面
int play(int arr[],int j,int z_o_f);  //主要的计算函数，将分割的以为数组逐个进行运算，并得出结果
void gotoxy(int x,int y);  //光标控制函数，用于控制输出的位置
void cut_arry(int d,int p);  //通过读取按键操作，把二维数组分割为不同的一维数组进行操作
void resetgame();  //重置游戏
void random_data();  //随机生成数字2或4;
void over_detection();   //判断游戏方格中是否被占满，如果方格满就通过if_over_fuction()判断游戏是否结束
void if_over_fuction();  //判断游戏是否结束
void gameover();  //游戏是否结束提示
void if_reset();  //判断是否重置游戏
void save_max();  //保存最高分
void read_max();  //读取最高分
//声明全局变量做用于全局
int arry[4][4] = {0}; //声明一个4*4的二维数组，用于抽象化的操作和存储数值
int if_over_arry[4] = {0};  //用于游戏结束判断操作
int if_over_arry2[4] = {0};  //用于游戏结束判断操作
int not_over = 0;  //用于标记游戏是否结束
int win = 0;  //用于标记游戏是否获得胜利
int score = 0;  //用于计算所得分数
int best = 0;  //用于存储游戏最历史高分
int cycle = 1;  //用于标记按键是否需要继续操作

int main()
{
    read_max();    //读取文件中保存的最高分
    random_data();   //开局随机生成两个数在表格中
    random_data();
    int ch=0;
    while (cycle)   //cycle 是全局的变量，开始为 1 (死循环)；游戏结束或退出时变为 0 (结束循环)
    {
        ch=getch();  //将读取的值赋值给ch
        switch (ch)
        {
            case 72: cut_arry(1,0);break;  //按下方向上键，数字整体向上移动并计算
            case 80: cut_arry(1,1);break;  //按下方向下键，数字整体向下移动并计算
            case 75: cut_arry(0,0);break;  //按下方向左键，数字整体向左移动并计算
            case 77: cut_arry(0,1);break;  //按下方向右键，数字整体向右移动并计算
            case 114: resetgame();break;  //r -> 重置程序
            case 116: cycle = 0;break;  //t -> 退出程序
            break;
        }
    }
}



void save_max()  //保存最高分
{
    FILE *fp;
    fp=fopen(TXT_FILE,"w");
    fprintf(fp,"%d",best);
    fclose(fp);
}

void read_max()  //读取最高分
{
    FILE *fp;
    fp=fopen(TXT_FILE,"r");
    fscanf(fp,"%d",&best);
    fclose(fp);
}



void over_detection()  //游戏结束判断
{
    int i,j;
    int zero = 0;   //初始定义二维数组中没有 0 也就是没有空位
    for (i=0;i<4;i++)  //循环判断数组中是否有 0（空格）
    {
        for (j=0;j<4;j++)
        {
            if (arry[i][j] == 2048)  //如果数组中的数字有 2048 游戏结束，玩家胜利
            {
                win += 1;     //标记游戏胜利
                gameover();
                break;
            }
            else if (arry[i][j] == 0)
            {
                zero += 1;   //如果有 0（空格）就将标记改变
                break;
            }
        }
    }
    if (zero == 0)    //如果没有找到空格，就要判断游戏是否结束
    {
        for (i=0;i<4;i++)
        {
            for (j=0;j<4;j++)
            {
                if_over_arry[j] = arry[i][j];    //将二维数组依次分割为行
                if_over_arry2[j] = arry[j][i];   //将二维数组依次分割为列
            }
            if_over_fuction();  //调用函数对分割后的一维数组进行结束判断
        }
        gameover();  //通过判断后的结果来输出对应的提示信息
    }
    else  //否则继续生成随机数
    {
        random_data();
    }
}


void if_over_fuction()  //判断游戏是否结束
{
    not_over = 0;
    int i;
    //初始定义标记not_over为结束(0)
    for (i=0;i<3;i++)  //循环判断一维数组的状态
    {
        if ((if_over_arry[i] == if_over_arry[i+1]) || (if_over_arry2[i] == if_over_arry2[i+1]))   //如果某一个数字相邻的位置与其相等，就代表游戏未结束
        {
            not_over += 1;  //标记未结束
            break;
        }
    }
    memset(if_over_arry,0,sizeof(if_over_arry));    //清空判断时用于存储的数组，方便进行下一次判断
    memset(if_over_arry2,0,sizeof(if_over_arry2));
}


void random_data()
{
    int no;
    srand(time(NULL));   //利用系统时间返回一个随机数
    int i,j;
    do
    {
        i=(rand())%(MAXRANDOMVALUE+1);  //通过取余数来得到一个整数，切随机整数的最大值不会超过MAXRANDOMVALUE
        j=(rand())%(MAXRANDOMVALUE+1);
    }while(arry[i][j]!=0);   //如果随机生成的坐标处没有数字(0),就在那个位置随机生成数字

    no=2*((rand ()%10)+1);  //通过判断随机数的余数来生成2或4，生成4的几率比2小
    if(no==3 || no==2)
    {
        arry[i][j]=4;
    }
    else
    {
        arry[i][j]=2;
    }
    print();
}


void resetgame()    //重置游戏
{
    int i,j;
    for(i=0;i<4;i++)
    {
        for(j=0;j<4;j++)
        {
            arry[i][j]=0;   //将数组中的所有数字重置为 0
        }
    }
    system("cls");  //清屏
    score = 0;  //将分数清零
    cycle = 1;  //将循环标记重置
    not_over = 0;  //重置未结束标记
    win = 0;  //重置获胜标记
    main();
}


void gotoxy(int x,int y)  //光标控制函数，控制光标到控制台的某一(初始)位置
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



void print()   //界面打印输出函数
{
    if (score >= best)  //如果当前分数比最高分高，就重置最高分
    {
        best = score;
    }
    gotoxy(0,0);  //控制光标到初始位置
    printf("\t       ┌       HAVE      A      GOOD      TIME     !    ┐\n");
    printf("\t       └           加    油   ！  奥   力   给  !       ┘\n\n");
    printf("\t\t          SCORE: %05d  │  BEST: %06d\n", score, best);
    printf("\t\t--------------------------------------------------");

    /* 绘制方格和数字 */
    printf("\n\t\t\t     ┌────┬────┬────┬────┐\n");
    int i;
    for (i = 0; i < 4; i++)    //用循环输出界面
    {
        printf("                             │");
        int j;
        for (j = 0; j < 4; j++)
        {
            if (arry[i][j] != 0)
            {
                if (arry[i][j] < 10)
                {
                    printf("  %d │", arry[i][j]);
                }
                else if (arry[i][j] < 100)
                {
                    printf(" %d │", arry[i][j]);
                }
                else if (arry[i][j] < 1000)
                {
                    printf(" %d│", arry[i][j]);
                }
                else if (arry[i][j] < 10000)
                {
                    printf("%4d│", arry[i][j]);
                }
                else
                {
                    int n = arry[i][j];
                    int k;
                    for (k = 1; k < 20; ++k)
                    {
                        n = n >> 1;
                        if (n == 1)
                        {
                            printf("2^%02d│", k); /* 超过四位的数字用2的幂形式表示，如2^13形式 */
                            break;
                        }
                    }
                }
            }
            else printf("    │");
        }
        if (i < 3)
        {
            printf("\n\t\t\t     ├────┼────┼────┼────┤\n");
        }
        else
        {
            printf("\n\t\t\t     └────┴────┴────┴────┘\n");
        }
    }
    printf("\n");
    printf("\t\t--------------------------------------------------\n");
    printf("\t\t              [↑]:UP    [↓]:DOWN\n");
    printf("\t\t              [←]:LEFT  [→]:RIGHT\n");
    printf("\t\t              [R]:RESET  [T]:EXIT\n\n\n");
}


void cut_arry(int d,int p)    //将二维数组切割为一维数组在进行计算，通过参数控制切割方式(d,p的值为0,1)
{
    int i,j;
    if (d == 0 && p == 0)  //d=0表示切割的是行,p=0表示将数字按从左到右取出并从左到右放置在新的一维数组中(按下方向左键)
    {
        for (i=0;i<4;i++)
        {
            int arrys[4] = {};
            for (j=0;j<4;j++)
            {
                arrys[j] = arry[i][j];
            }
            play(arrys,i,0);  //将切割的数组进行计算移动
        }
        over_detection();   //判断移动后的数组是否结束
    }
    else if (d == 0 && p == 1)  //d=0表示切割的是行,p=1表示将数字按从左到右取出并从右到左放置在新的一维数组中(按下方向右键)
    {
        for (i=0;i<4;i++)
        {
            int arrys[4] = {};
            for (j=3;j>=0;j--)
            {
                arrys[3-j] = arry[i][j];
            }
            play(arrys,i,1);
        }
        over_detection();
    }
    else if (d == 1 && p == 0)  //d=1表示切割的是行,p=1表示将数字按从上到下取出并从左到右放置在新的一维数组中(按下方向上键)
    {
        for (i=0;i<4;i++)
        {
            int arrys[4] = {};
            for (j=0;j<4;j++)
            {
                arrys[j] = arry[j][i];
            }
            play(arrys,i,2);
        }
        over_detection();
    }
    else
    {
        for (i=0;i<4;i++)
        {
            int arrys[4] = {};
            for (j=3;j>=0;j--)
            {
                arrys[3-j] = arry[j][i];   //否则将数字按从上到下取出并从右到左放置在新的一维数组中(按下方向下键)
            }
            play(arrys,i,3);
        }
        over_detection();
    }
}


int play(int arrs[],int j,int z_o_f)  //j代表二维数组的行或列，z_o_f代表将计算后的数字放回二维数组方式
{
    int i,nextI,m;  //定义i和m作为循环控制变量;定义indexI作为存储下标的变量
    for (i = 0; i < 4; i++)    //遍历数组，从下标为0的值开始
    {
        nextI = -1;   //先为indexI定义一个不为[0,4]的值，用于保存目标值的下标和条件判断，每次循环indexI都重置为-1
        for (m = i+1; m < 4; m++) //从arry[i]自身向数组后面遍历，直到找到除自身外第一个不为0的数字，如果没有，就什么也不做
        {
            if(arrs[m] != 0)
            {
                nextI = m;
                break;
            }
        }
        if (nextI !=-1)
        {
            //存在下个不为0的位置
            if (arrs[i] == 0)  //如果arry[i]的数是0，就把这个数和找到的第一个数交换，然后再从arry[i]的位置开始往后寻找下一个数字
            {
                arrs[i] = arrs[nextI];
                arrs[nextI] = 0;
                i--;  //目的是让循环再次从i开始，找到下一个不为零的数字，(i--与循环结束后的i++抵消)
            }
            else if (arrs[i] == arrs[nextI]) //如果arry[i]和找到的那个数字相等，arry[i]就自身乘以2，并将找到的数字的值变为0，相当于两者相加并将值放在arry[i]中
            {
                arrs[i] *= 2;
                arrs[nextI] = 0;
                score += arrs[i];
            }
            //如果上面两个条件都不符合，也就是指找到的两个数不相等，就什么也不做
        }
    }
    for (i=0;i<4;i++)  //将移动计算后的数组中的数字放回二维数组中
    {
        //z_o_f 用于标记放回的方式
        if (z_o_f == 0)  //z_o_f=0表示将一维数组从左到右放置到二维数组的第J行从左到右的位置
        {
            arry[j][i] = arrs[i];
        }
        else if (z_o_f == 1)  //z_o_f=1表示将一维数组从左到右放置到二维数组的第J行从右到左的位置
        {
            arry[j][3-i] = arrs[i];
        }
        else if (z_o_f == 2)  //z_o_f=2表示将一维数组从左到右放置到二维数组的第J列从上到下的位置
        {
            arry[i][j] = arrs[i];
        }
        else  //z_o_f=表示将一维数组从左到右放置到二维数组的第J列从下到上的位置
        {
            arry[3-i][j] = arrs[i];
        }
    }
}

void gameover()  //游戏结束提示
{
    if (not_over == 0)  //如果未结束的标记为 0，代表游戏结束，玩家失败
    {
        save_max();  //保存最高分
        cycle = 0;  //结束按键读取操作
        system("cls"); //清屏
        printf("                \t\t..:::::::::..\n\
\t\t           ..:::aad8888888baa:::..\n\
\t\t        .::::d:?88888888888?::8b::::.\n\
\t\t     .:::d8888:?88888888??a888888b:::.\n\
\t\t    .:::d8888888a8888888aa8888888888b:::.\n\
\t\t   ::::dP::::::::88888888888::::::::Yb::::\n\
\t\t  ::::dP:::::::::Y888888888P:::::::::Yb::::\n\
\t\t ::::d8:::::::::::Y8888888P:::::::::::8b::::\n\
\t\t.::::88::::::::::::Y88888P::::::::::::88::::.\n\
\t\t:::::Y8baaaaaaaaaa88P:T:Y88aaaaaaaaaad8P:::::\n\
\t\t:::::::Y88888888888P::|::Y88888888888P:::::::\n\
\t\t::::::::::::::::888:::|:::888::::::::::::::::\n\
\t\t`:::::::::::::::8888888888888b::::::::::::::'\n\
\t\t :::::::::::::::88888888888888::::::::::::::\n\
\t\t  :::::::::::::d88888888888888:::::::::::::\n\
\t\t   ::::::::::::88::88::88:::88::::::::::::\n\
\t\t    `::::::::::88::88::88:::88::::::::::'\n\
\t\t      `::::::::88::88::P::::88::::::::'\n\
\t\t        `::::::88::88:::::::88::::::'\n\
\t\t           ``:::::::::::::::::::''\n\
\t\t                ``:::::::::''\n\n\
\t\t                GAME    OVER   !\n\n\
\t\t         YOU    SCORE    IS   <%5d> \n\n\n",score);
        if_reset();  //询问玩家是否重置游戏
    }
    else if (not_over != 0)   //如果未结束的标记不为 0，代表游戏未结束，玩家还可以移动，输出提示信息
    {
        printf("仔细观察哦！");
    }
    else if (win != 0)  //如果获胜的标记不为 0，代表游戏结束，玩家胜利
    {
        save_max();  //保存最高分
        cycle = 0;  //结束按键读取循环
        system("cls"); //清屏
        printf("\t                         YOU    WIN    !!!    \n\
\t |____|____|____|____|____|____|____|____|____|____|____|____|____|\n\
\t ____|____|____|____|____|____|____|____|____|____|____|____|____|\n\
\t __|____|____|____|____|___|_         ____|____|____|____|____|__\n\
\t |____|____|____|____|___|    (\\.-./)  _|____|____|____|___|___|_ \n\
\t ___|____|____|____|____|_  = (^ Y ^) =  _|____|____|____|____|__\n\
\t|____|____|____|____|____|___ /`---`\\ __|____|____|____|____|____|\n\
\t __|____|____|____|____|____|_U___|_U|____|____|____|____|____|___|\n\
\t |____|____|____|____|____|____|____|____|____|____|____|____|____|\n\
\t|____|____|____|____|____|____|____|____|____|____|____|____|____|\n\n\
\t        CONGRATULATIONS     ON     YOUR     VICTORY    !\n\n\n");
        if_reset();  //询问玩家是否重置游戏
    }
}

void if_reset()  //询问玩家是否重置游戏
{
    char ifreset;
    printf("DO    YOU    WANT    RESET    THE    GAME(y/n)?\n");
    scanf("%s",&ifreset);
    if (ifreset == 'y' || ifreset == 'Y')   //玩家输入 y/Y 重置游戏
    {
        resetgame();
    }
    else  //否则就结束游戏
    {
        printf("\nOK  !   GOOD     BYE !");
    }
}


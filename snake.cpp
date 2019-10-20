#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

#define SCREEN_WIDTH  40   /*游戏屏幕宽度*/ 
#define SCREEN_LENGTH 20   /*游戏屏幕长度*/ 
#define START_X 20         /*屏幕起始X坐标*/ 
#define START_Y 2          /*屏幕起始y坐标*/

enum direc{up, down, left, right};  /*蛇的运动方向*///0,1,2,3

typedef struct snake{               /*蛇身结构体*/
 int x;
 int y;
 struct snake *next;
 struct snake *pre;
 struct snake *end;
}SNAKE;

typedef struct{                /*食物*/
 int x;
 int y;
}FOOD;

void HideCursor()         /*隐藏光标，用到了一些CONSOLE API*/
{
 HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE); //从一个特定的标准设备（标准输入、标准输出或标准错误）中取得一个句柄
 CONSOLE_CURSOR_INFO cci;                       //控制台光标的信息
 GetConsoleCursorInfo(hOut, &cci);				//检索有关指定的控制台屏幕缓冲区的光标的可见性和大小信息。
 cci.bVisible = FALSE;							//BOOL  bVisible;// 是否可见
 SetConsoleCursorInfo(hOut, &cci);				//设置控制台光标信息
}

void GotoRood(int x, int y)    /*光标跳跃到某个位置，用了 CONSOLE API*/
{
 HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
 COORD cod;//结构体：表示一个字符在控制台屏幕上的坐标
 cod.X = x;
 cod.Y = y;
 SetConsoleCursorPosition(hOut, cod);
}

int Random(int n)          /*随机生成食物的位置*/
{ 
    srand(time(NULL));    //设置随机数种子
    return (rand() % n);  //由随机数种子产生随机数
}

void BuildSnk(SNAKE *head)  /*构造蛇身*/
{
 SNAKE *p = head;
 while (p != NULL)
 {
    GotoRood(p->x, p->y);
    printf("#");
    p = p->next;
 }
}
void BuildBody(SNAKE *head) 
{
 SNAKE *p = head;
 while (p != NULL)
 {
    GotoRood(p->x, p->y);
    printf("#");
    p = p->next;
 }
}

void RemoveSnk(SNAKE *head)   /*清除函数*/
{
 SNAKE *p = head;
 while (p != NULL)
 {
  GotoRood(p->x, p->y);
  printf(" ");
  p = p->next;
 }
}

void Move(int *d)  //移动方向

{
 char ch;
 while(_kbhit()) //_kbhit()检查当前是否有键盘输入，若有则返回一个非0值，否则返回0
 {
  ch = getch(); //getch() 在执行时,检测按下什么键,如果不按键该函数不返回;是阻塞函数
  if (ch == 72)
  {
    if ((*d == left || *d == right) && *d != down)
      *d = up; //按向上键  修改*d即修改direct
  }
     else if (ch == 80)
        {
       if ((*d == left || *d == right) && *d != up)
        *d = down; //按向下键
        }
     else if (ch == 75)
     {
    if ((*d == up || *d == down) && *d != right) 
       *d = left; //按左键
     }
  else if (ch == 77)
     {
       if ((*d == up || *d == down) && *d != left)
      *d = right; //按右键
    }
 }
}


void IniScreen(SNAKE *head)   /*初始化屏幕*/
{
 int i,j;
 SNAKE *p1, *p2; 
 
 HideCursor();
 GotoRood(START_X,START_Y);

 for (i = 0; i < SCREEN_WIDTH +2; i++)
 {
	 printf("+"); 
 }
   
 for (j = 1; j <= SCREEN_LENGTH; j++)
 {
	 GotoRood(START_X + SCREEN_WIDTH + 1, START_Y + j);
	 printf("|");
 } 

 GotoRood(START_X, START_Y + SCREEN_LENGTH + 1);
 for (i = 0; i < SCREEN_WIDTH + 2; i++)
 {
      printf("+"); 
 }

 GotoRood(START_X-1, START_Y + SCREEN_LENGTH + 2);
// for (i = 0; i < SCREEN_WIDTH + 4; i++)
//     printf("-");
 for (j = 1; j <= SCREEN_LENGTH; j++)
 {
     GotoRood(START_X,START_Y + j);  // 内：|
     printf("|");					 //     |
 }
    
 GotoRood(START_X + 20, START_Y - 1);
 printf("贪吃蛇小游戏"); 
  
 GotoRood(1,START_Y +4);
 printf("******************\n 游戏说明：\n\n ↑设置等级\n\n 按〖回车〗开始\n\n ↑↓←→\n 控制方向\n ******************\n");
 //
 head->x = START_X + SCREEN_WIDTH / 2;
 head->y = START_Y + 8;
 head->pre = NULL;

 p1 = head;
 i = 0;
 while (++i < 3)//构造循环坐标链表 循环了两次
 {
  p2 = p1;
  //构造新节点
  p1 = (SNAKE *)malloc(sizeof(SNAKE)); //新节点 向系统申请一个SNAKE的空间
  p1->x = START_X + SCREEN_WIDTH / 2;  
  p1->y = START_Y + 8 + i;		   
  p1->end = NULL;				  

  p2->next = p1;
  p1->pre = p2; 
 }

 p1->next = NULL;
 head->end = p1;
 BuildBody(head);//画出三段长的蛇
}

void CreatFood(FOOD *fd, SNAKE *snk)  /*做吃的，记得增加一个功能就是让食物不与蛇体冲突*/ 
{

 SNAKE *p = snk;
 int clash = 0;    //标记食物位置是否和蛇发生冲突

 while(1)
 {
   clash = 0;
   fd->x = START_X + 1 + Random(SCREEN_WIDTH);  // x控制在边宽内 
   fd->y = START_Y + 1 + Random(SCREEN_LENGTH);  // y控制在变长内 
   //判断是否与蛇身冲突
   for (;p != NULL ;p = p->next)
   {
     if (fd->x == p->x && fd->y == p->y)
     {
        clash = 1;
        break;//跳出for循环
      }
   }
   if (clash == 0)
   {
	   break;//跳出while循环
   }
 }

 GotoRood(fd->x, fd->y);
 printf("*");
}



void CollisionTest(SNAKE *head)/*判断是否撞墙*/
{

 if (head->x == START_X)  /*一头撞在左墙上.....*/  //head->x = START_X + SCREEN_WIDTH;   /*嗯...那从右边出来吧*/
 {
	BuildSnk(head);
	GotoRood(START_X + (SCREEN_WIDTH-5)/2, START_Y + (SCREEN_LENGTH+2)/2);
	printf("Game Over!\n");
	GotoRood(START_X + (SCREEN_WIDTH-10)/2, START_Y + (SCREEN_LENGTH+4)/2);
	exit(0);
	
 }
  else if (head->x == START_X + SCREEN_WIDTH + 1) /*..换个方向接着撞....*///head->x = START_X + 1;
 {
	BuildSnk(head);
	GotoRood(START_X + (SCREEN_WIDTH-5)/2, START_Y + (SCREEN_LENGTH+2)/2);
	printf("Game Over!\n");
	GotoRood(START_X + (SCREEN_WIDTH-10)/2, START_Y + (SCREEN_LENGTH+4)/2);
	exit(0);
 }
 else if (head->y == START_Y)     /*...再向上撞...*/// head->y = START_Y + SCREEN_LENGTH;
 {
	BuildSnk(head);
	GotoRood(START_X + (SCREEN_WIDTH-5)/2, START_Y + (SCREEN_LENGTH+2)/2);
	printf("Game Over!\n");
	GotoRood(START_X + (SCREEN_WIDTH-10)/2, START_Y + (SCREEN_LENGTH+4)/2);
	exit(0);
 }
 else if (head->y == START_Y + SCREEN_LENGTH + 1)    /*下面也不能放过...*/ //  head->y = START_Y + 1; 
 {
	BuildSnk(head);
	GotoRood(START_X + (SCREEN_WIDTH-5)/2, START_Y + (SCREEN_LENGTH+2)/2);
	printf("Game Over!\n");
	GotoRood(START_X + (SCREEN_WIDTH-10)/2, START_Y + (SCREEN_LENGTH+4)/2);
	exit(0);
 }

}

int Eated(SNAKE *head, FOOD *fd)            /*....有没有吃到食物*/
{
 if (head->x == fd->x && head->y == fd->y)
 {
	 printf("\7");
	 return 1;
 }
    return 0;
}

int GameOver(SNAKE *head)      /*判断游戏结束了没.*/
{
 SNAKE *p;
 for(p = head->next; p != NULL; p = p->next)
 {
  if (head->x == p->x && head->y == p->y)
     return 1;
 }
 return 0;
}

void gameing()   /*游戏过程*/
{
 int i = 0, eat = 0, rank, scorelimit = 0, score = 0;
 int direct = up;   // 初始方向为向上  即direct=up=0
 time_t grade = 1;      //  初始等级为1   time_t实际上就是长整型long int;
 FOOD *fd;
 SNAKE *head, *ptemp;  /*最少3个蛇身 + 1个蛇头的时候才会挂。, *pnew  */

 //构造蛇头
 head = (SNAKE *)malloc(sizeof(SNAKE));
 //构造食物
 fd = (FOOD *)malloc(sizeof(FOOD));

 //写Grade：
 GotoRood(START_X, START_Y - 1);
 printf("Grade: %d", grade);
 //写Score：
 GotoRood(START_X + 10, START_Y - 1);
 printf("Score: %-3d", score);

 //初始化屏幕 并 创造出蛇和食物
 IniScreen(head);
 CreatFood(fd, head);
 //监听键盘 选择难度 getch()：获取你敲得键盘字符
 while(rank = getch())      /*接受按键...↑设置等级【空格】开始游戏*/
 {
  if (rank == 72)//72↑键
  {
    grade = ++grade % 10;
    if (grade == 0)
    {
		grade = 1;
	}  
    GotoRood(START_X, START_Y - 1);
    printf("Grade: %d", grade);
  }
  else if (rank == 13)//回车键 按下它程序向下执行 即程序开始
  { 
	  break;
  } 
 }

 while (1)   
 {
  RemoveSnk(head);             
  ptemp = (SNAKE *)malloc(sizeof(SNAKE));   /*临时的，用来增加在蛇的头部*/
  Move(&direct);   /*....接受用户选择蛇的运动方向*/
  switch(direct)
  {
   case up: ptemp->x = head->x;
            ptemp->y = head->y - 1;
            break;
   case down: ptemp->x = head->x;
              ptemp->y = head->y + 1;
              break;
   case left: ptemp->x = head->x - 1;
              ptemp->y = head->y;
              break;
   case right: ptemp->x = head->x + 1;
               ptemp->y = head->y; 
               break;
  }

  if (!eat)    /*如果没吃到，那自然增加蛇的长度*/
  {
     ptemp->end = head->end->pre;   /*将蛇的尾端位置改变，边为倒数第二个*/
     head->end->pre->next = NULL;  /*再连接下...*///倒数第二个的next=NULL
     free(head->end);     //释放最后一个         
     head->end = NULL;    //让第一个的end=NULL
  }
  else
  {
     ptemp->end = head->end;/*吃到了。。在原有的基础上换一个头，这样就多了一节*/
  }
  head->pre = ptemp;          /*。。队列连接*/
  ptemp->next = head;
  ptemp->pre = NULL;
  head = ptemp;

  if (eat = Eated(head, fd))  
  {
    CreatFood(fd,head);
    score += grade;     /*吃到了，就加分1*/

    //scorelimit += score;   
	scorelimit ++;
	
    if ( 0==scorelimit % 2 && grade < 10)  /*这个用来增加等级。*/
    {                                    /*每过一次10分,等级增加一次，10级是上限..嘿嘿*/
          GotoRood(START_X, START_Y - 1);
          printf("Grade: %d", ++grade);
         // scorelimit = score % 2;
    }

    GotoRood(START_X + 10, START_Y - 1);
    printf("Score: %-3d", score);
  }

  if (GameOver(head))   /*游戏是否结束....*/
   {
         BuildSnk(head);   /*留下最后的英姿。。。吃货该死..*/
         GotoRood(START_X + SCREEN_WIDTH / 2, START_Y + SCREEN_LENGTH / 2);
         puts("Game Over!");
		 GotoRood(START_X + (SCREEN_WIDTH-10)/2, START_Y + (SCREEN_LENGTH+4)/2);
         return;
   }

  CollisionTest(head);	     //碰撞测试
  BuildSnk(head);			 //每循环一次重新画一次蛇
  Sleep((10 - grade) * 50);  //延时...程序会暂停(10 - grade)*50ms 的时间, windows.h里的函数
 }
}

//主函数
int main()
{
 gameing();
 return 0;
}
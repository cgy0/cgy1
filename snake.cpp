#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

#define SCREEN_WIDTH  40   /*��Ϸ��Ļ���*/ 
#define SCREEN_LENGTH 20   /*��Ϸ��Ļ����*/ 
#define START_X 20         /*��Ļ��ʼX����*/ 
#define START_Y 2          /*��Ļ��ʼy����*/

enum direc{up, down, left, right};  /*�ߵ��˶�����*///0,1,2,3

typedef struct snake{               /*����ṹ��*/
 int x;
 int y;
 struct snake *next;
 struct snake *pre;
 struct snake *end;
}SNAKE;

typedef struct{                /*ʳ��*/
 int x;
 int y;
}FOOD;

void HideCursor()         /*���ع�꣬�õ���һЩCONSOLE API*/
{
 HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE); //��һ���ض��ı�׼�豸����׼���롢��׼������׼������ȡ��һ�����
 CONSOLE_CURSOR_INFO cci;                       //����̨������Ϣ
 GetConsoleCursorInfo(hOut, &cci);				//�����й�ָ���Ŀ���̨��Ļ�������Ĺ��Ŀɼ��Ժʹ�С��Ϣ��
 cci.bVisible = FALSE;							//BOOL  bVisible;// �Ƿ�ɼ�
 SetConsoleCursorInfo(hOut, &cci);				//���ÿ���̨�����Ϣ
}

void GotoRood(int x, int y)    /*�����Ծ��ĳ��λ�ã����� CONSOLE API*/
{
 HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
 COORD cod;//�ṹ�壺��ʾһ���ַ��ڿ���̨��Ļ�ϵ�����
 cod.X = x;
 cod.Y = y;
 SetConsoleCursorPosition(hOut, cod);
}

int Random(int n)          /*�������ʳ���λ��*/
{ 
    srand(time(NULL));    //�������������
    return (rand() % n);  //����������Ӳ��������
}

void BuildSnk(SNAKE *head)  /*��������*/
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

void RemoveSnk(SNAKE *head)   /*�������*/
{
 SNAKE *p = head;
 while (p != NULL)
 {
  GotoRood(p->x, p->y);
  printf(" ");
  p = p->next;
 }
}

void Move(int *d)  //�ƶ�����

{
 char ch;
 while(_kbhit()) //_kbhit()��鵱ǰ�Ƿ��м������룬�����򷵻�һ����0ֵ�����򷵻�0
 {
  ch = getch(); //getch() ��ִ��ʱ,��ⰴ��ʲô��,����������ú���������;����������
  if (ch == 72)
  {
    if ((*d == left || *d == right) && *d != down)
      *d = up; //�����ϼ�  �޸�*d���޸�direct
  }
     else if (ch == 80)
        {
       if ((*d == left || *d == right) && *d != up)
        *d = down; //�����¼�
        }
     else if (ch == 75)
     {
    if ((*d == up || *d == down) && *d != right) 
       *d = left; //�����
     }
  else if (ch == 77)
     {
       if ((*d == up || *d == down) && *d != left)
      *d = right; //���Ҽ�
    }
 }
}


void IniScreen(SNAKE *head)   /*��ʼ����Ļ*/
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
     GotoRood(START_X,START_Y + j);  // �ڣ�|
     printf("|");					 //     |
 }
    
 GotoRood(START_X + 20, START_Y - 1);
 printf("̰����С��Ϸ"); 
  
 GotoRood(1,START_Y +4);
 printf("******************\n ��Ϸ˵����\n\n �����õȼ�\n\n �����س�����ʼ\n\n ��������\n ���Ʒ���\n ******************\n");
 //
 head->x = START_X + SCREEN_WIDTH / 2;
 head->y = START_Y + 8;
 head->pre = NULL;

 p1 = head;
 i = 0;
 while (++i < 3)//����ѭ���������� ѭ��������
 {
  p2 = p1;
  //�����½ڵ�
  p1 = (SNAKE *)malloc(sizeof(SNAKE)); //�½ڵ� ��ϵͳ����һ��SNAKE�Ŀռ�
  p1->x = START_X + SCREEN_WIDTH / 2;  
  p1->y = START_Y + 8 + i;		   
  p1->end = NULL;				  

  p2->next = p1;
  p1->pre = p2; 
 }

 p1->next = NULL;
 head->end = p1;
 BuildBody(head);//�������γ�����
}

void CreatFood(FOOD *fd, SNAKE *snk)  /*���Եģ��ǵ�����һ�����ܾ�����ʳ�ﲻ�������ͻ*/ 
{

 SNAKE *p = snk;
 int clash = 0;    //���ʳ��λ���Ƿ���߷�����ͻ

 while(1)
 {
   clash = 0;
   fd->x = START_X + 1 + Random(SCREEN_WIDTH);  // x�����ڱ߿��� 
   fd->y = START_Y + 1 + Random(SCREEN_LENGTH);  // y�����ڱ䳤�� 
   //�ж��Ƿ��������ͻ
   for (;p != NULL ;p = p->next)
   {
     if (fd->x == p->x && fd->y == p->y)
     {
        clash = 1;
        break;//����forѭ��
      }
   }
   if (clash == 0)
   {
	   break;//����whileѭ��
   }
 }

 GotoRood(fd->x, fd->y);
 printf("*");
}



void CollisionTest(SNAKE *head)/*�ж��Ƿ�ײǽ*/
{

 if (head->x == START_X)  /*һͷײ����ǽ��.....*/  //head->x = START_X + SCREEN_WIDTH;   /*��...�Ǵ��ұ߳�����*/
 {
	BuildSnk(head);
	GotoRood(START_X + (SCREEN_WIDTH-5)/2, START_Y + (SCREEN_LENGTH+2)/2);
	printf("Game Over!\n");
	GotoRood(START_X + (SCREEN_WIDTH-10)/2, START_Y + (SCREEN_LENGTH+4)/2);
	exit(0);
	
 }
  else if (head->x == START_X + SCREEN_WIDTH + 1) /*..�����������ײ....*///head->x = START_X + 1;
 {
	BuildSnk(head);
	GotoRood(START_X + (SCREEN_WIDTH-5)/2, START_Y + (SCREEN_LENGTH+2)/2);
	printf("Game Over!\n");
	GotoRood(START_X + (SCREEN_WIDTH-10)/2, START_Y + (SCREEN_LENGTH+4)/2);
	exit(0);
 }
 else if (head->y == START_Y)     /*...������ײ...*/// head->y = START_Y + SCREEN_LENGTH;
 {
	BuildSnk(head);
	GotoRood(START_X + (SCREEN_WIDTH-5)/2, START_Y + (SCREEN_LENGTH+2)/2);
	printf("Game Over!\n");
	GotoRood(START_X + (SCREEN_WIDTH-10)/2, START_Y + (SCREEN_LENGTH+4)/2);
	exit(0);
 }
 else if (head->y == START_Y + SCREEN_LENGTH + 1)    /*����Ҳ���ܷŹ�...*/ //  head->y = START_Y + 1; 
 {
	BuildSnk(head);
	GotoRood(START_X + (SCREEN_WIDTH-5)/2, START_Y + (SCREEN_LENGTH+2)/2);
	printf("Game Over!\n");
	GotoRood(START_X + (SCREEN_WIDTH-10)/2, START_Y + (SCREEN_LENGTH+4)/2);
	exit(0);
 }

}

int Eated(SNAKE *head, FOOD *fd)            /*....��û�гԵ�ʳ��*/
{
 if (head->x == fd->x && head->y == fd->y)
 {
	 printf("\7");
	 return 1;
 }
    return 0;
}

int GameOver(SNAKE *head)      /*�ж���Ϸ������û.*/
{
 SNAKE *p;
 for(p = head->next; p != NULL; p = p->next)
 {
  if (head->x == p->x && head->y == p->y)
     return 1;
 }
 return 0;
}

void gameing()   /*��Ϸ����*/
{
 int i = 0, eat = 0, rank, scorelimit = 0, score = 0;
 int direct = up;   // ��ʼ����Ϊ����  ��direct=up=0
 time_t grade = 1;      //  ��ʼ�ȼ�Ϊ1   time_tʵ���Ͼ��ǳ�����long int;
 FOOD *fd;
 SNAKE *head, *ptemp;  /*����3������ + 1����ͷ��ʱ��Ż�ҡ�, *pnew  */

 //������ͷ
 head = (SNAKE *)malloc(sizeof(SNAKE));
 //����ʳ��
 fd = (FOOD *)malloc(sizeof(FOOD));

 //дGrade��
 GotoRood(START_X, START_Y - 1);
 printf("Grade: %d", grade);
 //дScore��
 GotoRood(START_X + 10, START_Y - 1);
 printf("Score: %-3d", score);

 //��ʼ����Ļ �� ������ߺ�ʳ��
 IniScreen(head);
 CreatFood(fd, head);
 //�������� ѡ���Ѷ� getch()����ȡ���õü����ַ�
 while(rank = getch())      /*���ܰ���...�����õȼ����ո񡿿�ʼ��Ϸ*/
 {
  if (rank == 72)//72����
  {
    grade = ++grade % 10;
    if (grade == 0)
    {
		grade = 1;
	}  
    GotoRood(START_X, START_Y - 1);
    printf("Grade: %d", grade);
  }
  else if (rank == 13)//�س��� ��������������ִ�� ������ʼ
  { 
	  break;
  } 
 }

 while (1)   
 {
  RemoveSnk(head);             
  ptemp = (SNAKE *)malloc(sizeof(SNAKE));   /*��ʱ�ģ������������ߵ�ͷ��*/
  Move(&direct);   /*....�����û�ѡ���ߵ��˶�����*/
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

  if (!eat)    /*���û�Ե�������Ȼ�����ߵĳ���*/
  {
     ptemp->end = head->end->pre;   /*���ߵ�β��λ�øı䣬��Ϊ�����ڶ���*/
     head->end->pre->next = NULL;  /*��������...*///�����ڶ�����next=NULL
     free(head->end);     //�ͷ����һ��         
     head->end = NULL;    //�õ�һ����end=NULL
  }
  else
  {
     ptemp->end = head->end;/*�Ե��ˡ�����ԭ�еĻ����ϻ�һ��ͷ�������Ͷ���һ��*/
  }
  head->pre = ptemp;          /*������������*/
  ptemp->next = head;
  ptemp->pre = NULL;
  head = ptemp;

  if (eat = Eated(head, fd))  
  {
    CreatFood(fd,head);
    score += grade;     /*�Ե��ˣ��ͼӷ�1*/

    //scorelimit += score;   
	scorelimit ++;
	
    if ( 0==scorelimit % 2 && grade < 10)  /*����������ӵȼ���*/
    {                                    /*ÿ��һ��10��,�ȼ�����һ�Σ�10��������..�ٺ�*/
          GotoRood(START_X, START_Y - 1);
          printf("Grade: %d", ++grade);
         // scorelimit = score % 2;
    }

    GotoRood(START_X + 10, START_Y - 1);
    printf("Score: %-3d", score);
  }

  if (GameOver(head))   /*��Ϸ�Ƿ����....*/
   {
         BuildSnk(head);   /*��������Ӣ�ˡ������Ի�����..*/
         GotoRood(START_X + SCREEN_WIDTH / 2, START_Y + SCREEN_LENGTH / 2);
         puts("Game Over!");
		 GotoRood(START_X + (SCREEN_WIDTH-10)/2, START_Y + (SCREEN_LENGTH+4)/2);
         return;
   }

  CollisionTest(head);	     //��ײ����
  BuildSnk(head);			 //ÿѭ��һ�����»�һ����
  Sleep((10 - grade) * 50);  //��ʱ...�������ͣ(10 - grade)*50ms ��ʱ��, windows.h��ĺ���
 }
}

//������
int main()
{
 gameing();
 return 0;
}
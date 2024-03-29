


//TIC TAC TOE
//designed by ZhouSheng 2018.5    

#include "pch.h"
#include <stdio.h>
#include <stdlib.h>
#define draw 0
#define win 100
#define man 1   //人
#define com -1     //计算机
#define max 1000  
int board[3][3];  //棋盘数组 ,用0,1，-1表示
char print[3][3];  //输出数组，用X和O 表示
int now_depth;      //当前深度
int player;           //当前玩家 
typedef struct bestpoint   //用于存储棋盘中空位置的坐标
{
	int bestcol;
	int bestrow;
}best;
best chessbest;        //存储最佳落子位置
void initialise()//初始化数组
{
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
		{
			board[i][j] = 0;
		}
}
void ergodic()     //遍历棋盘，1为X，-1为O,用于输出X和0
{
	int i, j;
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
		{
			if (board[i][j] == 1)
				print[i][j] = 'X';
			else if (board[i][j] == -1)
				print[i][j] = 'O';
			else
				print[i][j] = ' ';
		}
	}
}
void display()   //显示棋盘
{
	ergodic();
	for (int i = 0; i < 3; i++)
	{
		printf(" %c | %c | %c \n", print[i][0], print[i][1], print[i][2]);
		if (i < 2)
			printf("---|---|---\n");
	}

}
int judge_win()  //判断输赢
{
	int i, j;
	for (i = 0; i < 3; i++)
	{
		if (board[i][0] == 1 && board[i][1] == 1 && board[i][2] == 1)
			return man;
		else if (board[i][0] == -1 && board[i][1] == -1 && board[i][2] == -1)
			return com;
	}
	for (j = 0; j < 3; j++)
	{
		if (board[0][j] == 1 && board[1][j] == 1 && board[2][j] == 1)
			return man;
		else if (board[0][j] == -1 && board[1][j] == -1 && board[2][j] == -1)
			return com;
	}
	if (board[0][0] == 1 && board[1][1] == 1 && board[2][2] == 1)
		return man;
	else if (board[0][2] == 1 && board[1][1] == 1 && board[2][0] == 1)
		return man;
	else if (board[0][0] == -1 && board[1][1] == -1 && board[2][2] == -1)
		return com;
	else if (board[0][2] == -1 && board[1][1] == -1 && board[2][0] == -1)
		return com;
	else
		return 0;
}
void change(best i)     //改变空棋盘的数值,用于进行极大极小的向下遍历搜索
{
	board[i.bestcol][i.bestrow] = player;
	player = (player == com) ? man : com;
}
int location(best a[])         //返回空位置的数量
{
	int i, j;
	int count = 0;
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
		{
			if (board[i][j] == 0)
			{
				a[count].bestcol = i;
				a[count].bestrow = j;
				count++;
			}
		}
	}
	return count;
}
void restore(best i)  //还原
{
	board[i.bestcol][i.bestrow] = 0;
	player = (player == com) ? man : com;
}
int value()          //计算机方对棋盘局面的估值
{
	if (judge_win() == com)
		return max;             //返回极大值
	else if (judge_win() == man)
		return -max;           //返回极小值
	else
		return 0;
}

int maxmini(int deep)    //极大极小算法，计算机方为极大，玩家方为极小
{
	int i;
	int weight;                //权值
	int best_value = 0;
	int count = 0;
	best a[9];
	best best1;
	if (man == judge_win() || com == judge_win())
		return value();             //如果遍历到，某一层后出现了赢的局面，则直接返回相应的极大极小值
	if (deep == 0)
		return value();        //如果全部遍历完成，即深度为0，则返回相应的估值
	if (man == player)
		best_value = max;    //给玩家一方赋最大值，以便于找到最小的估值
	else if (com == player)
		best_value = -max;   //给计算机一方赋最小值，以便找到最大的估值
	count = location(a);     //得到棋盘中有多少个空位置
	for (i = 0; i < count; i++)
	{
		best1 = a[i];
		change(best1);                  //改变空棋盘位置的值
		weight = maxmini(deep - 1);   //递归，每次递归深度减1
		restore(best1);               //还原被改变的值
		if (player == man)
		{
			if (weight < best_value)  //给玩家方找到极小值
			{
				best_value = weight;
				if (deep == now_depth)
					chessbest = best1;
			}
		}
		else if (player == com)
		{
			if (weight > best_value)        //给计算机方找到极大值
			{
				best_value = weight;
				if (deep == now_depth)
					chessbest = best1;         //找到最佳落子位置
			}

		}
	}
	return best_value;
}
void man_input()
{
	int i, j;
	scanf_s("%d%d", &i, &j);
	while (board[i - 1][j - 1])
	{
		printf("您输入的位置已有棋子,请重新输入:\n");
		scanf_s("%d%d", &i, &j);
	}
	board[i - 1][j - 1] = man;
	display();
}
int at_once()  //判断计算机是否连个连成一条线，当计算机已有两个连成一线时，并且这一线无玩家棋子，直接选择这一线的那个点使计算机获胜
{
	int i, j;
	int num1 = 0, num2 = 0;
	for (i = 0; i < 3; i++)
	{
		if (board[i][i] == com)
		{
			num1++;
		}
		for (j = 0; j < 3; j++)
		{
			if ((board[i][0] == com && board[i][1] == com) || (board[i][0] == com && board[i][2] == com) || (board[i][1] == com & board[i][2] == com))
			{
				if (board[i][j] == 0)
				{
					board[i][j] = com;
					printf("计算机选择的位置:(%d,%d)\n", i + 1, j + 1);
					return 1;
				}
			}
			else if (board[0][i] == com && board[1][i] == com || board[0][i] == com && board[2][i] == com || board[1][i] == com & board[2][i] == com)
			{
				if (board[j][i] == 0)
				{
					board[j][i] = com;
					printf("计算机选择的位置:(%d,%d)\n", j, i);
					return 1;
				}
			}
			else if (i + j == 2)
			{
				if (board[i][j] == com)
					num2++;
			}
		}
	}
	for (i = 0; i < 3; i++)
	{
		if (num1 == 2 && board[i][i] == 0)
		{
			board[i][i] = com;
			printf("计算机选择的位置:(%d,%d)\n", j + 1, j + 1);
			return 1;
		}
		for (j = 0; j < 3; j++)
		{
			if (i + j == 2 && num2 == 2)
				if (board[i][j] == 0)
				{
					board[i][j] = com;
					printf("计算机选择的位置:(%d,%d)\n", i + 1, j + 1);
					return 1;
				}
		}
	}
	return 0;
}
void com_input() //计算机落子
{
	if (at_once() == 0)
	{
		maxmini(now_depth); //调用极大极小算法找到位置
		board[chessbest.bestcol][chessbest.bestrow] = com;
		printf("计算机选择的位置:(%d,%d)\n", chessbest.bestcol + 1, chessbest.bestrow + 1);
	}
	display();
}
void man_game()//人先手
{
	printf("井字棋\n");
	printf("X:玩家\nO:计算机\n\n");
	int step = 1;
	now_depth = 9;  //深度最大为9，每遍历一层减小1
	initialise();
	display();
	player = man;
	while (step <= 9)
	{
		printf("\n请输入棋子位置:\n范围：(1,1)-（3,3）\n");
		man_input();
		printf("\n");
		if (man == judge_win())
		{
			printf("\n玩家获胜!\n");
			break;
		}
		step++;
		now_depth--;
		if (step == 10)
		{
			printf("\n平局!\n");
			break;
		}
		player = com;  //转换当前玩家
		com_input();
		if (com == judge_win())
		{
			printf("\n计算机获胜！\n");
			break;
		}
		step++;
		now_depth--;
		player = (player == com) ? man : com;    //转换当前玩家
	}
}
void com_game()//计算机先手
{
	printf("井字棋\n");
	printf("X:玩家\nO:计算机\n\n");
	int step = 1;
	now_depth = 9;  //深度最大为9，每遍历一层减小1
	initialise();
	player = com;
	while (step <= 9)
	{
		com_input();
		printf("\n");
		if (com == judge_win())
		{
			printf("\n计算机获胜!\n");
			break;
		}
		step++;
		now_depth--;
		if (step == 10)
		{
			printf("\n平局!\n");
			break;
		}
		player = man;  //转换当前玩家
		printf("\n请输入棋子位置:\n范围：(1 1)-（3 3）\n");
		man_input();
		if (man == judge_win())
		{
			printf("\n玩家获胜！\n");
			break;
		}
		step++;
		now_depth--;
		player = com;    //转换当前玩家
	}
}
int main()
{
	while (1)
	{
		char point;
		int n;
		printf("请输入人先手还是计算机先手:(1.人，2.计算机)\n");
		scanf_s("%d", &n);
		if (n == 1)
			man_game();
		if (n == 2)
			com_game();
		printf("\n");
		printf("再来一局？ y/n\n");
		scanf_s(" %c", &point);
		system("cls");   //清屏
		if (point == 'y')
			continue;
		else if (point == 'n')
			exit(0);
	}
	return 0;
}

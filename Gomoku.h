#ifndef WZQ_H
#define WZQ_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define SIZE 15
#define CHARSIZE 2
#define BLACK 1
#define WHITE 2
#define EMPTY 0
#define END 1
#define GOON 0
#define INBOARD(x) ((x) >= 0 && (x) < SIZE) // 判断是否在棋盘内
#define INFINITY 1000000000000000000        // 无穷大
// 用于作为mango函数的返回标记
enum
{
    QUIT,
    BACK,
    WRONG,
    LOWER,
    UPPER,
    DONE
};
// 用于索引分数数组
enum
{
    FIVEROW,
    OPENFOUR,
    HALFOPENFOUR,
    OPENTHREE,
    HALFOPENTHREE,
    OPENTWO,
    HALFOPENTWO,
    OPENONE,
    HALFOPENONE,
    SLEEP,
    DEAD,
};

void initRecordBoard(void);
void innerLayoutToDisplayArray(void);
void displayBoard(void);
int getinput(char s[]);
int checkwin();
int checkForbiddenMoves(int x, int y);
int isempty(int x, int y);
int iscurpiece(int x, int y, int curpiece);
int isalone(int x, int y);
int ManGo();
void AiGo();
void findbestmove(int *bestX, int *bestY);
long long int minimax(int depth, long long int alpha, long long int beta, int player, long long int lastscore);
long long int evaluate(int i, int j);

extern int arrayForInnerBoardLayout[SIZE][SIZE], row, col, sign;
extern int manpiece, aipiece, maxdepth;
extern int direction[4][2];
extern char input[6];

#endif
#include "Gomoku.h"

// 棋盘使用的是GBK编码，每一个中文字符占用2个字节。

// 空棋盘模板
char arrayForEmptyBoard[SIZE][SIZE * CHARSIZE + 1] = //+1存\0
    {
        "┏┯┯┯┯┯┯┯┯┯┯┯┯┯┓",
        "┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
        "┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
        "┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
        "┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
        "┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
        "┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
        "┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
        "┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
        "┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
        "┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
        "┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
        "┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
        "┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
        "┗┷┷┷┷┷┷┷┷┷┷┷┷┷┛"};
// 此数组存储用于显示的棋盘
char arrayForDisplayBoard[SIZE][SIZE * CHARSIZE + 1];

char play1Pic[] = "●"; // 黑棋子;
char play1CurrentPic[] = "▲";

char play2Pic[] = "◎"; // 白棋子;
char play2CurrentPic[] = "△";

// 此数组用于记录当前的棋盘的格局
int arrayForInnerBoardLayout[SIZE][SIZE];

int row = 0, col = 0, sign = 1; // row,col为数组真正索引的行和列，也是最终落子的位置
// sign用于标记当前为黑棋子还是白棋，sign=-1为白棋，+1为黑棋

char input[6]; // 用于读取用户输入

int manpiece = BLACK;
int aipiece = WHITE; // 定义玩家和电脑的棋子
int maxdepth = 3;    //

int main()

{
    int mode = 0, firstplayer = 0;
    // mode为1表示人机对战，mode为2表示人人对战
    // firstplayer为1表示电脑先手，firstplayer为2表示玩家先手

    // 读取用户选择的模式
    while (1)
    {
        printf("人机对战，请输入1\n");
        printf("人人对战，请输入2：\n");
        scanf("%d", &mode);
        getchar();
        if (mode == 1 || mode == 2)
            break; // 输入正确，退出循环
        else
        {
            printf("输入错误，请重新输入！\n");
            continue;
            // 输入有误，重新输入
        }
    }
    initRecordBoard(); // 初始化一个空棋盘并显示
    innerLayoutToDisplayArray();
    displayBoard();

    int isful = 0;               // 判断是否填满棋盘（下了多少步）
    sign = 1;                    // 先手黑棋
    int state = 0;               // 判断下棋过程中的情况
    int history[SIZE * SIZE][2]; // 用于记录历史走法，最多记录225步
    int current_piece;           // 当前下棋的棋子

    // 人机对战
    if (mode == 1)
    {
        while (1)
        {
            // 读取先下棋的阵营
            printf("电脑先手，请输入1\n");
            printf("玩家先手，请输入2：\n");
            scanf("%d", &firstplayer);
            getchar();
            if (firstplayer == 1 || firstplayer == 2)
                break; // 输入正确，退出循环
            else
            {
                printf("输入错误，请重新输入！\n");
                continue; // 输入有误，重新输入
            }
        }
        // 根据读取的先手阵营，设置manpiece和aipiece
        manpiece = (firstplayer == 1) ? WHITE : BLACK;
        aipiece = (firstplayer == 2) ? WHITE : BLACK;
        while (1)
        {
            // 设置当前下棋的棋子
            current_piece = (sign > 0) ? BLACK : WHITE;

            if (current_piece == manpiece)
            {
                // 人类下棋
                state = ManGo();
                // 输入q退出
                if (state == QUIT)
                    return 0;
                // 输入有误，重新输入
                else if (state == WRONG || state == LOWER || state == UPPER)
                    continue;
                // 输入r悔棋
                else if (state == BACK)
                {
                    if (isful > 1)
                    {
                        // 悔棋回退两步，玩家一步，电脑一步
                        // 玩家回退
                        isful--;
                        row = history[isful][0];
                        col = history[isful][1];
                        arrayForInnerBoardLayout[row][col] = EMPTY;
                        // 电脑回退
                        isful--;
                        row = history[isful][0];
                        col = history[isful][1];
                        arrayForInnerBoardLayout[row][col] = EMPTY;
                        innerLayoutToDisplayArray(); // 重新显示悔棋后的棋盘
                        displayBoard();
                        printf("\n悔棋成功！\n"); // 提示用户
                        continue;
                    }
                    else
                    {
                        // 处理失败情况，比如无棋子可悔
                        printf("悔棋失败！\n");
                        continue;
                    }
                }
            }
            else
                AiGo(); // 电脑下棋

            // 显示下棋后的棋盘
            innerLayoutToDisplayArray();
            displayBoard();

            // 显示电脑下棋的位置
            if (current_piece == aipiece)
                printf("电脑下棋：%d %c\n", SIZE - row, 'A' + col);

            // 判断胜负
            if (checkwin())
            {
                printf("%s《%s》胜利!!!!!\n", (current_piece == aipiece) ? "电脑" : "玩家", sign > 0 ? "黑方" : "白方");
                getinput(input);
                if (input[0] != 'r')
                    return 0;
            }
            // 判断禁手
            if (sign > 0 && checkForbiddenMoves(row, col))
            {
                printf("触犯禁手，黑方负!!!!!\n如果是误判，请输入pass忽略本次判断\n");
                // 输入pass忽略本次判断
                getinput(input);
                if (input[0] != 'p')
                    return 0;
                printf("游戏继续!!!\n");
            }
            // 记录当前走法到历史记录中
            history[isful][0] = row;
            history[isful][1] = col;
            // 已下的棋子数加1
            isful++;
            // 判断平局
            if (isful >= SIZE * SIZE)
            {
                printf("平局！！！\n");
                return 0;
            }
            // 切换下一轮的棋子
            sign *= -1;
        }
    }
    // 人人对战
    else if (mode == 2)
    {
        while (1)
        {
            manpiece = (sign > 0) ? BLACK : WHITE;
            state = ManGo(); // 人类下棋
            if (state == QUIT)
                return 0;
            else if (state == WRONG)
                continue;
            else if (state == BACK)
            {
                // 悔棋
                if (isful > 1)
                {
                    // 回退己方棋子
                    isful--;
                    row = history[isful][0];
                    col = history[isful][1];
                    arrayForInnerBoardLayout[row][col] = EMPTY;
                    // 回退对方棋子
                    isful--;
                    row = history[isful][0];
                    col = history[isful][1];
                    arrayForInnerBoardLayout[row][col] = EMPTY;
                    innerLayoutToDisplayArray(); // 显示悔棋后的棋盘
                    displayBoard();
                    printf("\n悔棋成功！\n");
                    continue;
                }
                else
                {
                    // 处理失败情况，比如无棋子可悔
                    printf("悔棋失败！\n");
                    continue;
                }
            }

            innerLayoutToDisplayArray(); // 显示下棋后棋盘
            displayBoard();

            // 判断胜负
            if (checkwin())
            {
                printf("《%s》胜利!!!!!\n", sign > 0 ? "黑方" : "白方");
                getinput(input);
                if (input[0] != 'r')
                    return 0;
            }
            // 判断禁手
            if (sign > 0 && checkForbiddenMoves(row, col))
            {
                printf("触犯禁手，黑方负!!!!!\n如果是误判，请输入pass忽略本次判断\n");
                getinput(input);
                if (input[0] != 'p')
                    return 0;
                printf("游戏继续!!!\n");
            }
            // 记录当前走法到历史记录中
            history[isful][0] = row;
            history[isful][1] = col;
            // 已下的棋子数加1
            isful++;
            // 判断平局
            if (isful >= SIZE * SIZE)
            {
                printf("平局！！！\n");
                return 0;
            }
            sign *= -1; // 切换下一轮的棋子
        }
    }

    return 0;
}
// 初始化一个空棋盘格局
void initRecordBoard(void)
{
    // 通过双重循环，将arrayForInnerBoardLayout清0
    int i, j;
    for (i = 0; i < SIZE; i++)
        for (j = 0; j < SIZE; j++)
            arrayForInnerBoardLayout[i][j] = EMPTY;
    return;
}

// 将arrayForInnerBoardLayout中记录的棋子位置，转化到arrayForDisplayBoard中
void innerLayoutToDisplayArray(void)
{
    // 第一步：将arrayForEmptyBoard中记录的空棋盘，复制到arrayForDisplayBoard中
    int i, j;
    for (i = 0; i < SIZE; i++)
        for (j = 0; j < SIZE * CHARSIZE + 1; j++)
            arrayForDisplayBoard[i][j] = arrayForEmptyBoard[i][j];

    // 第二步：扫描arrayForInnerBoardLayout，当遇到非EMPTY的元素，将●或者◎复制到arrayForDisplayBoard的相应位置上
    for (i = 0; i < SIZE; i++)
        for (j = 0; j < SIZE; j++)
        {
            if (arrayForInnerBoardLayout[i][j] == BLACK)
            {
                // 黑棋显示
                arrayForDisplayBoard[i][j * CHARSIZE] = play1Pic[0];
                arrayForDisplayBoard[i][j * CHARSIZE + 1] = play1Pic[1];
                if (CHARSIZE == 3)
                    arrayForDisplayBoard[i][j * CHARSIZE + 2] = play1Pic[2];
            }
            else if (arrayForInnerBoardLayout[i][j] == WHITE)
            {
                // 白棋显示
                arrayForDisplayBoard[i][j * CHARSIZE] = play2Pic[0];
                arrayForDisplayBoard[i][j * CHARSIZE + 1] = play2Pic[1];
                if (CHARSIZE == 3)
                    arrayForDisplayBoard[i][j * CHARSIZE + 3] = play2Pic[2];
            }
        }
    // 第三步：把最新的棋子换成△或▲
    i = row;
    j = col;
    if (arrayForInnerBoardLayout[i][j] == BLACK)
    {
        arrayForDisplayBoard[i][j * CHARSIZE] = play1CurrentPic[0];
        arrayForDisplayBoard[i][j * CHARSIZE + 1] = play1CurrentPic[1];
        if (CHARSIZE == 3)
            arrayForDisplayBoard[i][j * CHARSIZE + 3] = play1CurrentPic[3];
    }
    else if (arrayForInnerBoardLayout[i][j] == WHITE)
    {
        arrayForDisplayBoard[i][j * CHARSIZE] = play2CurrentPic[0];
        arrayForDisplayBoard[i][j * CHARSIZE + 1] = play2CurrentPic[1];
        if (CHARSIZE == 3)
            arrayForDisplayBoard[i][j * CHARSIZE + 3] = play2CurrentPic[3];
    }
}

// 显示棋盘格局
void displayBoard(void)
{
    int i;
    // 第一步：清屏
    system("clear"); // 清屏
    // 第二步：将arrayForDisplayBoard输出到屏幕上
    printf("Work of 周正凯\n");
    for (i = 0; i < SIZE; i++)
        printf("%3d %s\n", SIZE - i, arrayForDisplayBoard[i]);

    // 第三步：输出最下面的一行字母A B ....
    char c = 'A';
    printf("    ");
    for (i = 0; i < SIZE; i++)
    {
        printf("%2c", c);
        c++;
    }
    printf("\n");
}

// 读取用户输入
int getinput(char v[])
{
    int i = 0;
    char c;
    // 读取用户输入，直到遇到换行符或输入5个字符
    while ((c = getchar()) != EOF && c != '\n' && i < 5)
        if (isalnum(c))
            v[i++] = c; // 遇到字母或数字，存入数组
    v[i] = '\0';
    return strlen(v); // 返回输入的字符数
}
#include "Gomoku.h"
// ai下棋
void AiGo()
{
    int kill();
    printf("电脑《%s》下棋中……\n", aipiece == BLACK ? "黑方" : "白方");
    // 寻找最佳走法
    findbestmove(&row, &col);
    arrayForInnerBoardLayout[row][col] = aipiece; // 落子
    return;
}
// 开局定式
int firstmove()
{
    if (aipiece == BLACK && arrayForInnerBoardLayout[SIZE / 2][SIZE / 2] == EMPTY)
    {
        row = SIZE / 2, col = SIZE / 2;
        return 1;
    }
    else if (aipiece == WHITE)
    {
        row = row + ((row < SIZE / 2) ? 1 : -1);
        col = col + ((col < SIZE / 2) ? 1 : -1);
        return 1;
    }
    return 0;
}
// 保险起见，优先判断五连，先判断自己，再判断对手
int kill()
{
    int i1, j1, i2, j2, count = 0;
    int dir;                    // 方向索引
    int currentpiece = aipiece; // 先判断自己
    int twotimes = 2;           // 一共判断两次，一次是自己，一次是对手
    long long int score, maxscore = -INFINITY, beta = INFINITY, alpha = -INFINITY;
    int maxdepth = 3; // 最大搜索深度
    int bestX, bestY;
    int state = 0, aistate = 0; // 标记有没有找到活四和己方的活四
    while (twotimes--)
    {
        // 遍历每个方向
        for (row = 0; row < SIZE; row++)
            for (col = 0; col < SIZE; col++)
                if (arrayForInnerBoardLayout[row][col] == EMPTY && !isalone(row, col))
                {
                    arrayForInnerBoardLayout[row][col] = currentpiece;
                    for (dir = 0; dir < 4; dir++)
                    {
                        count = 0;
                        // 向一个方向移动并计数
                        for (i1 = row, j1 = col; iscurpiece(i1, j1, currentpiece);)
                        {
                            i1 += direction[dir][0];
                            j1 += direction[dir][1];
                            count++;
                        }
                        // 向相反方向移动并计数
                        for (i2 = row, j2 = col; iscurpiece(i2, j2, currentpiece);)
                        {
                            i2 -= direction[dir][0];
                            j2 -= direction[dir][1];
                            count++;
                        }
                        count--; // 初始位置被计了两次

                        // 如果计数为5，则返回获胜标志，要成五的地方一定要下
                        if (count == 5)
                        {
                            arrayForInnerBoardLayout[row][col] = EMPTY;
                            return 1;
                        }
                        // 判断活四，优先判断己方，优先成己方活四
                        if (count == 4 && !(currentpiece == manpiece && aistate == 1))
                        {
                            if (isempty(i1, j1) && isempty(i2, j2))
                            {
                                if (currentpiece == aipiece)
                                {
                                    aistate = 1; // 找到了己方活四
                                    state = 1;
                                    bestX = row;
                                    bestY = col;
                                }
                                else
                                {
                                    state = 1;
                                    // 通过评分比较来确定最佳的堵活四的位置
                                    score = -minimax(maxdepth, beta, alpha, 3 - currentpiece, 1.000001 * evaluate(row, col));
                                    if (score > maxscore)
                                    {
                                        maxscore = score;
                                        beta = -score;
                                        bestX = row;
                                        bestY = col;
                                    }
                                    arrayForInnerBoardLayout[row][col] = EMPTY;
                                    arrayForInnerBoardLayout[i1][j1] = currentpiece;
                                    score = -minimax(maxdepth, beta, alpha, 3 - currentpiece, 1.000001 * evaluate(i1, j1));
                                    if (score > maxscore)
                                    {
                                        maxscore = score;
                                        beta = -score;
                                        bestX = i1;
                                        bestY = j1;
                                    }
                                    arrayForInnerBoardLayout[i1][j1] = EMPTY;
                                    arrayForInnerBoardLayout[i2][j2] = currentpiece;
                                    score = -minimax(maxdepth, beta, alpha, 3 - currentpiece, 1.000001 * evaluate(i2, j2));
                                    if (score > maxscore)
                                    {
                                        maxscore = score;
                                        beta = -score;
                                        bestX = i2;
                                        bestY = j2;
                                    }
                                    arrayForInnerBoardLayout[i2][j2] = EMPTY;
                                }
                            }
                        }
                    }
                    arrayForInnerBoardLayout[row][col] = EMPTY;
                }
        // 切换对手
        currentpiece = manpiece;
    }
    if (state)
    {
        // 找到了活四，返回要下的位置
        row = bestX;
        col = bestY;
        return 1;
    }
    return 0;
}

// 寻找最佳走法，并返回到bestX, bestY中
void findbestmove(int *bestX, int *bestY)
{
    int i, j;
    static int start = 1; // 开局标志
    long long score, maxscore = -INFINITY, beta = INFINITY, alpha = -INFINITY;
    double weight = 1.0000001; // 权重系数，用于平衡不同层次的分数
    // 开局定式
    if (start)
    {
        firstmove();
        start = 0;
        return;
    }
    // 先算杀
    if (kill())
    {
        arrayForInnerBoardLayout[row][col] = aipiece;
        return;
    }

    // 遍历所有可能的落子位置
    for (i = 0; i < SIZE; i++)
        for (j = 0; j < SIZE; j++)
        {
            if (arrayForInnerBoardLayout[i][j] == EMPTY && !isalone(i, j))
            {
                // 模拟落一子
                arrayForInnerBoardLayout[i][j] = aipiece;
                // 会触犯禁手的位置跳过
                if (aipiece == BLACK && checkForbiddenMoves(i, j))
                {
                    arrayForInnerBoardLayout[i][j] = EMPTY;
                    continue;
                }
                // 获取多层分数
                score = minimax(maxdepth, beta, alpha, manpiece, weight * evaluate(i, j));
                // 撤销落子
                arrayForInnerBoardLayout[i][j] = EMPTY;
                // 记录最佳分数和位置
                if (score > maxscore)
                {
                    maxscore = score;
                    alpha = score; // 及时更新alpha界
                    // 传出最佳位置
                    *bestX = i;
                    *bestY = j;
                }
            }
        }
    return;
}
// 最小最大搜索，返回分数
long long int minimax(int depth, long long int beta, long long int alpha, int curpiece, long long int lastscore)
{
    long long score = 0, layerscore = 0;
    int i, j;
    double weight = 1.00000001; // 权重系数，用于平衡不同层次的分数
    // 最终层直接返回分数
    if (depth == 0)
        return lastscore;
    // max层，要返回最大分数
    if (curpiece == aipiece)
    {
        long long int maxscore = -INFINITY; // 初始化最大分数为无穷小
        for (i = 0; i < SIZE; i++)
            for (j = 0; j < SIZE; j++)
            {
                // 检查当前棋盘位置是否为空且不孤立
                if (arrayForInnerBoardLayout[i][j] == EMPTY && !isalone(i, j))
                {
                    // 模拟落子
                    arrayForInnerBoardLayout[i][j] = curpiece;
                    // 计算这一层该点的分数，然后递归地传到下一层
                    layerscore = evaluate(i, j) + lastscore;
                    // 得出总分
                    score = minimax(depth - 1, beta, alpha, manpiece, weight * layerscore);
                    // 撤销落子
                    arrayForInnerBoardLayout[i][j] = EMPTY;
                    // 记录最大分数并及时更新alpha界
                    if (score > maxscore)
                    {
                        maxscore = score;
                        alpha = score;
                    }
                    // beta剪枝
                    if (score >= beta)
                        return maxscore;
                }
            }
        return maxscore;
    }
    // min层
    if (curpiece == manpiece)
    {
        long long int minscore = INFINITY; // 初始化最小分数为无穷大
        for (i = 0; i < SIZE; i++)
            for (j = 0; j < SIZE; j++)
            {
                // 检查当前棋盘位置是否为空且不孤立
                if (arrayForInnerBoardLayout[i][j] == EMPTY && !isalone(i, j))
                {
                    // 模拟当前玩家落子
                    arrayForInnerBoardLayout[i][j] = curpiece;
                    // 计算当前落子的分数，并加上上一层的分数
                    layerscore = evaluate(i, j) + lastscore;
                    // 递归调用minimax，进入下一层
                    score = minimax(depth - 1, beta, alpha, aipiece, weight * layerscore);
                    // 撤销落子，恢复棋盘状态
                    arrayForInnerBoardLayout[i][j] = EMPTY;

                    // 更新最小分数和beta界限
                    if (score < minscore)
                    {
                        minscore = score; // 更新最小分数为当前得分
                        beta = score;     // 更新beta为当前得分
                    }
                    // 剪枝：如果当前得分小于等于alpha，则返回当前minscore
                    if (score <= alpha)
                        return minscore;
                }
            }
        return minscore; // 返回当前层的最小分数
    }

    return 0;
}

// 评估函数，返回单个点的评估分数
long long int evaluate(int i, int j)
{
    long long posiScore = 0, negaScore = 0;
    // 评分表，对每种棋型评分
    long long int scoretab[11] = {1000000, 100000, 10000, 1000, 100, 20, 10, 2, 1, 1, -1000000};
    // 计算该点分数
    int i1, j1, i2, j2, i11, j11, i22, j22; // 分别记录同一条线上的正向与负向
    int dir;
    int piecerow;
    int count = 0;
    long long int *pscore;
    // 该点的棋子类型
    int curpiece = arrayForInnerBoardLayout[i][j];
    if (arrayForInnerBoardLayout[i][j] != EMPTY)
    {
        // 对ai有利则为正分，对对手有利则为负分
        if (curpiece == aipiece)
            pscore = &posiScore;
        else
            pscore = &negaScore;

        // 禁手
        if (curpiece == BLACK && checkForbiddenMoves(i, j))
        {
            *pscore += scoretab[DEAD];
            return posiScore - negaScore;
        }
        // 遍历每个方向
        for (dir = 0; dir < 4; dir++)
        {
            count = 0;
            // 向一个方向移动并计数
            for (i1 = i, j1 = j; iscurpiece(i1, j1, curpiece);)
            {
                i1 += direction[dir][0];
                j1 += direction[dir][1];
                count++;
            }
            // 向相反方向移动并计数
            for (i2 = i, j2 = j; iscurpiece(i2, j2, curpiece);)
            {
                i2 -= direction[dir][0];
                j2 -= direction[dir][1];
                count++;
            }
            count--; // 初始位置被计了两次

            // 注意最终i,j在两个方向上的断子处，所以此处可能是敌方子，空白和边界

            // 形成五连直接获胜；
            if (count >= 5)
            {
                *pscore += scoretab[FIVEROW];
            }

            // 判断活四和冲四
            if (count == 4)
            {
                if (isempty(i1, j1) && isempty(i2, j2))
                    *pscore += scoretab[OPENFOUR];
                else if (isempty(i1, j1) || isempty(i2, j2))
                    *pscore += scoretab[HALFOPENFOUR];
                else
                    *pscore += scoretab[SLEEP] * count;
            }
            i11 = i1 + direction[dir][0];
            j11 = j1 + direction[dir][1];
            i22 = i2 - direction[dir][0];
            j22 = j2 - direction[dir][1]; // 用于继续判断下一格

            // 判断活三和冲三和不连续冲四
            if (count == 3)
            {
                if (isempty(i1, j1) && isempty(i2, j2))
                {
                    if (!iscurpiece(i11, j11, curpiece) && !iscurpiece(i22, j22, curpiece))
                    {
                        if (isempty(i11, j11) || isempty(i22, j22))
                            *pscore += scoretab[OPENTHREE];
                        else
                            *pscore += scoretab[HALFOPENTHREE];
                    }
                    else
                    {
                        if (iscurpiece(i11, j11, curpiece))
                            *pscore += scoretab[HALFOPENFOUR] - 10;
                        if (iscurpiece(i22, j22, curpiece))
                            *pscore += scoretab[HALFOPENFOUR] - 10;
                    }
                }
                else if (isempty(i1, j1))
                {
                    if (iscurpiece(i11, j11, curpiece))
                        *pscore += scoretab[HALFOPENFOUR] - 10;
                    else if (isempty(i11, j11))
                        *pscore += scoretab[HALFOPENTHREE];
                    else
                        *pscore += scoretab[SLEEP] * count;
                }
                else if (isempty(i2, j2))
                {
                    if (iscurpiece(i22, j22, curpiece))
                        *pscore += scoretab[HALFOPENFOUR] - 10;
                    else if (isempty(i22, j22))
                        *pscore += scoretab[HALFOPENTHREE];
                    else
                        *pscore += scoretab[SLEEP] * count;
                }
                else
                    *pscore += scoretab[SLEEP] * count;
            }
            // 判断活二，不连续活三和冲四
            if (count == 2)
            {
                if (isempty(i1, j1) && isempty(i2, j2))
                {
                    if (!iscurpiece(i11, j11, curpiece) && !iscurpiece(i22, j22, curpiece))
                        *pscore += scoretab[OPENTWO];

                    for (piecerow = 0; iscurpiece(i11, j11, curpiece); i11 += direction[dir][0], j11 += direction[dir][1])
                        piecerow++;
                    if (piecerow == 1)
                    {
                        if (isempty(i11, j11))
                            *pscore += scoretab[OPENTHREE] - 10;
                        else
                            *pscore += scoretab[HALFOPENTHREE] - 10;
                    }
                    else if (piecerow >= 2)
                        *pscore += scoretab[HALFOPENFOUR] - 10;

                    for (piecerow = 0; iscurpiece(i22, j22, curpiece); i22 -= direction[dir][0], j22 -= direction[dir][1])
                        piecerow++;
                    if (piecerow == 1)
                    {
                        if (isempty(i22, j22))
                            *pscore += scoretab[OPENTHREE] - 10;
                        else
                            *pscore += scoretab[HALFOPENTHREE] - 10;
                    }
                    else if (piecerow >= 2)
                        *pscore += scoretab[HALFOPENFOUR] - 10;
                }
                else if (isempty(i1, j1))
                {
                    if (!iscurpiece(i11, j11, curpiece))
                        *pscore += scoretab[HALFOPENTWO];
                    for (piecerow = 0; iscurpiece(i11, j11, curpiece); i11 += direction[dir][0], j11 += direction[dir][1])
                        piecerow++;
                    if (piecerow == 1)
                    {
                        if (isempty(i11, j11))
                            *pscore += scoretab[HALFOPENTHREE] - 10;
                        else
                            *pscore += scoretab[SLEEP] * count;
                    }
                    else if (piecerow >= 2)
                        *pscore += scoretab[HALFOPENFOUR] - 10;
                }
                else if (isempty(i2, j2))
                {
                    if (!iscurpiece(i22, j22, curpiece))
                        *pscore += scoretab[HALFOPENTWO];
                    for (piecerow = 0; iscurpiece(i22, j22, curpiece); i22 -= direction[dir][0], j22 -= direction[dir][1])
                        piecerow++;
                    if (piecerow == 1)
                    {
                        if (isempty(i22, j22))
                            *pscore += scoretab[HALFOPENTHREE] - 10;
                        else
                            *pscore += scoretab[SLEEP] * count;
                    }
                    else if (piecerow >= 2)
                        *pscore += scoretab[HALFOPENFOUR] - 10;
                }
                else
                    *pscore += scoretab[SLEEP] * count;
            }
            // 判断不连续冲二，活三，冲四
            if (count == 1)
            {
                if (isempty(i1, j1) && isempty(i2, j2))
                {
                    if (!iscurpiece(i11, j11, curpiece) && !iscurpiece(i22, j22, curpiece))
                        *pscore += scoretab[OPENONE];
                    for (piecerow = 0; iscurpiece(i11, j11, curpiece); i11 += direction[dir][0], j11 += direction[dir][1])
                        piecerow++;
                    if (piecerow == 2)
                    {
                        if (isempty(i11, j11))
                            *pscore += scoretab[OPENTHREE] - 10;
                        else
                            *pscore += scoretab[HALFOPENTHREE] - 10;
                    }
                    else if (piecerow >= 3)
                        *pscore += scoretab[HALFOPENFOUR] - 10;
                    for (piecerow = 0; iscurpiece(i22, j22, curpiece); i22 -= direction[dir][0], j22 -= direction[dir][1])
                        piecerow++;
                    if (piecerow == 2)
                    {
                        if (isempty(i22, j22))
                            *pscore += scoretab[OPENTHREE] - 10;
                        else
                            *pscore += scoretab[HALFOPENTHREE] - 10;
                    }
                    else if (piecerow >= 3)
                        *pscore += scoretab[HALFOPENFOUR] - 10;
                }
                else if (isempty(i1, j1))
                {
                    if (!iscurpiece(i11, j11, curpiece))
                        *pscore += scoretab[HALFOPENONE];
                    for (piecerow = 0; iscurpiece(i11, j11, curpiece); i11 += direction[dir][0], j11 += direction[dir][1])
                        piecerow++;
                    if (piecerow == 2)
                    {
                        if (isempty(i11, j11))
                            *pscore += scoretab[HALFOPENTHREE] - 10;
                        else
                            *pscore += scoretab[SLEEP] * count;
                    }
                    else if (piecerow >= 3)
                        *pscore += scoretab[HALFOPENFOUR] - 10;
                }
                else if (isempty(i2, j2))
                {
                    if (!iscurpiece(i22, j22, curpiece))
                        *pscore += scoretab[HALFOPENONE];
                    for (piecerow = 0; iscurpiece(i22, j22, curpiece); i22 -= direction[dir][0], j22 -= direction[dir][1])
                        piecerow++;
                    if (piecerow == 2)
                    {
                        if (isempty(i22, j22))
                            *pscore += scoretab[HALFOPENTHREE] - 10;
                        else
                            *pscore += scoretab[SLEEP] * count;
                    }
                    else if (piecerow >= 3)
                        *pscore += scoretab[HALFOPENFOUR] - 10;
                }
                else
                    *pscore += scoretab[SLEEP] * count;
            }
        }
    }
    // 返回分数
    return posiScore - negaScore;
}
// 判断一个点是否孤立，这样可以跳过许多无效位置，节省时间
// 孤立点是八个方向两格以内都没有棋子的点
int isalone(int x, int y)
{
    int dir, i1, j1, i2, j2;
    for (dir = 0; dir < 4; dir++)
    {
        i1 = x + direction[dir][0];
        j1 = y + direction[dir][1];
        i2 = x - direction[dir][0];
        j2 = y - direction[dir][1];
        if (INBOARD(i1) && INBOARD(j1))
            if (!isempty(i1, j1))
                return 0; // 找到了附近的棋子，返回0
        if (INBOARD(i2) && INBOARD(j2))
            if (!isempty(i2, j2))
                return 0; // 找到了附近的棋子，返回0
        i1 = x + 2 * direction[dir][0];
        j1 = y + 2 * direction[dir][1];
        i2 = x - 2 * direction[dir][0];
        j2 = y - 2 * direction[dir][1];
        if (INBOARD(i1) && INBOARD(j1))
            if (!isempty(i1, j1))
                return 0; // 找到了附近的棋子，返回0
        if (INBOARD(i2) && INBOARD(j2))
            if (!isempty(i2, j2))
                return 0; // 找到了附近的棋子，返回0
    }
    return 1; // 附近无棋子
}
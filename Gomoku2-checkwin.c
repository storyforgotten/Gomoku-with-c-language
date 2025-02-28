#include "Gomoku.h"

// �������飬��ʾ�ĸ�������ƶ�
int direction[4][2] = {{0, 1}, {1, 0}, {1, 1}, {-1, 1}};

/// @brief ��鵱ǰ����Ƿ��л�ʤ��
/// @return ���ػ�ʤ��־��END���������Ϸ��־��GOON��
int checkwin()
{
    int i, j, count = 0;
    int dir;
    int currentpiece = arrayForInnerBoardLayout[row][col]; // ��ǰλ�õ�����
    // ����ÿ������
    for (dir = 0; dir < 4; dir++)
    {
        count = 0;
        // ��һ���������ƶ�������
        for (i = row, j = col; iscurpiece(i, j, currentpiece);)
        {
            i += direction[dir][0];
            j += direction[dir][1];
            count++;
        }
        // ���෴�����ƶ�������
        for (i = row, j = col; iscurpiece(i, j, currentpiece);)
        {
            i -= direction[dir][0];
            j -= direction[dir][1];
            count++;
        }
        count--; // ��ʼλ�ñ���������
        // �������Ϊ5���򷵻ػ�ʤ��־
        if ((currentpiece == BLACK && count == 5) || (currentpiece == WHITE && count >= 5))
            return END;
    }
    // û�л�ʤ��������Ϸ
    return GOON;
}
/// @brief ���ڼ�����
/// @param x ������һ��ĺ�����
/// @param y ������һ���������
/// @return ���ֱ�־��END���������Ϸ��־��GOON��
int checkForbiddenMoves(int x, int y)
{
    int liveThreeCount = 0;                 // ����
    int openFourCount = 0;                  // ����
    int halfOpenFourCount = 0;              // ����
    int longChainCount = 0;                 // ����
    int count = 0;                          // ��¼��������
    int i1, j1, i2, j2, i11, j11, i22, j22; // �ֱ��¼ͬһ�����ϵ������븺��
    // i1Ϊ��������꣬i11��i1����һ��i2�Ǹ�������꣬i22��i2ǰ��һ��
    // j1Ϊ���������꣬j11��j1����һ��j2�Ǹ��������꣬j22��j2ǰ��һ��
    int dir;              // ���ڱ�������
    int piecerow;         // �ж�������
    int curpiece = BLACK; // ��ǰ����

    // ����ÿ������
    for (dir = 0; dir < 4; dir++)
    {
        count = 0;
        // ��һ�������ƶ�������
        for (i1 = x, j1 = y; iscurpiece(i1, j1, curpiece);)
        {
            i1 += direction[dir][0];
            j1 += direction[dir][1];
            count++;
        }
        // ���෴�����ƶ�������
        for (i2 = x, j2 = y; iscurpiece(i2, j2, curpiece);)
        {
            i2 -= direction[dir][0];
            j2 -= direction[dir][1];
            count++;
        }
        count--; // ��ʼλ�ñ���������

        // ע������i,j�����������ϵĶ��Ӵ������Դ˴������ǵз��ӣ��հ׺ͱ߽�

        // �γ�����ֱ�ӻ�ʤ��
        if (count == 5)
            return GOON;

        // �жϳ���
        if (count > 5)
        {
            longChainCount++;
            return END;
        }

        // �жϻ��ĺ���������
        if (count == 4)
        {
            if (isempty(i1, j1) && isempty(i2, j2))
                openFourCount++;
            else if (isempty(i1, j1) || isempty(i2, j2))
                halfOpenFourCount++;
        }

        i11 = i1 + direction[dir][0];
        j11 = j1 + direction[dir][1];
        i22 = i2 - direction[dir][0];
        j22 = j2 - direction[dir][1]; // ���ڼ����ж���һ��

        // �ж����������Ͳ������ĳ���
        if (count == 3)
        {
            if (isempty(i1, j1) && isempty(i2, j2))
            {
                if (!iscurpiece(i11, j11, curpiece) && !iscurpiece(i22, j22, curpiece))
                {
                    if (isempty(i11, j11) || isempty(i22, j22))
                        liveThreeCount++;
                    // 01110
                }
                else
                {
                    // ����
                    if (iscurpiece(i11, j11, curpiece))
                        halfOpenFourCount++; // 011101
                    if (iscurpiece(i22, j22, curpiece))
                        halfOpenFourCount++; // 101110
                }
            }
            else if (isempty(i1, j1))
            {
                if (iscurpiece(i11, j11, curpiece))
                    halfOpenFourCount++; // x10111
            }
            else if (isempty(i2, j2))
            {
                if (iscurpiece(i22, j22, curpiece))
                    halfOpenFourCount++;
            }
        }
        // �жϲ����������Ͳ���������
        if (count == 2)
        {
            if (isempty(i1, j1) && isempty(i2, j2))
            {
                if (iscurpiece(i11, j11, curpiece) && isempty(i11 + direction[dir][0], j11 + direction[dir][1]))
                    if (!iscurpiece(i11 + 2 * direction[dir][0], j11 + 2 * direction[dir][1], curpiece))
                        liveThreeCount++;
                if (iscurpiece(i22, j22, curpiece) && isempty(i22 - direction[dir][0], j22 - direction[dir][1]))
                    if (!iscurpiece(i22 - 2 * direction[dir][0], j22 - 2 * direction[dir][1], curpiece))
                        liveThreeCount++;
            }
            if (isempty(i1, j1))
            {
                for (piecerow = 0; iscurpiece(i11, j11, curpiece); i11 += direction[dir][0], j11 += direction[dir][1])
                    piecerow++;
                if (piecerow == 2)
                    halfOpenFourCount++;
            }
            if (isempty(i2, j2))
            {
                for (piecerow = 0; iscurpiece(i22, j22, curpiece); i22 -= direction[dir][0], j22 -= direction[dir][1])
                    piecerow++;
                if (piecerow == 2)
                    halfOpenFourCount++;
            }
        }
        // �жϲ����������Ͳ���������
        if (count == 1)
        {
            if (isempty(i1, j1) && isempty(i2, j2))
            {
                for (piecerow = 0; iscurpiece(i11, j11, curpiece); i11 += direction[dir][0], j11 += direction[dir][1])
                    piecerow++;
                if (piecerow == 2)
                    if (isempty(i11, j11) && (!iscurpiece(i22, j22, curpiece)))
                        liveThreeCount++;

                i11 = i1 + direction[dir][0];
                j11 = j1 + direction[dir][1];

                for (piecerow = 0; iscurpiece(i22, j22, curpiece); i22 -= direction[dir][0], j22 -= direction[dir][1])
                    piecerow++;
                if (piecerow == 2)
                    if (isempty(i22, j22) && (!iscurpiece(i11, j11, curpiece)))
                        liveThreeCount++;
            }
            i11 = i1 + direction[dir][0];
            j11 = j1 + direction[dir][1];
            i22 = i2 - direction[dir][0];
            j22 = j2 - direction[dir][1];
            if (isempty(i1, j1))
            {
                for (piecerow = 0; iscurpiece(i11, j11, curpiece); i11 += direction[dir][0], j11 += direction[dir][1])
                    piecerow++;
                if (piecerow == 3)
                    halfOpenFourCount++;
            }
            if (isempty(i2, j2))
            {
                for (piecerow = 0; iscurpiece(i22, j22, curpiece); i22 -= direction[dir][0], j22 -= direction[dir][1])
                    piecerow++;
                if (piecerow == 3)
                    halfOpenFourCount++;
            }
        }
    }

    // ���㻷��
    if ((openFourCount + halfOpenFourCount) > 1 || liveThreeCount > 1)
        return END; // ������Ч���ڷ���

    return GOON;
}

// �ж�ĳһ�㴦�Ƿ�Ϊ�����ڵĿո�
int isempty(int x, int y)
{
    if (INBOARD(x) && INBOARD(y))
        if (arrayForInnerBoardLayout[x][y] == EMPTY)
            return 1;
    return 0;
}
// �ж�ĳһ���Ƿ�Ϊ��ǰ��curpiece
int iscurpiece(int x, int y, int curpiece)
{
    if (INBOARD(x) && INBOARD(y))
        if (arrayForInnerBoardLayout[x][y] == curpiece)
            return 1;
    return 0;
}
#include "Gomoku.h"

// �����û������벢����
int ManGo()
{

    int i, j;
    row = 0, col = 0;
    printf("��ҡ�%s��������λ�ã�\n", sign > 0 ? "�ڷ�" : "�׷�");
    getinput(input);
    // ������ת��Ϊ׼ȷλ��
    // �û������ֱֵ�Ӵ浽row��col��,����size-row
    for (i = 0; input[i] != '\0'; i++)
    {
        if (input[i] >= 'a' && input[i] <= 'o')
            col = input[i] - 'a';
        else if (input[i] >= 'A' && input[i] <= 'O')
            col = input[i] - 'A'; // ������ĸ����
        else if (isdigit(input[i]))
        {
            row = row * 10 + input[i] - '0'; // �������ֲ���
        }
        else if (input[i] == 'q')
            return QUIT; // ��ʾ�˳���Ϸ
        else if (input[i] == 'r')
            return BACK; // ��ʾ����
        else if (input[i] == 'z')
        {
            maxdepth--; // ��С������ȷ�ֹ��ʱ
            return LOWER;
        }
        else if (input[i] == 'u')
        {
            maxdepth++; // �����������
            return UPPER;
        }
        else
        {
            printf("��������!!!\n");
            return WRONG; // ��ʾ���������˳�����������ѭ��
        }
    }
    row = SIZE - row;
    // ��������Ƿ�Ϸ�
    if (row >= SIZE || col >= SIZE || row < 0 || col < 0 || arrayForInnerBoardLayout[row][col] != EMPTY)
    {
        printf("��������!!!\n");
        return WRONG;
    }
    arrayForInnerBoardLayout[row][col] = manpiece; // ����
    return DONE;                                   // �ɹ�����
}
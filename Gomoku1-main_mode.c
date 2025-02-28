#include "Gomoku.h"

// ����ʹ�õ���GBK���룬ÿһ�������ַ�ռ��2���ֽڡ�

// ������ģ��
char arrayForEmptyBoard[SIZE][SIZE * CHARSIZE + 1] = //+1��\0
    {
        "���өөөөөөөөөөөөө�",
        "�ĩ��������������",
        "�ĩ��������������",
        "�ĩ��������������",
        "�ĩ��������������",
        "�ĩ��������������",
        "�ĩ��������������",
        "�ĩ��������������",
        "�ĩ��������������",
        "�ĩ��������������",
        "�ĩ��������������",
        "�ĩ��������������",
        "�ĩ��������������",
        "�ĩ��������������",
        "���۩۩۩۩۩۩۩۩۩۩۩۩۩�"};
// ������洢������ʾ������
char arrayForDisplayBoard[SIZE][SIZE * CHARSIZE + 1];

char play1Pic[] = "��"; // ������;
char play1CurrentPic[] = "��";

char play2Pic[] = "��"; // ������;
char play2CurrentPic[] = "��";

// ���������ڼ�¼��ǰ�����̵ĸ��
int arrayForInnerBoardLayout[SIZE][SIZE];

int row = 0, col = 0, sign = 1; // row,colΪ���������������к��У�Ҳ���������ӵ�λ��
// sign���ڱ�ǵ�ǰΪ�����ӻ��ǰ��壬sign=-1Ϊ���壬+1Ϊ����

char input[6]; // ���ڶ�ȡ�û�����

int manpiece = BLACK;
int aipiece = WHITE; // ������Һ͵��Ե�����
int maxdepth = 3;    //

int main()

{
    int mode = 0, firstplayer = 0;
    // modeΪ1��ʾ�˻���ս��modeΪ2��ʾ���˶�ս
    // firstplayerΪ1��ʾ�������֣�firstplayerΪ2��ʾ�������

    // ��ȡ�û�ѡ���ģʽ
    while (1)
    {
        printf("�˻���ս��������1\n");
        printf("���˶�ս��������2��\n");
        scanf("%d", &mode);
        getchar();
        if (mode == 1 || mode == 2)
            break; // ������ȷ���˳�ѭ��
        else
        {
            printf("����������������룡\n");
            continue;
            // ����������������
        }
    }
    initRecordBoard(); // ��ʼ��һ�������̲���ʾ
    innerLayoutToDisplayArray();
    displayBoard();

    int isful = 0;               // �ж��Ƿ��������̣����˶��ٲ���
    sign = 1;                    // ���ֺ���
    int state = 0;               // �ж���������е����
    int history[SIZE * SIZE][2]; // ���ڼ�¼��ʷ�߷�������¼225��
    int current_piece;           // ��ǰ���������

    // �˻���ս
    if (mode == 1)
    {
        while (1)
        {
            // ��ȡ���������Ӫ
            printf("�������֣�������1\n");
            printf("������֣�������2��\n");
            scanf("%d", &firstplayer);
            getchar();
            if (firstplayer == 1 || firstplayer == 2)
                break; // ������ȷ���˳�ѭ��
            else
            {
                printf("����������������룡\n");
                continue; // ����������������
            }
        }
        // ���ݶ�ȡ��������Ӫ������manpiece��aipiece
        manpiece = (firstplayer == 1) ? WHITE : BLACK;
        aipiece = (firstplayer == 2) ? WHITE : BLACK;
        while (1)
        {
            // ���õ�ǰ���������
            current_piece = (sign > 0) ? BLACK : WHITE;

            if (current_piece == manpiece)
            {
                // ��������
                state = ManGo();
                // ����q�˳�
                if (state == QUIT)
                    return 0;
                // ����������������
                else if (state == WRONG || state == LOWER || state == UPPER)
                    continue;
                // ����r����
                else if (state == BACK)
                {
                    if (isful > 1)
                    {
                        // ����������������һ��������һ��
                        // ��һ���
                        isful--;
                        row = history[isful][0];
                        col = history[isful][1];
                        arrayForInnerBoardLayout[row][col] = EMPTY;
                        // ���Ի���
                        isful--;
                        row = history[isful][0];
                        col = history[isful][1];
                        arrayForInnerBoardLayout[row][col] = EMPTY;
                        innerLayoutToDisplayArray(); // ������ʾ����������
                        displayBoard();
                        printf("\n����ɹ���\n"); // ��ʾ�û�
                        continue;
                    }
                    else
                    {
                        // ����ʧ����������������ӿɻ�
                        printf("����ʧ�ܣ�\n");
                        continue;
                    }
                }
            }
            else
                AiGo(); // ��������

            // ��ʾ����������
            innerLayoutToDisplayArray();
            displayBoard();

            // ��ʾ���������λ��
            if (current_piece == aipiece)
                printf("�������壺%d %c\n", SIZE - row, 'A' + col);

            // �ж�ʤ��
            if (checkwin())
            {
                printf("%s��%s��ʤ��!!!!!\n", (current_piece == aipiece) ? "����" : "���", sign > 0 ? "�ڷ�" : "�׷�");
                getinput(input);
                if (input[0] != 'r')
                    return 0;
            }
            // �жϽ���
            if (sign > 0 && checkForbiddenMoves(row, col))
            {
                printf("�������֣��ڷ���!!!!!\n��������У�������pass���Ա����ж�\n");
                // ����pass���Ա����ж�
                getinput(input);
                if (input[0] != 'p')
                    return 0;
                printf("��Ϸ����!!!\n");
            }
            // ��¼��ǰ�߷�����ʷ��¼��
            history[isful][0] = row;
            history[isful][1] = col;
            // ���µ���������1
            isful++;
            // �ж�ƽ��
            if (isful >= SIZE * SIZE)
            {
                printf("ƽ�֣�����\n");
                return 0;
            }
            // �л���һ�ֵ�����
            sign *= -1;
        }
    }
    // ���˶�ս
    else if (mode == 2)
    {
        while (1)
        {
            manpiece = (sign > 0) ? BLACK : WHITE;
            state = ManGo(); // ��������
            if (state == QUIT)
                return 0;
            else if (state == WRONG)
                continue;
            else if (state == BACK)
            {
                // ����
                if (isful > 1)
                {
                    // ���˼�������
                    isful--;
                    row = history[isful][0];
                    col = history[isful][1];
                    arrayForInnerBoardLayout[row][col] = EMPTY;
                    // ���˶Է�����
                    isful--;
                    row = history[isful][0];
                    col = history[isful][1];
                    arrayForInnerBoardLayout[row][col] = EMPTY;
                    innerLayoutToDisplayArray(); // ��ʾ����������
                    displayBoard();
                    printf("\n����ɹ���\n");
                    continue;
                }
                else
                {
                    // ����ʧ����������������ӿɻ�
                    printf("����ʧ�ܣ�\n");
                    continue;
                }
            }

            innerLayoutToDisplayArray(); // ��ʾ���������
            displayBoard();

            // �ж�ʤ��
            if (checkwin())
            {
                printf("��%s��ʤ��!!!!!\n", sign > 0 ? "�ڷ�" : "�׷�");
                getinput(input);
                if (input[0] != 'r')
                    return 0;
            }
            // �жϽ���
            if (sign > 0 && checkForbiddenMoves(row, col))
            {
                printf("�������֣��ڷ���!!!!!\n��������У�������pass���Ա����ж�\n");
                getinput(input);
                if (input[0] != 'p')
                    return 0;
                printf("��Ϸ����!!!\n");
            }
            // ��¼��ǰ�߷�����ʷ��¼��
            history[isful][0] = row;
            history[isful][1] = col;
            // ���µ���������1
            isful++;
            // �ж�ƽ��
            if (isful >= SIZE * SIZE)
            {
                printf("ƽ�֣�����\n");
                return 0;
            }
            sign *= -1; // �л���һ�ֵ�����
        }
    }

    return 0;
}
// ��ʼ��һ�������̸��
void initRecordBoard(void)
{
    // ͨ��˫��ѭ������arrayForInnerBoardLayout��0
    int i, j;
    for (i = 0; i < SIZE; i++)
        for (j = 0; j < SIZE; j++)
            arrayForInnerBoardLayout[i][j] = EMPTY;
    return;
}

// ��arrayForInnerBoardLayout�м�¼������λ�ã�ת����arrayForDisplayBoard��
void innerLayoutToDisplayArray(void)
{
    // ��һ������arrayForEmptyBoard�м�¼�Ŀ����̣����Ƶ�arrayForDisplayBoard��
    int i, j;
    for (i = 0; i < SIZE; i++)
        for (j = 0; j < SIZE * CHARSIZE + 1; j++)
            arrayForDisplayBoard[i][j] = arrayForEmptyBoard[i][j];

    // �ڶ�����ɨ��arrayForInnerBoardLayout����������EMPTY��Ԫ�أ�������ߡ��Ƶ�arrayForDisplayBoard����Ӧλ����
    for (i = 0; i < SIZE; i++)
        for (j = 0; j < SIZE; j++)
        {
            if (arrayForInnerBoardLayout[i][j] == BLACK)
            {
                // ������ʾ
                arrayForDisplayBoard[i][j * CHARSIZE] = play1Pic[0];
                arrayForDisplayBoard[i][j * CHARSIZE + 1] = play1Pic[1];
                if (CHARSIZE == 3)
                    arrayForDisplayBoard[i][j * CHARSIZE + 2] = play1Pic[2];
            }
            else if (arrayForInnerBoardLayout[i][j] == WHITE)
            {
                // ������ʾ
                arrayForDisplayBoard[i][j * CHARSIZE] = play2Pic[0];
                arrayForDisplayBoard[i][j * CHARSIZE + 1] = play2Pic[1];
                if (CHARSIZE == 3)
                    arrayForDisplayBoard[i][j * CHARSIZE + 3] = play2Pic[2];
            }
        }
    // �������������µ����ӻ��ɡ����
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

// ��ʾ���̸��
void displayBoard(void)
{
    int i;
    // ��һ��������
    system("clear"); // ����
    // �ڶ�������arrayForDisplayBoard�������Ļ��
    printf("Work of ������\n");
    for (i = 0; i < SIZE; i++)
        printf("%3d %s\n", SIZE - i, arrayForDisplayBoard[i]);

    // ������������������һ����ĸA B ....
    char c = 'A';
    printf("    ");
    for (i = 0; i < SIZE; i++)
    {
        printf("%2c", c);
        c++;
    }
    printf("\n");
}

// ��ȡ�û�����
int getinput(char v[])
{
    int i = 0;
    char c;
    // ��ȡ�û����룬ֱ���������з�������5���ַ�
    while ((c = getchar()) != EOF && c != '\n' && i < 5)
        if (isalnum(c))
            v[i++] = c; // ������ĸ�����֣���������
    v[i] = '\0';
    return strlen(v); // ����������ַ���
}
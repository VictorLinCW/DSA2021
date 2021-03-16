// Problem 2 - Nonogram Solver
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

int** buildCheckArray(int, int[]);
bool Recur(char**, int, int, int, const int**, int, const int*);
bool isFullOfColor(int, int);
bool isValid(char**, const int**, int, int, const int*);

int main()
{
    int row, col = 0;
    scanf("%d%d", &row, &col);
    int num1 = row + col;
    int num2 = row * col;

    //build something we need
    int* checkArrayLeN = calloc(num1, sizeof(int));  //裝checkArray裡的每行長度
    
    int** checkArray = buildCheckArray(num1,checkArrayLeN);  //裝檢查用的數值

    char** paintArray = calloc(row, sizeof(char*));  //準備畫爆它。shape為 (row * col)
    for (int i = 0; i < row; i++)
    {
        paintArray[i] = calloc(col, sizeof(char));
    }

    int cumulative = -1; //進去Recur剛好變0

    Recur(paintArray, cumulative, num1, num2, checkArray, col, checkArrayLeN);
    // free
    free(checkArrayLeN);
    free(checkArray);
    free(paintArray);
    return 0; 
}

int** buildCheckArray(int num1, int checkArrayLeN[])
{
    int** checkArray = calloc(num1, sizeof(int*));
    for (int i = 0; i < num1; i++)
    {
        int len = 0;
        scanf("%d", &len); 
        checkArrayLeN[i] = len;//紀錄每行的長度給外面的array
        checkArray[i] = calloc(len, sizeof(int));
        for (int j = 0; j < len; j++)
        {
            scanf("%d",checkArray[i] + j);
        }
    }
    return checkArray;
}

bool isFullOfColor(int cumulative, int num2)
{
    if(cumulative == (num2 - 1))
    {
        return true;
    }
    return false;
}


bool Recur(char** paintArray, int cumulative, int num1, int num2, const int** checkArray, int col, const int* checkArrayLeN)
{
    cumulative += 1; //紀錄在第幾格

    if (isFullOfColor(cumulative, num2))
    {
        int row = num1 - col;
        paintArray[row - 1][col - 1] = 'o';  //last one paint cell black
        if (isValid(paintArray, checkArray, num1, col, checkArrayLeN))
        {
            char** ansArray = calloc(row, sizeof(char*));  //找到答案，輸出才會用到
            for (int i = 0; i < row; i++)
            {
                ansArray[i] = calloc(col, sizeof(char));
            }
            
            for (int i = 0; i < row; i++)
            {
                for (int j = 0; j < col; j++)
                {
                    ansArray[i][j] = paintArray[i][j];  //複製一波
                }
            }

            for (int i = 0; i < row; i++)
            {
                for (int j = 0; j < col; j++)
                {
                    printf("%c", ansArray[i][j]);  //印出ansArray
                }
                printf("\n");
            }
            return true;
        }

        paintArray[row - 1][col - 1] = '_';  //last one paint cell white
        if (isValid(paintArray, checkArray, num1, col, checkArrayLeN))
        {
            char** ansArray = calloc(row, sizeof(char*));  //輸出才會用到
            for (int i = 0; i < row; i++)
            {
                ansArray[i] = calloc(col, sizeof(char));
            }
            
            for (int i = 0; i < row; i++)
            {
                for (int j = 0; j < col; j++)
                {
                    ansArray[i][j] = paintArray[i][j];  //複製一波
                }
            }

            for (int i = 0; i < row; i++)
            {
                for (int j = 0; j < col; j++)
                {
                    printf("%c", ansArray[i][j]);  //印出ansArray
                }
                printf("\n");
            }
            return true;
        }

        paintArray[row - 1][col - 1] = 0;  //make last one cell empty
        return false;
    }

    else
    {
        int cellRow = cumulative/col;
        int cellCol = (cumulative%col);
        bool succeed = 0;

        paintArray[cellRow][cellCol] = 'o';  //paint cell black
        succeed = Recur(paintArray, cumulative, num1, num2, checkArray, col, checkArrayLeN);
        if (succeed)
            return true;

        paintArray[cellRow][cellCol] = '_';  //paint cell white
        succeed = Recur(paintArray, cumulative, num1, num2, checkArray, col, checkArrayLeN);  //fallen
        if (succeed)
             return true;   

        paintArray[cellRow][cellCol] = 0;  //make cell empty
        return false;
    }


}


bool isValid(char** paintArray, const int** checkArray, int num1, int col, const int* checkArrayLeN)
{
    int black = 0;  //黑的總數
    int record = 0;  //留下上次是黑or白的紀錄 0為白 1為黑
    int row = (num1 - col); //共有幾列，懶得傳進來哈哈
    int check = 0; //一行中連續黑的次數
    int index = 0;// 紀錄checkArray要拿第幾個元素來比
    int* blackArray = calloc(num1, sizeof(int));//此行黑色總數

    for(int i =0; i < num1; i++)  //建立blackArray
    {
        for(int j = 0; j < checkArrayLeN[i]; j++)
        {
            blackArray[i] += *(checkArray[i] + j);
        }
    }

    for (int i = 0; i < row; i++)  //先檢查row
    {
        index = 0;  //新的row，checkArray此行的第幾個使用元素要歸零
        check = 0;
        record = 0;
        black = 0;
        for (int j = 0; j < col; j++) //此row中一個個cell檢查
        {
            if (paintArray[i][j] == 'o')  //遇到黑
            {
                black += 1;
                check += 1;
                record = 1;
                if (j == (col - 1))//最後一格是黑的情形要檢查
                {
                    //check
                    if (check != *(checkArray[i] + index))
                        return false;
                }
            }
            else if (paintArray[i][j] == '_')  //遇到白有兩種情況
            {
                if(record == 1)  //上次黑要檢查
                {
                    //check
                    if (check != *(checkArray[i] + index))
                        return false;
                    index += 1; // 換checkArray的下一個元素檢查
                    check = 0;  //通過檢查就繼續跑
                    record = 0;
                }
                else if (record == 0)  //上次白不要檢查
                    record = 0;
            }
        }
        if (black != blackArray[i])
            return false;
    }

    for (int j = 0; j < col; j++)  //再檢查column
    {
        index = 0;  //新的col，checkArray此行的第幾個使用元素要歸零
        check = 0;
        record = 0;
        black = 0;
        for (int i = 0; i < row; i++) //此row中一個個cell檢查
        {
            if (paintArray[i][j] == 'o')  //遇到黑
            {
                black += 1;
                check += 1;
                record = 1;
                if (i == (row - 1))//最後一格是黑的情形
                {
                    //check
                    if (check != *(checkArray[row + j] + index))
                        return false;
                }
            }
            else if (paintArray[i][j] == '_')  //遇到白有兩種情況
            {
                if(record == 1)  //上次黑要檢查
                {
                    //check
                    if (check != *(checkArray[row + j] + index))
                        return false;
                    index += 1; // 換checkArray的下一個元素檢查
                    check = 0;  //通過檢查就繼續跑
                    record = 0;
                }
                else if (record == 0)  //上次白不要檢查
                    record = 0;
            }
        }

        if (black != blackArray[row + j])
        {
            return false;
        }
    }

    return true; //跑到這就結束啦~~
}


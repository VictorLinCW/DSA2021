// Problem 1 - Greatest Common Divisor of Big Integers
// How to deal with big integers? char array → int array
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#define MAX_LEN 257

int max(int, int);
int* charReverse2Int(char [], int, const int);
bool Comparison(const int [], const int [], const int);
// void ptrSwap(int* ptr1, int* ptr2);  爛，失敗
void swap(int **t1, int **t2);
bool isPositive(const int [], const int);
bool isEven(const int []);
void divideBy2(int [], const int);
int* subtraction(int [], const int [], const int);
void multipleBy_2k(int*, int, const int);
void printArray(int*, const int);
// const int MAX_LEN = 257;


int main()
{
    char str1[MAX_LEN] = {0};  //必須用define來宣告global variable，才能夠初始化array
	char str2[MAX_LEN] = {0};
	int multipleBy2 = 0; // 最後array要乘以2的次數
    int n1, n2= 0;

	scanf("%s%s", &str1, &str2);
	n1 = strlen(str1);
    n2 = strlen(str2);
    const int LEN = max(n1, n2);

    // int arr1[LEN]; 無法初始化，小麻煩
    // int arr2[LEN]; 更正!不是無法初始化，常數變數是要在define宣告

    int* aptr = NULL;
    int* bptr = NULL;
    aptr = charReverse2Int(str1, n1, LEN);
    bptr = charReverse2Int(str2, n2, LEN);

    int** aptr2ptr = &aptr;  //指向指標的指標
    int** bptr2ptr = &bptr;

    if (Comparison(bptr, aptr, LEN))
    {
        swap(aptr2ptr, bptr2ptr);
    }
    
    while(isPositive(aptr, LEN) && isPositive(bptr, LEN))
    {
        if (isEven(aptr) && isEven(bptr))
        {
            multipleBy2 += 1;
            divideBy2(aptr, LEN);
            divideBy2(bptr, LEN);
        }
        else if (isEven(aptr))
        {
            divideBy2(aptr, LEN);
        }
        else if (isEven(bptr))
        {
            divideBy2(bptr, LEN);
        }

        if (Comparison(bptr, aptr, LEN))
        {
            swap(aptr2ptr, bptr2ptr);
        }

        aptr = subtraction(aptr, bptr, LEN);

    }

    //把bptr裡的數*2 乘以(multipleBy2)次
    multipleBy_2k(bptr, multipleBy2, LEN);

    printArray(bptr, LEN);
    
    free(aptr);
    free(bptr);
    return 0;

}

//比大小，回傳較大值
int max(int a, int b)
{
    if(a >= b)
        return a;
    else
        return b;
}

//將 char array倒轉裝入 int array中
int* charReverse2Int(char str[], int strlen, const int LEN)
{
    int* ptr = calloc(LEN, sizeof(int));
    for (int i = 0; i < strlen; i++)
        ptr[i] = (int)(str[strlen - i - 1] - '0');  //char轉為int:減0
    return ptr;
}

//比較兩數誰較大，前面較大回傳true
bool Comparison(const int a[], const int b[], const int LEN)
{
    for (int i = LEN - 1; i >= 0; i--)
    {
        if (a[i] > b[i])
            return true;
        else if (a[i] < b[i])
            return false;
    }
    return true;
}


// void ptrSwap(int* ptr1, int* ptr2) 失敗
// {
//     int* tmp = NULL;
//     tmp = ptr1;
//     ptr1 = ptr2;
//     ptr2 = tmp;
//     return;
// }

void swap(int **t1, int **t2) {
    int* t = *t1;
    *t1 = *t2;
    *t2 = t;
}


bool isPositive(const int a[], const int LEN)
{
    for (int i = 0; i < LEN; i++)
    {
        if (a[i] > 0)
            return true;
    }
    return false;
}


bool isEven(const int a[])
{
    if (a[0] % 2 == 0)
        return true;
    return false;
}

void divideBy2(int a[], const int LEN)
{
    for(int i = LEN - 1; i >= 0; i--)
    {
        if (a[i] != 1)
        {
            int left = a[i] % 2;  //是否能整除2，不行要給下一位10
            a[i] = a[i]/2;
            if (left != 0)
            {
                a[i - 1] += 10;
            }
        }
        else if (a[i] ==1)
        {
            a[i] = 0;
            a[i - 1] += 10;
        }
    }
    return;
}


int* subtraction(int a[], const int b[], const int LEN)
{
    for (int i = 0; i < LEN; i++)
    {
        if (a[i] >= b[i])
            a[i] -= b[i];
        else
        {
            a[i] += 10;
            a[i + 1] -= 1;
            a[i] -= b[i];
        }
    }
    return a;
}

void multipleBy_2k(int* a, int multipleBy2, const int LEN)
{
    for (int i = 0; i < multipleBy2; i++)
        {
            int cumulative = 0;  //進位，給下一輪+1
            for (int i = 0; i < LEN; i++)
            {
                if (a[i] <= 4)
                {
                    a[i] *= 2;
                    a[i] += cumulative;
                    cumulative = 0;
                }
                else
                {
                    a[i] *= 2;
                    a[i] += cumulative;
                    cumulative = 0;
                    a[i] -= 10;
                    cumulative += 1;
                }
            }
        }
}

void printArray(int* a, const int LEN)
{
    int stop = 0;  //紀錄從後面回去第一個非0的數
    for(int i = LEN; i >= 0; i--)
    {
        if (a[i] == 0)
            stop += 1;
        else
            break;
    }
    for(int i = (LEN - stop); i >= 0; i--)
    {
        printf("%d", a[i]);
    }
}

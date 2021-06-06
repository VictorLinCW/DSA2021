#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>


typedef struct stack {
    int top;
    unsigned capacity;
    int* array;
}stack;

bool isStackFull(stack stack)
{
    return stack.top > stack.capacity - 1;
}

typedef struct QNode {
    int key;  //stack的index
    struct QNode* next;
    struct QNode* prev;
}QNode;


typedef struct Queue {
    struct QNode *front, *rear;
    int size ;
}Queue;


QNode* newNode(int k)
{
    struct QNode* temp = (struct QNode*)malloc(sizeof(struct QNode));
    temp->key = k;
    temp->next = NULL;
    temp->prev = NULL;
    return temp;
}


Queue* createQueue()
{
    struct Queue* q = (struct Queue*)malloc(sizeof(struct Queue));
    q->front = q->rear = NULL;
    return q;
}


Queue* enqueue(Queue* q, int k)
{
    struct QNode* temp = newNode(k);
  
    // If queue is empty
    if (q->rear == NULL)
    {
        q->front = q->rear = temp;
        q->size += 1;
        return q;
    }
    //連接好新的node
    q->rear->next = temp;
    temp->prev = q->rear;  //++
    q->rear = temp;
    //連接好首尾
    q->rear->next = q->front;  //circular
    q->front->prev = q->rear;  //circular
    
    q->size += 1;
    return q;
}


bool isInQueue(bool* InQueue, int target)
{
    return(InQueue[target]);
}

bool find_orNot(stack* Stack, int target)//是否能找到相對應的vertex
{
    stack tmp = Stack[target];
    int index = tmp.array[tmp.top];  //目前vertex上的第一個值

    if (Stack[index].array[Stack[index].top] == target)  //別人的值等於我的index
        return true;
    return false;
}

int find_Num(stack* Stack, int target)//回傳對應vertex的index
{
    return Stack[target].array[Stack[target].top];
}


int main()
{
    int numVertices; scanf("%d", &numVertices);
    stack* Stack = calloc(numVertices + 1, sizeof(stack));  //index為0不要用
    Queue* queue = createQueue();  //只需要一個queue
    queue->size = 0;
    
    int degree = 0;
    for (int i = 1; i < numVertices + 1; i++)  //讀取input到stack，index從1開始跑
    {
        Stack[i].top = 0;
        scanf("%d", &Stack[i].capacity);
        degree += Stack[i].capacity;
        Stack[i].array = calloc(Stack[i].capacity, sizeof(int));
        for (int j = 0; j < Stack[i].capacity; j++)
            scanf("%d", Stack[i].array + j);
    }


    int edgeNum = degree/2;  //所有邊的數量
    int** outcome = calloc(edgeNum, sizeof(int*));  //存輸出內容
    bool* InQueue = calloc(numVertices + 1, sizeof(bool));  //是否曾在queue裡面過了，enqueue時需要參考
    bool possible = true;  //判斷最後是yes or no
    
    for(int i = 1; i < numVertices + 1; i++)//traverse一遍所有vertex，找到第一層所有可用的vertex
    {
        if(Stack[i].capacity > 0 && find_orNot(Stack, i))
        {
            int tmp = find_Num(Stack, i);//對應的vertex
            if(!isInQueue(InQueue, i))  //沒加過才加
            {
                queue = enqueue(queue, i);
                InQueue[i] = true;
            }
            if(!isInQueue(InQueue, tmp))  //沒加過才加
            {
                queue = enqueue(queue, tmp);   
                InQueue[tmp] = true;
            }
        }
    }


    int count = 0;//目前累積的成功存取邊數
    int step = 0;//拿來偵測所有可用vertex都配對失敗的狀況
    QNode* start = queue->front; //目前成功的第一個queue(預設為front)，之後會持續更新
    int stack_index; //同等於start->key
    
    while(queue->size > 0)
    {
        // printf("queue size : %d\n", queue->size);
        step = 0;
        stack_index = start->key;
        if (isStackFull(Stack[stack_index]))  //check:目前的vertex是否跑完了，是的話換下一個
        {
            // printf("pass:%d\n", stack_index);
            QNode* tmp = start;

            //把用完的vertex前後接起來
            start->prev->next = start->next;
            start->next->prev = start->prev;
            queue->size -= 1;
            InQueue[stack_index] = 0;
            
            if (start == queue->front)
                queue->front = start->next;  //更新front成下一個
            
            if (start == queue->rear)
                queue->rear = start->prev;  //更新rear成前一個

            if (start == queue->rear->next)
                start = queue->front;  //回到front
            else
                start = start->next;
            free(tmp);
            continue;
        }

        else //經過檢查後，第一順位的vertex還沒走完
        {
            while(step < queue->size)  //迴圈目的：持續找目前所有有效的vertex是否可以找到，若都不行代表失敗
            {
                stack_index = start->key;  //vertex會持續更新
                // printf("stack_index :%d\n", stack_index);
                if(!(isStackFull(Stack[stack_index])) && find_orNot(Stack, stack_index))  //目前vertex可以用而且有找到
                {
                    // printf("delete!\n");
                    int tmp = find_Num(Stack, stack_index);  //對應的vertex index
                    if(!isInQueue(InQueue, tmp))  //此vertex還沒加進過queue
                    {
                        queue = enqueue(queue, tmp);   
                        InQueue[tmp] = true;
                    }

                    outcome[count] = calloc(2, sizeof(int));  //有了，記下來
                    int num1 = Stack[stack_index].array[Stack[stack_index].top];
                    int num2 = Stack[tmp].array[Stack[tmp].top];
                    if (num1 < num2)
                    {
                        outcome[count][0] = num1;
                        outcome[count][1] = num2;
                    }
                    else
                    {
                        outcome[count][0] = num2;
                        outcome[count][1] = num1;
                    }
                    Stack[stack_index].top += 1;
                    Stack[tmp].top += 1;
                    count += 1;
                    break;
                }
                else
                {
                    if (InQueue[stack_index])//可以用的vertex
                        step += 1;

                    if (start == queue->rear)  //跳到front
                        start = queue->front;
                    else
                        start = start->next;
                    
                    continue;
                }
            }

            if (step == queue->size)  //若上面while迴圈跑完沒成功，即代表目前所有可用vertex都無法配對
            {
                possible = false;  //狀況一：有stack但配對不到
                break;
            }
        }
    }

    if (queue->size == 0 && count < edgeNum)  //狀況二：沒vertex了
        possible = false;
    
    if (possible == false)
    {
        printf("No");
        return 0;
    }
    
    printf("Yes\n");
    for(int i = 0; i < edgeNum; i++)
    {
        for(int j = 0; j < 2; j++)
        {
            if (j == 0)
                printf("%d ", *(outcome[i]+j));
            else
                printf("%d\n", *(outcome[i]+j));
        }
    }
    
    free(Stack);
    free(outcome);
    free(InQueue);
    
    return 0;
}

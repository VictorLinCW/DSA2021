// reference:https://www.geeksforgeeks.org/queue-linked-list-implementation/
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct vertex {
    int top;
    unsigned capacity;
    int* array;
}vertex;

bool isVertexFull(vertex vertex)
{
    return vertex.top > vertex.capacity - 1;
}

typedef struct QNode {
    int key;  //vertex的index
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
  
    if (q->rear == NULL)
    {
        q->front = q->rear = temp;
        q->size += 1;
        return q;
    }
  
    q->rear->next = temp;
    q->rear = temp;
    q->size += 1;
    return q;
}

int dequeue(struct Queue* q)
{
    if (q->front == NULL)
        return 0;
  
    struct QNode* temp = q->front;
    int vertex_index = temp->key;
    q->front = q->front->next;

    if (q->front == NULL)
        q->rear = NULL;
    
    q->size -= 1;
    free(temp);
    return vertex_index;
}


bool isInQueue(bool* InQueue, int target)
{
    return(InQueue[target]);
}

bool find_orNot(vertex* Vertex, int target)//是否能找到相對應的vertex
{
    vertex tmp = Vertex[target];
    int index = tmp.array[tmp.top];  //目前vertex上的第一個值

    if (Vertex[index].array[Vertex[index].top] == target)  //別人的值等於我的index
        return true;
    return false;
}

int find_Num(vertex* Vertex, int target)//若通過find_orNot，則用此函式回傳對應vertex的index
{
    return Vertex[target].array[Vertex[target].top];
}


int main()
{
    int numVertices; scanf("%d", &numVertices);
    vertex* Vertex = calloc(numVertices + 1, sizeof(vertex));  //index為0不要用(題目規定)
    Queue* queue = createQueue();  //只需要一個queue
    queue->size = 0;
    
    int degree = 0;
    for (int i = 1; i < numVertices + 1; i++)  //讀取vertex，index從1開始跑
    {
        Vertex[i].top = 0;
        scanf("%d", &Vertex[i].capacity);
        degree += Vertex[i].capacity;
        Vertex[i].array = calloc(Vertex[i].capacity, sizeof(int));
        for (int j = 0; j < Vertex[i].capacity; j++)
            scanf("%d", Vertex[i].array + j);
    }


    int edgeNum = degree/2;  //所有邊的數量
    int** outcome = calloc(edgeNum, sizeof(int*));  //存答案
    bool* InQueue = calloc(numVertices + 1, sizeof(bool));  //是否曾在queue裡面，enqueue時需要參考
    bool possible = true;  //判斷最後是yes or no
    
    for(int i = 1; i < numVertices + 1; i++)//traverse一遍所有vertex，enqueue第一層所有可用的vertex
    {
        if(Vertex[i].capacity > 0 && find_orNot(Vertex, i) && (!isInQueue(InQueue, i)))
        {
            queue = enqueue(queue, i);
            InQueue[i] = true;

            int tmp = find_Num(Vertex, i);//對應的vertex
            if(!isInQueue(InQueue, tmp))  //沒加過才加
            {
                queue = enqueue(queue, tmp);   
                InQueue[tmp] = true;
            }
        }
    }

    int count = 0;//目前累積的成功存取邊數
    int vertex_idx;//接住dequeue的vertex值
    int num1, num2;
    
    while(queue->size > 0)
    {
        vertex_idx = dequeue(queue);
        InQueue[vertex_idx] = false;//目前vertex_idx不在queue中

        while(!(isVertexFull(Vertex[vertex_idx])) && find_orNot(Vertex, vertex_idx))//isVertexFull防止跑出去
        {
            int tmp = find_Num(Vertex, vertex_idx);  //對應的vertex index
            if(!isInQueue(InQueue, tmp))  //此vertex還沒加進過queue
            {
                queue = enqueue(queue, tmp);   
                InQueue[tmp] = true;
            }

            outcome[count] = calloc(2, sizeof(int));  //有了，記下來
            num1 = Vertex[vertex_idx].array[Vertex[vertex_idx].top];
            num2 = Vertex[tmp].array[Vertex[tmp].top];

            outcome[count][0] = num1;
            outcome[count][1] = num2;

            Vertex[vertex_idx].top += 1;//自己往後看下一格
            Vertex[tmp].top += 1;//別人也往後看下一格
            count += 1;
            continue;
        }
    }

    if (count < edgeNum)//成功的邊數應該要和所有邊數相等，否則失敗
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
    
    free(Vertex);
    free(outcome);
    free(InQueue);
    
    return 0;
}

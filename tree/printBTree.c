/*
题目：

    输入一颗二元树，从上往下按层打印树的每个节点，同一层按照从左往右的顺序打印。


输入样例：

  8
 /  \
 6  10
/ \ / \
5 7 9 11

 
输出样例：

    8 6 10 5 7 9 11


思路分析：

    把一颗二叉树抽象成三个节点：根节点、左节点、右节点。

    先序遍历即可得到按行输出的效果。

    对于左子树只要保存其根节点，既保存了整个左子树。（右子树一样）

    对于根节点之外的两个子树来说说，始终是先访问左子树的根节点，再访问右子树的根节点。

    因此可以使用队列存储。
*/
#include "stdio.h"
#include "stdlib.h"
 
//二叉树节点
#define size 7
 
//二叉树节点定义
typedef struct node
{
    int data;
    struct node *left;
    struct node *right;
}BTree;
 
int printLine(BTree * root);
BTree * CreatTree(int a[],int n);
 
int main(void)
{
 
        int array[size] = {8,6,10,5,7,9,11};
        BTree * root;
 
        root = CreatTree(array,size);
    printLine(root);   
 
        printf("\n");
    return 0;
}
 
int printLine(BTree * root)
{
    BTree * queue[size], *p;
    int front,rear;
    front = rear = 0;
     
     
     
    rear = (rear+1)%size;
    queue[rear] = root;  
 
        //循环结束为队列为空
    while(front != rear)
    {        
            //根出队列
        front = (front +1)%size;
        p = queue[front];
        printf("%3d",p->data);
 
        //左孩子不空，队不满入队
        if(p->left && ((rear+1)%size != front))
        {
            rear = (rear+1)%size;
            queue[rear] = p->left;
        }
                 
                //右孩子不空，队不满入队
        if(p->right && ((rear+1)%size != front))
        {
            rear = (rear+1)%size;
            queue[rear] = p->right;
        }
                 
                //队满，报错
        if((rear+1)%size == front)
        {
            printf("队列空间不足，错误....\n");
            return 0;
        }
    }
 
    return 1;
}
 
//根据数组创建二叉排序树
BTree * CreatTree(int a[],int n)
{
        BTree * root ,*p,*cu,*pa;
        int i;
 
        root = (BTree *)malloc(sizeof(BTree));
        root->data = a[0];
        root->left = root->right =NULL;
 
        for(i=1;i<n;i++)
        {
                p = (BTree *)malloc(sizeof(BTree));
                p->data = a[i];
                p->left = p->right =NULL;
                cu = root;
 
                while(cu)
                {
                        pa = cu;
                        if(cu->data > p->data)
                                cu = cu->left;
                        else
                                cu = cu->right;
                }
                if(pa->data > p->data)
                        pa->left = p;
                else
                        pa->right = p;
        }
 
        return root;
}

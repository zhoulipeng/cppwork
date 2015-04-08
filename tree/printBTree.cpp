/*

题目：输入一颗二元树，从上往下按层打印树的每个结点，同一层中按照从
左往右的顺序打印。 
例如输入 
  8 
 /  \  
6   10 
/\  /  \  
5 7 9 11 
输出8 6 10 5 7 9 11。

思路：广度优先遍历
*/
#include <iostream>
#include <deque>
using namespace std;

struct BTree
{
    BTree* pLeft;
    BTree* pRight;
    int value;
};
void InsertBTree(BTree* &pRoot, int val)
{
    if (!pRoot)
    {
        pRoot=new BTree;
        pRoot->pLeft=NULL;
        pRoot->pRight=NULL;
        pRoot->value=val;
    }
    else
    {
        if (val>pRoot->value)
        {
            InsertBTree(pRoot->pRight,val);
        }
        if (val<pRoot->value)
        {
            InsertBTree(pRoot->pLeft,val);
        }
        if (val==pRoot->value)
        {
            cout<<"repeated insertion."<<endl;
        }
    }
}
void VisitByLevel(BTree* pRoot)
{
    if (!pRoot)
    {
        return;
    }
    deque<BTree*> BTDeque;
    BTDeque.push_back(pRoot);
    while (BTDeque.size())
    {
        BTree* p=BTDeque.front();
        cout<<p->value<<"  ";
        BTDeque.pop_front();
        if (p->pLeft)
        {
            BTDeque.push_back(p->pLeft);
        }
        if (p->pRight)
        {
            BTDeque.push_back(p->pRight);
        }
    }
}
int main()
{
    BTree* pRoot=NULL;
    InsertBTree(pRoot,8);
    InsertBTree(pRoot,6);
    InsertBTree(pRoot,10);
    InsertBTree(pRoot,5);
    InsertBTree(pRoot,7);
    InsertBTree(pRoot,9);
    InsertBTree(pRoot,11);
    VisitByLevel(pRoot);
}

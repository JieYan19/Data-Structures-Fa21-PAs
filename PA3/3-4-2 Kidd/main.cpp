//
//  main.cpp
//  3-4-2 Kidd
//
//  Created by Yan on 2022/5/5.
//

#include <iostream>
using namespace std;

long long forSort[400002];
long long bigNode[200001]; //合并后的大点，兼任将大点转化为小点

struct SegmentNode
{
    int left;
    int right;
    int sum;
    int lazyMark;
    SegmentNode():left(0), right(0), sum(0), lazyMark(0){};
} node[800005];

class SegmentTree
{
public:
    SegmentTree(){}; //构造函数
    void build(int left, int right,int num=1) //建树
    {
        node[num].left = left;
        node[num].right = right;
        if(left ==right)
            return;
        build(left,(right+left)/2,num<<1); //递归构造，左孩子
        build((right+left)/2+1,right,(num<<1)+1); //递归构造，右孩子
    }
    void update(int left, int right, int num = 1) //更新sum（实际上更新lazyMark）
    {
        if (left>right) //此孩子不在翻转范围内
            return;
        if ((node[num].left==left)&&(node[num].right==right))
        {
            node[num].sum+=right-left+1; //满了，则更新sum
            node[num].lazyMark +=1; //同时更新lazyMark
            return;
        }
        else
        {
            update(left,min(right,node[num<<1].right),num<<1); //更新左孩子
            update(max(node[(num<<1)+1].left,left),right,(num<<1)+1); //更新右孩子
            node[num].sum = node[num<<1].sum+node[(num<<1)+1].sum;
        }
    }
    void pushDown(int num) //向下更新sum
    {
        int tmpLeft = num<<1;
        int tmpRight =(num<<1)+1;
        node[tmpLeft].sum+=node[num].lazyMark*(node[tmpLeft].right-node[tmpLeft].left+1);
        node[tmpRight].sum+=node[num].lazyMark*(node[tmpRight].right-node[tmpRight].left+1);
            node[tmpLeft].lazyMark +=node[num].lazyMark;
            node[tmpRight].lazyMark += node[num].lazyMark;
            node[num].lazyMark=0;
    }
    int query(int left, int right, int num = 1)
    {
        if (left>right) //此孩子不在查询范围内
            return 0;
        if((node[num].left==left)&&(node[num].right==right))
            return node[num].sum;
        else
        {
            if(node[num].lazyMark!=0)
                pushDown(num);
            return query(left,min(right,node[num<<1].right),num<<1)+query(max(node[(num<<1)+1].left,left),right,(num<<1)+1); //查询左孩子和查询右孩子sum之和
        }
        return -1; //错误
    }
};

//轴点构造算法：通过调整元素位置构造区间[lo, hi)的轴点，并返回其秩
int partition (int lo, int hi) //DUP版：可优化处理多个关键码雷同的退化情况
{
    swap ( forSort[lo], forSort[ lo + rand() % ( hi - lo ) ] ); //任选一个元素与首元素交换
    hi--;
    long long pivot = forSort[lo]; //以首元素为候选轴点——经以上交换，等效于随机选取
    while ( lo < hi ) //从向量的两端交替地向中间扫描
    {
        while ( lo < hi )
            if ( pivot < forSort[hi] ) //在大于pivot的前提下
                hi--; //向左拓展右端子向量
            else //直至遇到不大于pivot者
            {
                forSort[lo++] = forSort[hi]; //将其归入左端子向量
                break;
            }
        while ( lo < hi )
            if ( forSort[lo] < pivot ) //在小于pivot的前提下
                lo++; //向右拓展左端子向量
            else //直至遇到不小于pivot者
            {
                forSort[hi--] = forSort[lo]; //将其归入右端子向量
                break;
            }
    } //assert: lo == hi
    forSort[lo] = pivot; //将备份的轴点记录置于前、后子向量之间
    return lo; //返回轴点的秩
}

//快速排序
void quickSort (int lo, int hi ) //0 <= lo < hi <= size
{
    if ( hi - lo < 2 ) //单元素区间自然有序，否则...
        return;
    int mi = partition ( lo, hi ); //在[lo, hi)内构造轴点
    quickSort ( lo, mi ); //对前缀递归排序
    quickSort ( mi + 1, hi ); //对后缀递归排序
}

int smallN2bigN(long long small) //将小点转化为大点
{
    int i;
    for (i = 1; i < 200001;i++)
        if(small == bigNode[i])
            break;
    return i;
}

int main()
{
    SegmentTree tree; //建一棵segment tree
    int n; //扑克牌的数量
    int m; //操作个数
    cin >>n>>m;
    int optRec[200001];
    long long left[200001];
    long long right[200001];
    for (int i=0;i<m;i++)
    {
        char opt;
        cin >> opt >> left[i] >> right[i];
        forSort[2*i] = left[i];
        forSort[2*i+1] = right[i];
        if(opt=='H')
            optRec[i]=0;
        if(opt=='Q')
            optRec[i]=1;
    }
    quickSort(0, 2*m); //将涉及到的断点排序（只有断点会被改变或询问
//    int small2Big[20000147483649];
    int curBig = 1;
    int curSmall = 0;
    while (curSmall<2*m-1)
    {
        while((forSort[curSmall] == forSort[curSmall+1]) &&(curSmall<2*m-1))
            curSmall++;
//        small2Big[forSort[curSmall]] = curBig;
        bigNode[curBig++]=forSort[curSmall++];
    }
    if(curSmall==2*m-1) //forSort[2m-2]与forSort[2m-1]不同，导致第2m-1项还没并入bigNode数组
    {
//        small2Big[forSort[curSmall]] = curBig;
        bigNode[curBig++] = forSort[curSmall];
    }
    tree.build(1,curBig -1);
    for (int i = 0;i<m;i++)
    {
        if(optRec[i]==0) //翻转操作
            tree.update(smallN2bigN(left[i]),smallN2bigN(right[i]));
        if(optRec[i]==1) //查询操作
            cout << tree.query(smallN2bigN(left[i]),smallN2bigN(right[i])) << endl;
    }
    return 0;
}

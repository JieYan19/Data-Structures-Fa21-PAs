//
//  main.cpp
//  3-1 Build
//
//  Created by Yan on 2022/4/9.
//

#include <iostream>
using namespace std;

struct MultiNode //多叉树节点类
{
    //可以全部使用int型而非MultiNode*型，因为第一本题只涉及int型，第二每个点的序号是唯一的，可精准定位
    //int data; //无需data，因为node数组的下标就等于data，且全程没有修改
    int parent;     //父节点
    int firstChild; //第一个孩子结点
    int leftBro;    //左兄弟结点
    int rightBro;   //右兄弟结点
    int height;     //高度，单结点高度为0
    int size;       //子树规模，单结点子树规模为1
    //构造函数
    MultiNode() : parent(-1), firstChild(-1), leftBro(-1), rightBro(-1), height(0), size(1){};
} node[1000005];

class MultiTree //多叉树模板类
{
public:
    MultiTree(){}; //构造函数
    int initHeight(int curNode = 1) //初始化height
    {
        //每个结点的height是其rB的height和fC的height+1中的最大值；最右结点的height是其fC的height+1
        if (curNode == -1) //curNode不存在子结点但依然调用了initHeight的情况
            return -1;
        if (node[curNode].height != 0) //之前已经算过
            return node[curNode].height;
        if (node[curNode].rightBro == -1) //最右结点，无rB
        {
            if (node[curNode].firstChild == -1) //无fC
                return 0;
            node[curNode].height = initHeight(node[curNode].firstChild) + 1; //有fC
            return node[curNode].height;
        }
        node[curNode].height = max(initHeight(node[curNode].rightBro), initHeight(node[curNode].firstChild) + 1); //有rB
        return node[curNode].height;
    }
    int initSize(int curNode = 1) //初始化size
    {
        int child = node[curNode].firstChild;
        if (node[curNode].firstChild == -1) //无孩子结点，即size=1
            return 1;
        if (node[curNode].size != 1) //之前已经算过，可直接返回计算值(.size相当于备忘录)
            return node[curNode].size;
        while (child != -1)
        {
            node[curNode].size += initSize(child);
            child = node[child].rightBro;
        }
        return node[curNode].size;
    }
    int find(int *path, int pathLen, int num = 0, int data = 1) //根据从根节点出发的路径来查询子树根节点
    {
        if (num == pathLen) //递归终点：path的最后一个元素也已找到对应的孩子结点
            return data;
        int cur = node[data].firstChild;
        if (cur == -1)
            return data;
        for (int i = 0; i < path[num]; i++)
        {
            cur = node[cur].rightBro;
            if (cur == -1)
                return data;
        }
        return find(path, pathLen, ++num, cur);
    };
    void move(int *pathDep, int *pathDes, int childPos, int pathDepLen, int pathDesLen) //子树移动方法
    {
        //需要改源子树的根、根结点原/新的父/左/右结点的参数信息，同时更新高度与规模
        int depRoot = find(pathDep, pathDepLen); //寻找path内第0个项对应的孩子结点，此孩子结点为data为1的结点(根节点)的孩子结点
        int probe = node[depRoot].leftBro; //记录原左/父结点信息，方便后续更新高度
        if (node[depRoot].leftBro == -1)
            probe = node[depRoot].parent;
        //更改原周围结点的参数(即从原位置中删除此子树)
        if (node[node[depRoot].parent].firstChild == depRoot) //改原父结点的fC
            node[node[depRoot].parent].firstChild = node[depRoot].rightBro;
        if (node[depRoot].leftBro != -1) //改原左兄弟结点的rB
            node[node[depRoot].leftBro].rightBro = node[depRoot].rightBro;
        if (node[depRoot].rightBro != -1) //改原右兄弟结点的lB
            node[node[depRoot].rightBro].leftBro = node[depRoot].leftBro;
        /*
         更改源子树的根及新周围结点的参数：
         新父结点：不变 或 修改firstChild
         新左兄弟结点：不存在 或 修改rightBro
         新右兄弟结点：不存在 或 修改leftBro
         自己：修改parent & leftBro & rightBro
         */
        int desRoot = find(pathDes, pathDesLen); //目的位置的节点表示为移除源子树后的节点表示!!!
        updateSize(depRoot, desRoot); //更新子树规模，此时depRoot的parent还是原父结点
        node[depRoot].parent = desRoot;
        if (node[desRoot].firstChild == -1) //移到没有子结点的父节点，那么没有子结点的左右兄弟需要更改
        {
            node[desRoot].firstChild = depRoot;
            node[depRoot].leftBro = -1;
            node[depRoot].rightBro = -1;
            updateHeight(probe); //更新子树新左/父结点高度①
            updateHeight(depRoot);
            return;
        }
        int cur = node[desRoot].firstChild;
        bool mostRight = false;
        for (int i = 0; i < childPos; i++) //切换到childPos位
        {
            if (node[cur].rightBro == -1) //移到最右
            {
                mostRight = true;
                break;
            }
            cur = node[cur].rightBro;
        }
        if (childPos == 0) //移到最左
            node[desRoot].firstChild = depRoot;
        if (mostRight == true) //移到最右
        {
            node[cur].rightBro = depRoot;
            node[depRoot].leftBro = cur;
            node[depRoot].rightBro = -1;
            updateHeight(probe); //更新子树新左/父结点高度②
            updateHeight(depRoot);
            return;
        }
        node[depRoot].leftBro = node[cur].leftBro; //先对depRoot的leftBro赋值，否则cur的leftBro将改变
        node[depRoot].rightBro = cur;
        if (childPos != 0) //移到最左时无需更改左兄弟的右兄弟
            node[node[cur].leftBro].rightBro = depRoot;
        node[cur].leftBro = depRoot;
        updateHeight(probe); //更新子树新左/父结点高度③
        updateHeight(depRoot);
    };
    void updateHeight(int curNode) //从curNode开始更新，然后找curNode的左/父结点依次更新
    {
        //对子/右结点无影响，影响历代的[父(为fC时)/所有左结点]和自身
        while (true)
        {
            int rightBH = 0;
            int childH = -1;
            if (node[curNode].rightBro != -1) //当前结点有右兄弟结点
                rightBH = node[node[curNode].rightBro].height;
            if (node[curNode].firstChild != -1) //当前结点有孩子结点
                childH = node[node[curNode].firstChild].height;
            node[curNode].height = max(rightBH, childH + 1); //更新当前结点高度
            //下面按照左/父的顺序寻找下一需更新高度的结点
            if (node[curNode].leftBro != -1) //当前结点有左兄弟结点则找左兄弟结点
                curNode = node[curNode].leftBro;
            else if (node[curNode].parent != -1) //当前结点无左兄弟结点则找父结点
                curNode = node[curNode].parent;
            else //父结点也不存在则代表更新到了根结点1
                break;
        }
    }
    void updateSize(int moveNode, int newParent) //更新子树规模：移动的子树的规模/移动到的新父结点
    {
        int oldParent = node[moveNode].parent;
        while (oldParent != -1)
        {
            node[oldParent].size -= node[moveNode].size;
            oldParent = node[oldParent].parent;
        }
        while (newParent != -1)
        {
            node[newParent].size += node[moveNode].size;
            newParent = node[newParent].parent;
        }
    }
    int showHeight(int *path, int pathLen) //查询高度
    {
        int root = find(path, pathLen);
        if (node[root].firstChild == -1)
            return 0;
        return node[node[root].firstChild].height + 1; //自身的height是右下区域的最高height，fC的height+1才是自身实际的高度
    }
    int showSize(int *path, int pathLen) //查询子树规模
    {
        int root = find(path, pathLen);
        return node[root].size;
    }
    void debug()
    {
        for (int i = 1; i <= 8; i++)
        {
            /*
            cout << i << ":" << endl;
            if (node[i].firstChild == -1)
                cout << "[height] " << 0 << endl;
            else
                cout << "[height] " << node[node[i].firstChild].height + 1 << endl;
            cout << "[size] " << node[i].size << endl;
            int cur = node[i].firstChild;
            while(cur != -1)
            {
                cout << cur << " ";
                cur = node[cur].rightBro;
            }
            cout << endl;
            //*/
        }
    }
};

int main()
{
    MultiTree tree;
    int n, m;
    cin >> n >> m; //多叉树有n个节点，子树移动和查询操作共有m个
    for (int i = 1; i <= n; i++)
    {
        int childNum;
        cin >> childNum;
        if (childNum != 0)
        {
            int predChild;
            cin >> predChild;
            node[i].firstChild = predChild;
            node[predChild].parent = i;
            int succChild;
            for (int j = 1; j < childNum; j++) //创建左右兄弟、单向孩子关系
            {
                cin >> succChild;
                node[succChild].parent = i;
                node[predChild].rightBro = succChild;
                node[succChild].leftBro = predChild;
                predChild = succChild;
            }
        }
    }
    tree.initHeight(); //初始化height
    tree.initSize(); //初始化size
    for (int i = 0; i < m; i++)
    {
        //tree.debug();
        int type;
        cin >> type;
        int pathLen; //源子树或所查询根节点的路径长
        cin >> pathLen;
        int path[pathLen]; //源子树或所查询根节点的具体路径
        for (int j = 0; j < pathLen; j++)
            cin >> path[j];
        if (type == 0) //子树移动操作
        {
            int pathDesLen; //插入到的目标结点的路径长
            cin >> pathDesLen;
            int pathDes[pathDesLen]; //插入到的目标结点的具体路径
            for (int j = 0; j < pathDesLen; j++)
                cin >> pathDes[j];
            int childPos; //插入到的目标节点的孩子序列的位置
            cin >> childPos;
            tree.move(path, pathDes, childPos, pathLen, pathDesLen);
        }
        else if (type == 1) //查询高度操作
        {
            cout << tree.showHeight(path, pathLen) << endl;
        }
        else if (type == 2) //查询子树规模操作
        {
            cout << tree.showSize(path, pathLen) << endl;
        }
    }
    return 0;
}

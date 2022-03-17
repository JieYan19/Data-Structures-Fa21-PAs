//
//  main.cpp
//  2-3 ⑨的完美文本编辑器 V1
//
//  Created by Yan on 2022/3/16.
//

#include <iostream>
using namespace std;

struct ListNode { //列表节点模板类（以双向链表形式实现）
//成员
    char data; //数值
    ListNode* pred; //前驱
    ListNode* succ; //后继
//构造函数
    ListNode() {}; //创建头哨兵、尾哨兵时使用此种构造函数
    ListNode(char e, ListNode* p = NULL, ListNode* s = NULL) : data(e), pred(p), succ(s) {}; //构造器
//操作接口
//    ListNode* insertAsPred (char const& e) //紧靠当前节点之前插入新节点
//    {
//        ListNode* x = new ListNode(e, pred, this); //创建新节点
//        pred->succ = x;
//        pred = x; //设置正向链接？
//        return x; //返回新节点的位置
//    };
    ListNode* insertAsSucc (char const& e) //紧随当前节点之后插入新节点
    {
        ListNode* x = new ListNode(e, this, succ); //创建新节点
        succ->pred = x;
        succ = x; //设置逆向链接？
        return x; //返回新节点的位置
    }
};

class List {
//private:
public:
//    int _size; //规模
    ListNode* header; //头哨兵
    ListNode* trailer; //尾哨兵
    
protected:
    void init()
    {
        header = new ListNode; //创建头哨兵节点
        trailer = new ListNode; //创建尾哨兵节点
        header->succ = trailer;
        header->pred = NULL;
        trailer->pred = header;
        trailer->succ = NULL;
    };
   
public:
//构造函数
    List() {init();};
//可写访问接口
    ListNode* insertR(ListNode* p, char const& e) //e从p的右侧（后继）插入
    {
        return p->insertAsSucc(e);
    };
//    ListNode* insertL(ListNode* p, char const& e) //e从p的左侧（前驱）插入
//    {
//        return p->insertAsPred(e);
//    };
    ListNode* removeR (ListNode* p) //删除合法节点p
    {
        p->pred->succ = p->succ; //改前驱的后继
        p->succ->pred = p->pred; //改后继的前驱
        return p->succ;
    }
    void reverse (ListNode* left, ListNode* right, int sum)
    {
        for (int i = 1; i < sum; i+=2) //（从首、末节点开始）由外而内，捉对地
        {
            swap (left->data, right->data); //交换对称节点的数据项
            left = left->succ;
            right = right->pred;
        }
    }
    void show () //显示链表内所有节点
    {
        ListNode* current = header->succ; //最后处理后为空？
        while (current != trailer)
        {
            cout << current->data;
            current = current->succ;
        }
        cout << endl;
    }
};

int main() {
    List list;
    ListNode* pred = list.header;
    char text = getchar();
    while (text != '\n')
    {
        pred = list.insertR(pred, text);
        text = getchar();
    }
    ListNode* cursorL = new ListNode(0, list.header, list.header->succ); //左光标
    ListNode* cursorR = new ListNode(0, list.trailer->pred, list.trailer); //右光标
    int n;
    cin >> n;
    for (int i = 0; i < n; i++)
    {
        getchar(); //读取上一行输入冗余换行符
        char opt = getchar();
        if (opt == '<')
        {
            getchar();
            opt = getchar();
            if (opt == 'L')
            {
                if (cursorL->pred != list.header)
                {
                    cout << "T" << endl;
                    cursorL->succ = cursorL->pred;
                    cursorL->pred = cursorL->pred->pred;
                }
                else
                    cout << "F" << endl;
                continue;
            }
            else
            {
                if (cursorR->pred != list.header)
                {
                    cout << "T" << endl;
                    cursorR->succ = cursorR->pred;
                    cursorR->pred = cursorR->pred->pred; //两个赋值式不能反
                }
                else
                    cout << "F" << endl;
                continue;
            }
        }
        else if (opt == '>')
        {
            getchar();
            opt = getchar();
            if (opt == 'L')
            {
                if (cursorL->succ != list.trailer)
                {
                    cout << "T" << endl;
                    cursorL->pred = cursorL->succ;
                    cursorL->succ = cursorL->succ->succ;
                }
                else
                    cout << "F" << endl;
                continue;
            }
            else
            {
                if (cursorR->succ != list.trailer)
                {
                    cout << "T" << endl;
                    cursorR->pred = cursorR->succ;
                    cursorR->succ = cursorR->succ->succ;
                }
                else
                    cout << "F" << endl;
                continue;
            }
        }
        else if (opt == 'I')
        {
            cout << "T" << endl;
            getchar();
            opt = getchar();
            if (opt == 'L')
            {
                getchar();
                char e = getchar();
                cursorL->pred = list.insertR(cursorL->pred, e);
                if (cursorR->pred->succ == cursorL->pred) //若cursorR的前驱与新加入节点相链，则需更新cursorR的前驱为新加入节点
                    cursorR->pred = cursorL->pred;
            }
            else
            {
                getchar();
                char e = getchar();
                cursorR->pred = list.insertR(cursorR->pred, e);
                if (cursorL->pred->succ == cursorR->pred) //若cursorL的前驱与新加入节点相链，则需更新cursorL的前驱为新加入节点
                    cursorL->pred = cursorR->pred;
            }
            continue;
        }
        else if (opt == 'D')
        {
            getchar();
            opt = getchar();
            if (opt == 'L')
            {
                if (cursorL->succ != list.trailer)
                {
                    cout << "T" << endl;
                    cursorL->succ = list.removeR(cursorL->succ);
                }
                else
                    cout << "F" << endl;
            }
            else
            {
                if (cursorR->succ != list.trailer)
                {
                    cout << "T" << endl;
                    cursorR->succ = list.removeR(cursorR->succ);
                }
                else
                    cout << "F" << endl;
            }
            continue;
        }
        else if (opt == 'R') //反向反data即可
        {
            ListNode* left = cursorL->succ;
            ListNode* right = cursorL->succ;
            bool valid = true;
            int sum = 0;
            while (right != cursorR->pred->succ)
            {
                if (right == list.trailer)
                {
                    valid = false;
                    break;
                }
                sum++;
                right = right->succ;
            }
            if (left == right) //左端与右端重合
                valid = false;
            if (valid == true)
            {
                cout << "T" << endl;
                list.reverse(left, right->pred, sum);
            }
            else
                cout << "F" << endl;
        }
        else if (opt == 'S')
        {
            list.show();
        }
    }
    return 0;
}

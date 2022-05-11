//
//  main.cpp
//  3-6 Hacker
//
//  Created by Yan on 2022/5/9.
//

#include <iostream>
#include <cstring>
#include <cmath>
#include "crc32.h"
using namespace std;

const int CHAR_SUM = 18;
char c[CHAR_SUM + 1] = "0123456789tsinghua";
char salt[4];
int saltLen;           // salt长度
char allHead[1000001]; //所有破解的明文的字母头顺序组成的数组
char *curHead;         //当前指向的数组

struct headJoint //破解结果集合相关数据记录（只包含长度为6、7、8的明文）
{
    unsigned int crc;
    long long key;
    long long value;
} headJoint[3];

struct EntryList
{
    long long key;   //明文加盐后对应的哈希值
    long long value; //明文的18进制表示
    int num;         //明文的位数，不超过8位
    EntryList *next;
    bool removed; //懒惰删除标记
    EntryList(long long k, long long v, int n) : key(k), value(v), num(n), next(NULL), removed(false){};
};
EntryList *ans; //找到的明文

class Hashtable // Hashtable模板类
{
private:
    EntryList **ht; //桶数组，存放词条指针
    int M;          //桶的总数

protected:
    int probe(const long long &k) //查找词条k匹配的桶
    {
        int r = ((size_t)((k >> 32) + (int)k)) % M; //按除余法确定桶序号
        return r;
    }

public:
    Hashtable(int c) //创建一个容量不小于c的散列表
    {
        M = c;
        ht = new EntryList *[M];                //开辟桶数组
        memset(ht, 0, sizeof(EntryList *) * M); //初始化各桶
    }
    void put(long long k, long long v, int n) //词条插入
    {
        int r = probe(k); //为新词条找个空桶
        if (ht[r] == NULL)
            ht[r] = new EntryList(k, v, n); //插入
        else
        {
            EntryList *cur = ht[r];
            bool exist = false;
            while (cur->next != NULL)
            {
                if (k == cur->key)
                    if (v == cur->value && n == cur->num)
                    {
                        cur->removed = true;
                        exist = true;
                        break;
                    }
                cur = cur->next;
            }
            if (exist == false)
                cur->next = new EntryList(k, v, n);
        }
    }
    int get(long long k) //词条查找 | 未找到返回0，找到一个返回1，找到多个返回2
    {
        int r = probe(k);
        if (ht[r] == NULL)
            return 0;
        EntryList *cur = ht[r];
        bool haveFound = false;
        while (cur != NULL)
        {
            if (k == cur->key && cur->removed == false)
            {
                if (haveFound == true)
                    return 2;
                ans = cur; //用ans记录找到的指针
                haveFound = true;
            }
            cur = cur->next;
        }
        if (haveFound == true)
            return 1;
        return 0;
    }
    void remove(long long k, long long v, int n) //词条删除（懒惰删除标记）
    {
        int r = probe(k);
        //        if (ht[r]==NULL)
        //            return; //此桶中没任何词条
        //        if (ht[r]->removed==true)
        //            return; //此桶中的词条已被懒惰删除
        EntryList *cur = ht[r];
        while (cur != NULL)
        {
            if (k == cur->key)
            {
                if (v == cur->value && n == cur->num)
                {
                    cur->removed = true;
                    break;
                }
            }
            cur = cur->next;
        }
    }
};
Hashtable ht(9999991);

long long diciotto2Dieci(char *plain, int len, int start = 0) //仿十八进制转化为十进制
{
    long long num = 0;
    for (int i = 0; i < len; i++)
    {
        int j;
        if (plain[i + start] <= '9')
            j = plain[i + start] - '0';
        else
            for (j = 10; j < CHAR_SUM; j++)
                if (plain[i + start] == c[j]) //找到此字母对应的18进制数j
                    break;
        num = num * 18 + j;
    }
    return num;
}

long long sedici2Dieci(char *a, int len) //十六进制转化为十进制
{
    long long sum = 0;
    for (int i = 0; i < len; i++)
        if (a[i] <= '9')
            sum += (a[i] - '0') * pow(16, (len - i - 1));
        else
        {
            sum += (a[i] - 'a' + 10) * pow(16, (len - i - 1));
        }
    return sum;
}

string dieci2Sedici(long long b, int len) //十进制转化为十六进制
{
    string bChar;
    for (int i = 0; i < len; i++)
    {
        int bVal = b / pow(18, (len - 1 - i));
        bChar += c[bVal];
        b -= bVal * pow(18, (len - 1 - i));
    }
    return bChar;
}

void init(unsigned int preCrc = 0, long long preVal = 0, int len = 1) //初始化明文试探池
{
    if (len > 5)
        return;
    char *plain = c;
    for (int i = 0; i < CHAR_SUM; i++)
    {
        long long curVal = preVal * 18 + diciotto2Dieci(plain, 1);      //明文字母串对应的18进制数字，用于下一轮计算，但在这一轮循环中preVal的值不能变
        unsigned int curCrc = crc32(preCrc, (unsigned char *)plain, 1); //未加盐
        ht.put(crc32(curCrc, (unsigned char *)salt, saltLen), curVal, len);
        init(curCrc, curVal, len + 1);
        plain += 1;
    }
}

void update(char head, int num)
{
    curHead = allHead;
    allHead[num] = head;
    if (num >= 5)
    {
        curHead += num;
        if (num >= 6)
        {
            ht.remove(headJoint[0].key, headJoint[0].value, 6);
            if (num >= 7)
            {
                ht.remove(headJoint[1].key, headJoint[1].value, 7);
                if (num >= 8)
                {
                    ht.remove(headJoint[2].key, headJoint[2].value, 8);
                }
                headJoint[2].crc = crc32(headJoint[1].crc, (unsigned char *)curHead, 1); //未加盐
                headJoint[2].key = crc32(headJoint[2].crc, (unsigned char *)salt, saltLen);
                headJoint[2].value = diciotto2Dieci(allHead, 8, num - 7);
                ht.put(headJoint[2].key, headJoint[2].value, 8);
            }
            headJoint[1].crc = crc32(headJoint[0].crc, (unsigned char *)curHead, 1); //未加盐
            headJoint[1].key = crc32(headJoint[1].crc, (unsigned char *)salt, saltLen);
            headJoint[1].value = diciotto2Dieci(allHead, 7, num - 6);
            ht.put(headJoint[1].key, headJoint[1].value, 7);
        }
        curHead -= 5;
        headJoint[0].crc = crc32(0, (unsigned char *)curHead, 6); //未加盐
        headJoint[0].key = crc32(headJoint[0].crc, (unsigned char *)salt, saltLen);
        headJoint[0].value = diciotto2Dieci(allHead, 6, num - 5);
        ht.put(headJoint[0].key, headJoint[0].value, 6);
    }
}

int main()
{
    int n; //密文个数
    cin >> n;
    cin >> salt;
    saltLen = (int)strlen(salt); // strlen获取salt长度
    init();
    int success = 0;
    for (int i = 0; i < n; i++)
    {
        char cipher[9];
        cin >> cipher; //输入十六进制密文
        int result = ht.get(sedici2Dieci(cipher, 8));
        switch (result)
        {
        case 0:
            cout << "No" << endl;
            break;
        case 1:
        {
            string str = dieci2Sedici(ans->value, ans->num);
            cout << str << endl;
            update(str[0], success);
            success++;
            break;
        }
        case 2:
            cout << "Duplicate" << endl;
            break;
        }
    }
    return 0;
}

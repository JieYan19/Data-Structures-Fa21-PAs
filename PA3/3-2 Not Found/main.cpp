//
//  main.cpp
//  3-2 Not Found
//
//  Created by Yan on 2022/5/3.
//

#include <iostream>
#include <cstring>
using namespace std;

class Bitmap //位图Bitmap类
{
private:
    unsigned char *M; //比特图所存放的空间M[]，char为一个字节
    int N; //容量为N*sizeof(char)*8比特
protected:
    void init(int n) //初始化
    {
        M = new unsigned char[N = (n + 7) / 8];
        memset(M, 0, N); //等效于诸位清零
    }

public:
    Bitmap(int n) //按指定或默认规模创建比特图
    {
        init(n);
    }
    void set(int k) //插入
    {
        M[k >> 3] |= (0x80 >> (k & 0x07));
    }
    bool test(int k) //判断
    {
        return M[k >> 3] & (0x80 >> (k & 0x07));
    }
    void debug()
    {
        for (int i = 1; i <= 64; i++)
        {
            if (test(i) == 1)
                cout << i << " ";
        }
        cout << endl;
    }
};
Bitmap bitmap(1 << 24); //开辟2^24空间

void resWrite(int num, int length) //余下的短串写入Bitmap
{
    for (int i = length - 1; i >= 1; i--)
    {
        num = num % (1 << i);
        bitmap.set(num + (1 << i));
    }
}

int main()
{
    //先一次性连读24个，然后再一次读一个，删除第一个 | 若一次性连读失败，则对这一
    char curChar = '0';
    int num = 0;
    bool shortStr = false; //shortStr表征此串是否是长度不大于24的串
    for (int i = 0; i < 24; i++)
    {
        curChar = getchar(); //getchar读取出的是char型，若存在int变量里，则是ASCII码
        if (curChar == '\n')
        {
            shortStr = true;
            bitmap.set(num + (1 << i));
            resWrite(num, i);
            break;
        }
        num = (num << 1) + ((int)curChar - 48); //实际值与ASCII码差48
    }
    if (shortStr == false)
    {
        while (curChar != '\n')
        {
            num = (num % (1 << 23)) * (1 << 1) + ((int)curChar - 48);
            bitmap.set(num);
        }
        resWrite(num, 24);
    }
    for (int i = 1; i < (1 << 24); i++)
    {
        if (bitmap.test(i) == true)
        {
            int num = i;
            while (num != 0)
            {
                num = num >> 1;
                bitmap.set(num);
            }
        }
    }
    for (int i = 1; i < (1 << 24); i++)
    {
        if (bitmap.test(i) == false)
        {
            string str;
            int target = i;
            int j = 0;
            while (target / 2 != 0)
            {
                str += target % 2 + '0';
                target /= 2;
                j++;
            }
            for (int k = str.size() - 1; k >= 0; k--)
                cout << str[k];
            cout << endl;
            break;
        }
    }
    return 0;
}

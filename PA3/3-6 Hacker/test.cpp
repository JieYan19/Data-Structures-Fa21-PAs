//
//  main.cpp
//  test
//
//  Created by Yan on 2022/5/11.
//

#include <iostream>
#include <fstream>
#include "crc32.h"
using namespace std;

int len = 0;

unsigned int encrypt(char* c, char* salt)
{
    unsigned int raw = crc32(0, (unsigned char *)c, len);
    return crc32(raw, (unsigned char *)salt, (int)strlen(salt));
}

string dieci2Sedici(unsigned int num)
{
    string str;
    string tplt = "0123456789abcdef";
    long long x;
    for (int i = 0; i < 8; i++)
    {
        x = num % 16;
        // 将 num % 16 转换为字符逆序存入 str
        str = tplt[x] + str;
        num = num / 16;
    }
    if(str == "")
        return 0;
    else
        return str;
}

char* random(char* c)
{
    len = rand() % 9 + 1;
    for (int i = 0; i < len; i++)
    {
        switch ((rand() % 4))
        {
        case 0:
            c[i] = '6';
            break;
        case 1:
            c[i] = 'u';
            break;
        case 2:
            c[i] = '4';
            break;
        case 3:
            c[i] = 't';
            break;
        }
    }
    return c;
}

int main()
{
    int n;
    cin >> n;
    char salt[5];
    cin >> salt;
    ofstream f;
    f.open("test.txt");
    f << n << endl << salt << endl;
    srand(time(nullptr));
    for (int i = 0; i < n; i++)
    {
        char c[11];
        f << dieci2Sedici(encrypt(random(c), salt)) << endl;
    }
    return 0;
}

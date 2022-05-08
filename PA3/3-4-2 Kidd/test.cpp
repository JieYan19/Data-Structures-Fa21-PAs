//
//  main.cpp
//  test
//
//  Created by Yan on 2022/5/7.
//

#include <iostream>
#include <fstream>
using namespace std;

int main()
{
    long long n = 2147483647;
    int m = 200000;
    ofstream f;
    f.open("test.txt");
    f << n << " " << m << endl;
    srand(time(nullptr));
    for (int i = 0; i < m; ++i)
    {
        switch ((rand() % 2))
        {
        case 0:
            f << "H" << " ";
            break;
        case 1:
            f << "Q" << " ";
            break;
        }
        long long left = rand() % n;
        long long right = rand() % n;
        while (right < left)
            right = rand() % n;
        f << left + 1 << " " << right + 1 << endl;
    }
    return 0;
}

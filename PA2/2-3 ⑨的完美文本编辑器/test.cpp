//
//  main.cpp
//  试验田0409
//
//  Created by Yan on 2022/4/9.
//

#include <iostream>
using namespace std;

int main(int argc, char * argv[])
{
    int m;
    cin >> m;
    int n;
    cin >> n;
    srand(time(nullptr));
    char str[100000];
    for (int i = 0; i < m; ++i)
    {
        switch ((rand() % 3))
        {
        case 1:
            str[i] = 'A' + rand() % 26;
            break;
        case 2:
            str[i] = 'a' + rand() % 26;
            break;
        default:
            str[i] = '0' + rand() % 10;
            break;
        }
        cout << str[i];
    }
    cout << endl << n << endl;
    for (int i = 0; i < n; i++)
    {
        switch ((rand() % 10))
        {
            case 0:
                cout << "< L" << endl;
                break;
            case 1:
                cout << "< R" << endl;
                break;
            case 2:
                cout << "> L" << endl;
                break;
            case 3:
                cout << "> R" << endl;
                break;
            case 4:
                cout << "I L a" << endl;
                break;
            case 5:
                cout << "I R b" << endl;
                break;
            case 6:
                cout << "D L" << endl;
                break;
            case 7:
                cout << "D R" << endl;
                break;
            case 8:
                cout << "R" << endl;
                break;
            case 9:
                cout << "S" << endl;
                break;
        }
    }
}

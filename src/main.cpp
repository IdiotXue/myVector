#include <iostream>
#include "Vector.h"
#include <string>
using namespace std;
using namespace myvector;

int main(int argc, char const *argv[])
{
    for (int i = 0; i < argc; ++i)
        cout << argv[i] << " ";
    cout << endl;
    Vector<int> iTemp(3);
    // try
    // {
    //     iTemp[4] = 0;
    // }
    // catch (const std::range_error &e)
    // {
    //     cout << e.what() << endl;
    // }
    iTemp[0] = 1;
    iTemp[1] = 2;
    iTemp[2] = 3;
    iTemp.pop_back();
    iTemp.push_back(4);
    iTemp.push_back(5);
    const int a = iTemp[2];
    cout << a << endl;
    myvector::Vector<int> iTemp2(iTemp);
    for (int i = 0; i < iTemp2.size(); ++i)
        cout << iTemp2[i] << " ";
    cout << endl;
    Vector<int>::const_iterator const_itr;
    for (const_itr = iTemp.begin(); const_itr != iTemp.end(); ++const_itr)
    {
        // *const_itr = '666';
        cout << *const_itr << " ";
    }
    cout << endl;

    Vector<string> iStr(3);
    iStr[0] = "hello";
    iStr[1] = "hi";
    iStr[2] = "nihao";
    iStr.push_back("gaoxiao");
    Vector<string>::iterator itr;
    for (itr = iStr.begin(); itr != iStr.end(); itr++)
    {
        // *itr = "test";
        cout << *itr << " ";
    }
    cout << *itr <<endl;
    cout << endl;
    cout << iStr[0] << endl;
}
#include <iostream>
#include <Vector.h>
using namespace std;

int main(int argc, char const *argv[])
{
    for (int i = 0; i < argc; ++i)
        cout << argv[i] << " ";
    cout << endl;
    myvector::Vector<int> iTemp(3);
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
}
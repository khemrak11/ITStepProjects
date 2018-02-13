#include <iostream>

using namespace std;

int Divide(int dividend, int divisor, int &quotient, int &remainder)
{
    quotient = dividend / divisor;
    remainder = dividend % divisor;
    return 0;
}

const int monthsInAYear = 10;
int month[monthsInAYear][2];

//Using function as reference
int &SetValue(int i){
    return month[i][1];
}

int main(){
    int a = 5;
    int *addressA = &a;
    int &b = a;
    int *addressB = &b;

    cout << "a has a value of " << a << " at " << addressA << endl;
    cout << "b has a value of " << b << " at " << addressB << endl;

    int m = 10;
    int n = 3;
    int q, r;
    Divide(m, n, q, r);
    cout << m << " divide by " << n << " is " << q << " with a remainder of " << r << endl;

    for (int i = 0; i < monthsInAYear; i++)
    {
        month[i][0] = i + 1;

        //month[i][1] = i * 3 + i - 6;
        //Another way of saying the above is seen below
        SetValue(i) = i * 3 + i - 6;
        
        cout << "Value at " << month[i][0] << " is " << SetValue(i) << endl;
    }


    return 0;
}
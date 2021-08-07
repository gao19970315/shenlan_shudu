#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
bool yes_or_no(vector<int> &ns, const int num1, const int num2);
int main()
{
    int n, a_mod, b_mod;
    int judge_num, num1, num2, num3;
    cin >> n;
    vector<int> ns;
    vector<int> xx;
    while (n)
    {
        cin >> judge_num;
        if (judge_num == 1)
        {
            cin >> num1 >> num2;
            ns.push_back(num1);
            ns.push_back(num2);
        }
        else if (judge_num == 2)
        {
            cin >> num1 >> num2>>num3;
            a_mod = num1 % num3;
            b_mod = num2 % num3;
            if (yes_or_no(ns, a_mod, b_mod))
                xx.push_back(1);
            else
                xx.push_back(0);
        }
        else
            cout<<"错误";
        judge_num=num1=num2=num3=0;
        n--;
    }
    for(auto const &it:xx)
    {
        if(it==1)
            cout<<"yes\n";
        else
            cout<<"No\n";
    }
    return 0;
}

bool yes_or_no(vector<int> &ns, const int num1, const int num2)
{
    if(num1==num2)
        return true;
    auto it =std::find(ns.begin(), ns.end(), num1);
    auto it2=std::find(ns.begin(), ns.end(), num2);
    if (it!=ns.end()&&it2!=ns.end())
        return true;
    else
        return false; 
}
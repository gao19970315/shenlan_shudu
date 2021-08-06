#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
int ans = 0;
using namespace std;

int num[9][9]; //单个解
int first_num;
class Array2D
{
public:
    int num2[9][9];
    Array2D(decltype(num) temp_num)//二维数组拷贝函数
    {
        for (int i = 0; i < 9; i++)
            for (int j = 0; j < 9; j++)
                num2[i][j] = temp_num[i][j];
    }
};

//创建9*9的数独矩阵

vector<Array2D> Ans; //存储所有的数独解
bool flag = false;   //解的标志

int first_num_zero(decltype(num) temp_num)
{
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            if (temp_num[i][j] == 0)
                return (i * 9 + j);
}

//检查第一个为0的数，是否填入了9
bool check_first_num(decltype(num) temp_num, int first_num)
{
    if (num[first_num / 9][first_num % 9] == 9)
        return true;
    else
        return false;
}

void Input(const char *file)
{
    ifstream ifs(file);
    if (!ifs)
    {
        cout << "未打开" << endl;
        exit(0);
    }
    int i, j;
    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 9; j++)
        {
            ifs >> num[i][j];
        }
    }
}

//检测输入的数是否满足数独条件

bool Check(int n, int key)
{
    //判断key填入n时是否满足条件，key为1~9的其中一个，n为0~80的其中一个
    int i, j;
    for (j = 0; j < 9; j++)
    {
        //判断n所在行是否合法
        i = n / 9;
        if (num[i][j] == key)
        {
            return false;
        }
    }

    for (i = 0; i < 9; i++)
    {
        //判断n所在列是否合法
        j = n % 9;
        if (num[i][j] == key)
        {
            return false;
        }
    }

    int x = n / 9 / 3 * 3; //小九宫格左上角横坐标
    int y = n % 9 / 3 * 3; //小九宫格左上角纵坐标
    //算数运算符/ %都为从左往右算，这样得到的x，y一定都为3的倍数
    for (i = x; i < x + 3; i++)
    {
        for (j = y; j < y + 3; j++)
        {
            if (num[i][j] == key)
            {
                return false;
            }
        }
    }

    return true; //全部合法
}

int DFS(int n)
{
    /*     static int count = 0;
    count++;
    cout << "第" << count << "迭代\n";
    for (auto &ii : num)
    {
        for (auto &jj : ii)
            cout << jj << " ";
        cout << "\n";
    } */

    //深度搜索构造数独,当元素数为81时，即认为已经构造好数独，所有的都符合条件
    if (n > 80)
    {
        
        flag = true;
        Ans.push_back(Array2D(num)); //存入Ans解的集合
        return 0;
    }

    if (num[n / 9][n % 9] != 0)
    {
        //如果不为0，跳过,即只处理为0数据
        DFS(n + 1);
    }
    else
    {
        //反之，对该位置进行填数
        for (int i = 1; i <= 9; i++)
        {
            //填入的数字为1~9的其中一个
            if (Check(n, i) == true)
            {
                num[n / 9][n % 9] = i; //赋值
                DFS(n + 1);            //继续搜索
                if (flag == true && false)//不主动退出，若只搜索一个解，将&&flase删除
                {
                    return 0;
                }
                num[n / 9][n % 9] = 0; //构造不成功还原当前位
            }
        }
    }
}

void Output()
{
    //输出9*9的数独矩阵
    cout << endl
         << "完成后的9*9数独：" << endl;
    int i, j;
    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 9; j++)
        {
            cout << num[i][j] << " ";
            if (j % 3 == 2)
            {
                cout << "  ";
            }
        }
        cout << endl;
        if (i % 3 == 2)
        {
            cout << endl;
        }
    }
}

int main(int argc, char **argv)
{
    Input("hard++.txt");
    first_num = first_num_zero(num);
    std::chrono::time_point<std::chrono::system_clock> server_create_time_ = std::chrono::system_clock::now();
    DFS(0); //共9*9=81个元素，下标为0~80
    cout << "二维数组\n";
    cout << "解的个数： " << Ans.size() << "\n";
    /*     for (auto count = 0; count < Ans.size(); count++)
    {
        cout << "第" << count + 1 << "个解： \n";
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                cout << Ans.at(count).num2[i][j] << " ";
            }
            cout << "\n";
        }
        cout << "\n";
    } */
/*     for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            cout << Ans.at(0).num2[i][j] << " ";
        }
        cout << "\n";
    } */
    //Output();
    std::cout << "solution_count = " << Ans.size() << std::endl;
    auto current_time = std::chrono::system_clock::now();
    std::chrono::duration<double, std::milli> duration_mili = current_time - server_create_time_;
    std::cout << "PrintDuration : duration_mili duration = " << (long)duration_mili.count() << " ms";
}

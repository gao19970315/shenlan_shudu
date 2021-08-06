#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
using namespace std;

//声明
void Input(const char *);
class Array1D;

//精确覆盖问题的定义：给定一个由0-1组成的矩阵，是否能找到一个行的集合，使得集合中每一列都恰好包含一个1
const int MN = 9 * 9 * 9 + 10;                      //最大行数,共有9*9个格子，每个格子可以放1~9
const int MM = 9 * 9 + 9 * 9 + 9 * 9 + 9 * 9 + 100; //最大列数
//用第(i - 1) * 9 + j列为1表示i行j列的已经填数。一共占用81列。
//用81 + (i - 1) * 9 + v列表示第i行已经有v这个值。一共占用81列。
//用162 + (j - 1) * 9 + v列表示第j列已经有v这个值。一共占用81列。
//用243 + (3 * ((i - 1) / 3) + (j + 2) / 3 - 1) + v列表示第3*((i - 1) / 3) + (j + 2) / 3宫格已经有v这个值。一共占用81列。
const int MNN = MN * MM; //最大点数

//全局变量定义
int path[81];
int num[9][9];
int n; //元素个数
int count;//记录解的个数
vector<Array1D> answers;//解的集合

//结构体的定义
struct node
{
    int r, c, v;
} nds[MN];

class Array1D
{
public:
    int path[81];
    Array1D(decltype(path) temp_num)
    {
        for (int i = 0; i < 81; i++)
            path[i] = temp_num[i];
    }
};

//舞蹈链定义
struct DLX
{
    int n, m, si; //n行数,m列数,si目前有的节点数
                  //十字链表组成部分
    int U[MNN], D[MNN], L[MNN], R[MNN], Row[MNN], Col[MNN];
    //第i个结点的U向上指针D下L左R右，所在位置Row行Col列
    int H[MN], S[MM];         //记录行的选择情况和列的覆盖情况
    int ansd, ans[MN];        //记录满足要求的行的个数，和行的位置
    void init(int _n, int _m) //初始化空表
    {
        n = _n;
        m = _m;
        for (int i = 0; i <= m; i++) //初始化第一横行（表头）
        {
            S[i] = 0;
            U[i] = D[i] = i; //目前纵向的链是空的
            L[i] = i - 1;
            R[i] = i + 1; //横向的连起来
        }
        R[m] = 0; //循环链，收尾相连
        L[0] = m;
        si = m; //表头结点也是结点，目前用了前0~m个结点

        //下面这一步没看懂
        for (int i = 1; i <= n; i++)
            H[i] = -1;
    }
    void link(int r, int c) //插入点(r,c)形参r,c即row，col
    {
        //行列
        ++S[Col[++si] = c]; //si++：元素数+1；Col[si]=c：新插入的si结点的为c列;S[c]++：c列的元素数+1;
        Row[si] = r;        //si该结点的行数为r
        //上下连接
        D[si] = D[c]; //向下指向c的下面的第一个结点，原来C下面的元素现在变成si下面了
        U[si] = c;    //si的上面为列指针
        U[D[c]] = si; //c的下面的第一个结点的上面为si
        D[c] = si;    //列指针指向的第一个该列中的元素设为si
        if (H[r] < 0) //如果第r行没有元素
            H[r] = L[si] = R[si] = si;
        else
        {
            R[si] = R[H[r]]; //si的右边为行指针所指的右边第一个元素
            L[R[H[r]]] = si; //行指针所指的右边第一个元素的左侧为si
            L[si] = H[r];    //si的左侧为行指针
            R[H[r]] = si;    //行指针的右侧为si
        }
    }
    void remove(int c) //列表中删掉c列
    {
        L[R[c]] = L[c]; //表头操作  //c列头指针的右边的元素的左侧指向c列头指针左边的元素
        R[L[c]] = R[c]; //c列头指针的左边的元素的右侧指向c列头指针右边的元素

        for (int i = D[c]; i != c; i = D[i])     //遍历该列的所有元素，方向从上到下
            for (int j = R[i]; j != i; j = R[j]) //方向从左到右
            {                                    //对于该列的某个元素所在的行进行遍历
                U[D[j]] = U[j];                  //把该元素从其所在列中除去
                D[U[j]] = D[j];
                --S[Col[j]]; //该元素所在的列数目减一
            }
    }
    void resume(int c) //恢复c列
    {
        for (int i = U[c]; i != c; i = U[i])     //枚举该列元素 方向 从下往上恢复
            for (int j = L[i]; j != i; j = L[j]) //枚举该列元素所在的行 方向：从右往左
                ++S[Col[U[D[j]] = D[U[j]] = j]]; //D[U[j]]=j;U[D[j]]=j;S[Col[j]]++;
        L[R[c]] = R[L[c]] = c;                   //c列头指针左右相连
    }
    bool dance(int d) //选取了d行
    {
        if (R[0] == 0) //全部覆盖了 即head=0，表头自己是自己
        {
            //全覆盖了之后的操作
            // ansd = d;
            // int tem_path[81];
            // for (int i = 0; i < this->ansd; i++)
            // {
            //     int posr = this->ans[i];                                         //即ans里存的是满足要求的行的下标
            //     tem_path[(nds[posr].r - 1) * 9 + nds[posr].c - 1] = nds[posr].v; //通过行的下标去访问最初对应的二维数组的行，列，值
            // }
            // answers.push_back(tem_path); //转换成一维数组后，存入解的集合
            count++;//只记录解的个数，不保存解的形式
            return 1; //开始回退
        }
        int c = R[0];                        //表头结点指向的第一个列
        for (int i = R[0]; i != 0; i = R[i]) //枚举列头指针
            if (S[i] < S[c])                 //找到列中元素个数最少的，从元素中个数最少的开始，优化选项
                c = i;
        remove(c); //将该列删去
        for (int i = D[c]; i != c; i = D[i])
        {                                        //枚举该列的元素
            ans[d] = Row[i];                     //记录该列元素的行
            for (int j = R[i]; j != i; j = R[j]) //删除该元素的右元素所在的行
                remove(Col[j]);                  //将该列的某个元素的行上的元素所在的列都删去
            if (dance(d + 1) && false)           //当所有解到找出来之后，即不主动退出，待迭代完所有行和列和自动退出
                return 1;
            for (int j = L[i]; j != i; j = L[j]) //失败了，则恢复J列
                resume(Col[j]);
        }
        resume(c); //恢复C列
        return 0;  //Dance函数结束
    }
} dlx;

int main()
{
    Input("hard++.txt");
    //舞蹈链初始化
    while (n < 81)
    {
        dlx.init(9 * 9 * 9, 9 * 9 * 4); //初始化,左参数为行数，右参数为列数列数
        int r = 1;                      //第一行开始
        //构建舞蹈链参数
        for (int i = 1; i <= 9; i++)
        {
            for (int j = 1; j <= 9; j++)
            {
                //若是0，则插入9行
                if (num[n / 9][n % 9] == 0)
                {
                    for (int z = 1; z <= 9; z++)
                    {
                        dlx.link(r, (i - 1) * 9 + j);
                        dlx.link(r, 81 + (i - 1) * 9 + z);
                        dlx.link(r, 162 + (j - 1) * 9 + z);
                        dlx.link(r, 243 + (((i - 1) / 3) * 3 + (j + 2) / 3 - 1) * 9 + z);
                        nds[r].r = i, nds[r].c = j, nds[r].v = z; //存入数组原位置的行数，列数，以及值
                        r++;                                      //行数+1
                    }
                }
                //若已给定元素，则插入一行
                else
                {
                    int z = num[n / 9][n % 9]; //num[n / 9][n % 9]，为数独中设定的已存在的值
                    dlx.link(r, (i - 1) * 9 + j);
                    dlx.link(r, 81 + (i - 1) * 9 + z);
                    dlx.link(r, 162 + (j - 1) * 9 + z);
                    dlx.link(r, 243 + (((i - 1) / 3) * 3 + (j + 2) / 3 - 1) * 9 + z);
                    nds[r].r = i, nds[r].c = j, nds[r].v = z;
                    r++;
                }
                n++;
            }
        }
    }
    dlx.ansd = -1;//行数初始化
    std::chrono::time_point<std::chrono::system_clock> server_create_time_ = std::chrono::system_clock::now();
    dlx.dance(0);
    //@解的遍历
    /*   int counts = 0;
     for (auto const &i : answers)
    {
        counts++;
        if (counts > 100)
            break; 
        cout << "第" << counts << "个解： \n";
        for (int j = 0; j < 9; j++)
        {
            for (int jj = 0; jj < 9; jj++)
                cout << i.path[j * 9 + jj] << " ";
            cout << "\n";
        }
    } */
    /*     cout << "第1个解： \n";
    for (int j = 0; j < 9; j++)
    {
        for (int jj = 0; jj < 9; jj++)
            cout << answers.at(0).path[j * 9 + jj] << " ";
        cout << "\n";
    }
    cout << "第2个解： \n";
    for (int j = 0; j < 9; j++)
    {
        for (int jj = 0; jj < 9; jj++)
            cout << answers.at(1).path[j * 9 + jj] << " ";
        cout << "\n";
    } */
    //计时程序
    std::cout << "solution_count = "<< count<<"\n";
    std::cout << "solution_count = " << answers.size() << std::endl;
    auto current_time = std::chrono::system_clock::now();
    std::chrono::duration<double, std::milli> duration_mili = current_time - server_create_time_;
    std::cout << "PrintDuration : duration_mili duration = " << (long)duration_mili.count() << " ms";
    return 0;
}

void Input(const char *file)
{
    ifstream ifs(file);
    if (!ifs)
    {
        cout << "未打开" << endl;
        exit(0);
    }
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            ifs >> num[i][j];
        }
    }
}

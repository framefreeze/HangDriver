/*
 * 这是最终打表得出的函数
 * designed by 于济凡
 * 2017.2.27
 */
#include <iostream>
#include <fstream>
using namespace std;

int next_Node[50][50]={0};

ifstream fin("/Users/yujifan/Desktop/next_node_info.txt");

void init()
{
    int x,y,next;
    for (int i = 0; i < 753; ++i) {
        fin>>x>>y>>next;
        next_Node[x][y]=next;
    }
}

void output(int start, int final)
{
    int x,y;
    if(next_Node[start][final]!=0)
    {
        cout<<next_Node[start][final]<<endl;
    }
    else
    {
        printf("No existed access!");
    }
}

int main()
{
    int x,y;
    init();
    while (cin>>x>>y)
    {
        output(x,y);
    }
    return 0;
}

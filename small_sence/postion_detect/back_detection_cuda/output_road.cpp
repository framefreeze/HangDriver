/*
 * 这是最终打表得出的函数
 * designed by 于济凡
 * 2017.2.27
 */
#include <iostream>
#include <fstream>
using namespace std;

int next_Node[50][50]={0};
//int route[100]={0};
//int counter = 0;
int direction;//direction 1,2,3,4 according to straight,left,right & turning around
int dirtmp = 0;// temporary direction of 19th area
int x,y; //for input start and final

ifstream fin("/home/framefreeze/Documents/HangDriver/lane_mentor/necessary_files/next_node_info.txt");

void init()
{
    int x,y,next;
    for (int i = 0; i < 753; ++i) {
        fin>>x>>y>>next;
        next_Node[x][y]=next;
    }
}

int direct(int start, int final)
{
    if(start == 13||start == 22||start == 24)
    {
        direction = 3;
    }
    else if(start == 16||start == 25||start == 26)
    {
        direction = 2;
    }
    else if(start == 10)
    {
        if(next_Node[start][final] == 8||next_Node[start][final] == 7)
        {
            direction = 1;
        }
        else if(next_Node[start][final] == 40||(next_Node[start][final] == 14))
        {
            direction = 3;
        }
    }
    else if(start == 6)
    {
        if(next_Node[start][final] == 4||next_Node[start][final] == 3)
        {
            direction = 1;
        }
        else if(next_Node[start][final] == 31)
        {
            direction = 3;
        }
    }
    else if(start == 4)
    {
        if(next_Node[start][final] == 2||next_Node[start][final] == 1)
        {
            direction = 1;
        }
        else if(next_Node[start][final] == 31)
        {
            direction = dirtmp;
        }
    }
    else if(start == 17)
    {
        if(next_Node[next_Node[start][final]][final] == 20)
        {
            direction = 1;
        }
        else if(next_Node[next_Node[start][final]][final] == 28)
        {
            direction = 2;
        }
        else if(next_Node[next_Node[start][final]][final] == 30)
        {
            direction = 3;
        }
        else if(next_Node[next_Node[start][final]][final] == 18)
        {
            direction = 4;
        }
        dirtmp = direction;
    }
    else if(start == 32)
    {
        if(next_Node[next_Node[start][final]][final] == 20)
        {
            direction = 3;
        }
        else if(next_Node[next_Node[start][final]][final] == 28)
        {
            direction = 1;
        }
        else if(next_Node[next_Node[start][final]][final] == 30)
        {
            direction = 4;
        }
        else if(next_Node[next_Node[start][final]][final] == 18)
        {
            direction = 2;
        }
        dirtmp = direction;
    }
    else if(start == 21)
    {
        if(next_Node[next_Node[start][final]][final] == 20)
        {
            direction = 4;
        }
        else if(next_Node[next_Node[start][final]][final] == 28)
        {
            direction = 3;
        }
        else if(next_Node[next_Node[start][final]][final] == 30)
        {
            direction = 2;
        }
        else if(next_Node[next_Node[start][final]][final] == 18)
        {
            direction = 1;
        }
        dirtmp = direction;
    }
    else if(start == 29)
    {
        if(next_Node[next_Node[start][final]][final] == 31)
        {
            direction = 4;
        }
        else if(next_Node[next_Node[start][final]][final] == 2)
        {
            direction = 3;
        }   
        dirtmp = direction;
    }
    else if(start == 27)
    {
        if(next_Node[next_Node[start][final]][final] == 20)
        {
            direction = 2;
        }
        else if(next_Node[next_Node[start][final]][final] == 28)
        {
            direction = 4;
        }
        else if(next_Node[next_Node[start][final]][final] == 30)
        {
            direction = 1;
        }
        else if(next_Node[next_Node[start][final]][final] == 18)
        {
            direction = 3;
        }
        dirtmp = direction;
    }
    else if(start == 19)
    {
        direction = dirtmp;
        dirtmp = 0;
    }
    else
    {
        direction = 1;
    }
    return direction;
}

void output(int start, int final)
{
    if(next_Node[start][final]!=0)
    {
        cout<<next_Node[start][final]<<endl;
    }
    else
    {
        printf("No existed access!\n");
    }
}

int output_route_with_direction(int x,int y, bool& can_change)
{
    init();
    if(x != y)
    {
        output(x,y);
        cout << "direction is :" << direct(x,y)<<endl;
        can_change = false;
        return direct(x,y);
        
    }
    else
    {
        cout << "Arrival !"<<endl;
        can_change = true;
    }
}
/*
int main()
{
    output_route_with_direction();
    return 0;
}
*/

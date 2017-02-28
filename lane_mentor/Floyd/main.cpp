#include<cstdio>
#include<algorithm>
#include<cstdlib>
#define MAX_NUM 1<<15

int* Floyd(int* ori_Paths,int* throug_Node,int num_Of_Node)
{
    int* new_Paths=(int*)malloc(num_Of_Node*num_Of_Node*sizeof(int));
    for(int i=0;i<num_Of_Node*num_Of_Node;i++) new_Paths[i]=ori_Paths[i];
    for(int k=0;k<num_Of_Node;k++)
    {
        for(int i=0;i<num_Of_Node;i++)
        {
            for(int j=0;j<num_Of_Node;j++)
            {
                if(new_Paths[i*num_Of_Node+j]>(new_Paths[i*num_Of_Node+k]+new_Paths[k*num_Of_Node+j]))//判断i到j经过k是否比直达更近
                {
                    new_Paths[i*num_Of_Node+j]=new_Paths[i*num_Of_Node+k]+new_Paths[k*num_Of_Node+j];
                    throug_Node[i*num_Of_Node+j]=throug_Node[i*num_Of_Node+k];
                }
            }
        }
    }
    return new_Paths;
}

void inti(int*paths,int* throug_Nodes,int num_Of_Node)
{
    for(int i=0;i<num_Of_Node;i++)
    {
        for(int j=0;j<num_Of_Node;j++)
        {
            paths[i*num_Of_Node+j]=MAX_NUM;
            if(i==j) paths[i*num_Of_Node+j]=0;
            throug_Nodes[i*num_Of_Node+j]=j;
        }

    }
}
void print_Path(int start_Node,int end_Node,int num_Of_Node,int* throug_Nodes)
{
    printf("path is:");
    int tmp=start_Node;
    while(tmp!=end_Node)
    {
        if(tmp!=start_Node) printf("->");
        printf("%d",tmp);
        tmp=throug_Nodes[tmp*num_Of_Node+end_Node];
    }
    printf("->%d\n\n",end_Node);
}

void print_Node(int start_Node,int end_Node,int num_Of_Node,int* throug_Nodes)
{
    int tmp=start_Node;
    while(tmp!=end_Node)
    {
        if(tmp!=start_Node) printf("->");
        printf("%d",tmp);
        tmp=throug_Nodes[tmp*num_Of_Node+end_Node];
    }
    printf("->%d\n\n",end_Node);
}

int main()
{
    int num_Of_Node;
    int num_Of_Route;
    printf("Please Input number of nodes and routes:");
    scanf("%d %d",&num_Of_Node,&num_Of_Route);

    int* paths=(int*)malloc(num_Of_Node*num_Of_Node*sizeof(int));
    int* throug_Nodes=(int*)malloc(num_Of_Node*num_Of_Node*sizeof(int));
    inti(paths,throug_Nodes,num_Of_Node);


    printf("Please Input number all route:\n");
    for(int i=0;i<num_Of_Route;i++)
    {
        int start_Node,end_Node,length;
        scanf("%d %d %d",&start_Node,&end_Node,&length);
        paths[start_Node*num_Of_Node+end_Node]=length;
        paths[end_Node*num_Of_Node+start_Node]=length;
    }

    int* new_Paths=Floyd(paths,throug_Nodes,num_Of_Node);
    for(int i=0;i<num_Of_Node;i++)
    {
        for(int j=0;j<num_Of_Node;j++)
        {
            if(i!=j)
            {
                 printf("The shortest lengths :%d->%d=%d\n",i,j,new_Paths[i*num_Of_Node+j]);
                 print_Path(i,j,num_Of_Node,throug_Nodes);
            }

        }
    }
}
/*
6 9
0 1 3
0 3 4
0 5 5
1 2 1
1 5 5
2 3 5
3 1 3
4 3 3
4 5 2
*/
/*
5 7
0 1 5
0 4 7
1 2 4
4 2 8
1 3 2
2 3 6
4 3 1
*/
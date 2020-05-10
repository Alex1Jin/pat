#include <iostream>
#define MaxSize 501
const int INF=0x7fffffff;       //定义一个很大的数
using namespace std;
struct Edge{
   int cost;   //花费
   int use;   //是否可以使用
};
Edge Graph[MaxSize][MaxSize];     //存放图
int cost_conquered[MaxSize];    //表示该城市被占领会需要花费多少
int lowcost[MaxSize];      //存放该节点到最小生成树的花费
int closeset[MaxSize];      //存放最小花费和当前相连的节点
int join[MaxSize];          //表示这个点是否有道路相连
int N,M;      //分别表示总的城市个数和连接城市的边
void Input(){
    int temp_s,temp_d,temp_cost,temp_use;
    for(int i=0;i<M;i++){
        cin>>temp_s>>temp_d>>temp_cost>>temp_use;
        Graph[temp_s][temp_d].cost=temp_cost;
        Graph[temp_s][temp_d].use=temp_use;
        Graph[temp_d][temp_s].cost=temp_cost;
        Graph[temp_d][temp_s].use=temp_use;
    }
}
//这里注意，其实也不是真正意义上的最小生成树，如果没有相连的就找最小的边，连起来就直接加进去
void Prim(int v,int u){
    int Min,index;
    lowcost[v]=0;
    for(int i=1;i<=N;i++){
        if(lowcost[i]!=0)   {
                lowcost[i]=Graph[v][i].cost;
                closeset[i]=v;
        }
        if(Graph[v][i].use==1){
            join[i]=1;
        }
    }
    for(int i=1;i<=N-2;i++){     //找出剩下的节点
        Min=INF;
        index=-1;
        for(int j=1;j<=N;j++){
            if(lowcost[j]!=0&&lowcost[j]<Min&&Graph[closeset[j]][j].use==1){   //找到一个和当前节点相连并且这条边没有被破坏
                Min=lowcost[j];
                index=j;
            }
        }
        if(index==-1){   //如果找不到这个点，则说明当前的点都没办法连
            Min=INF;
            for(int j=1;j<=N;j++){
                 if(lowcost[j]!=0&&lowcost[j]<Min){
                    Min=lowcost[j];
                    index=j;
                 }
            }
            cost_conquered[u]+=Min;
        }
        if(index==-1){    //剩下的节点没办法连通
            cost_conquered[u]=INF;
            return;
        }
        lowcost[index]=0;
        for(int j=1;j<=N;j++){    //当前节点加入之后，修改相应的lowcost和closeset
            if(Graph[index][j].cost<lowcost[j]||(join[j]==0&&Graph[index][j].use==1&&lowcost[j]!=0)){
                join[j]=1;
                lowcost[j]=Graph[index][j].cost;
                closeset[j]=index;
            }
        }
    }
}
int main()
{
    int Max=0;
    memset(cost_conquered,0,sizeof(cost_conquered));
    int Count=0;
    cin>>N>>M;
    for(int i=1;i<=N;i++){
        for(int j=1;j<=N;j++){
            Graph[i][j].cost=INF;
            Graph[i][j].use=0;
        }
    }
    Input();
    for(int i=1;i<=N;i++){
        memset(join,0,sizeof(join));
        for(int j=1;j<=N;j++){
            lowcost[j]=INF;
            closeset[j]=-1;
        }
        lowcost[i]=0;   //表示这个点已经排除
        if(i==1){
            Prim(2,1);    //当考虑第一个节点，那么第一个加入的节点就不能是1
        }
        else{
            Prim(1,i);    //当考虑剩下的节点时，那么首节点只要不和自己相同即可，所以选择了1
        }
        if(cost_conquered[i]>Max){
            Max=cost_conquered[i];
        }
    }
    if(Max==0){
        cout<<'0';
    }
    else{
        for(int i=1;i<=N;i++){
            if(cost_conquered[i]==Max){
               Count++;
            }
        }
        int i=1;
        while(Count>1){
            if(cost_conquered[i]==Max){
                cout<<i<<" ";
                Count--;

            }
            i++;
        }
        for(;i<=N;i++){
            if(cost_conquered[i]==Max){
                cout<<i;
            }
        }
    }
    return 0;
}


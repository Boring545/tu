#include<iostream>
#include<stdio.h>
#include<deque>
using namespace std;
typedef int Elemtype;
//图的邻接矩阵
typedef struct mGraph {
	Elemtype** a;
	int n;//图节点数
	int e;//图边数
	Elemtype noEdge;
}MGraph;
//初始化图
bool InitMGraph(MGraph* mg, int size, Elemtype noEdgeValue) {
	int i, j;
	if (size == 0)
		return 0;
	mg->a = (Elemtype**)malloc(size*sizeof(Elemtype*));//构建第一层邻接矩阵
	if (mg->a == NULL)
	{
		printf("Error_InitGraph:分配内存失败\n");
		return 0;
	}
	mg->n = size;
	mg->noEdge = noEdgeValue;
	mg->e = 0;
	for (i = 0;i < size;i++)
	{
		mg->a[i] = (Elemtype*)malloc(size*sizeof(Elemtype));
		if (mg->a[i] == NULL)
		{
			printf("Error_InitGraph:分配内存失败,位置%d\n",i);
			return 0;
		}
		for (j = 0;j < size;j++)
			mg->a[i][j] = noEdgeValue;
		mg->a[i][i] = 0;
	}
	return 1;
}
//释放图空间xx
void DestroyMGraph(MGraph* mg) {
	int i;
	for (i = 0;i < mg->n;i++)
		free(mg->a[i]);
	free(mg->a);
}
//判断边是否存在
bool ExistMGraph(MGraph* mg, int n1, int n2) {
	if (n1<0 || n2<0 || n1>mg->n - 1 || n2>mg->n - 1 ||n1==n2)
	{
		printf("边不存在\n");
		return 0;
	}
	else if (mg->a[n1][n2] == mg->noEdge)
	{
		//printf("参数无效\n");
		return 1;
	}
	else
		return 1;
}
//在图上插入<n1,n2>为序偶，权值为w的边(权值默认为1)，返回1；
bool InsertMGraph(MGraph* mg, int n1, int n2, Elemtype w=1) {
	if (ExistMGraph(mg,n1,n2)==0)
	{
		if (mg->a[n1][n2] != mg->noEdge)
			printf("边已存在\n");
		printf("Error_InsertGraph:插入失败\n");
		return 0;
	}
	else
	{
		mg->a[n1][n2] = w;
		mg->e++;
		return 1;
	}
}
//在图上删除以<n1, n2>为序偶的边
bool RemoveEdgeM(MGraph* mg, int n1, int n2)
{
	if (n1<0 || n2<0 || n1>mg->n - 1 || n2>mg->n - 1 || n1 == n2||mg->a[n1][n2]==mg->noEdge)
	{
		printf("Error_InsertGraph:删除失败\n");
		return 0;
	}
	else
	{
		mg->a[n1][n2] = mg->noEdge;
		mg->e--;
		return 1;
	}
}
//图的深度优先遍历
void MDFS(int v, int visited[], MGraph* mg) {//执行一次获得一条序列
	int x,temp;
	printf("%d ", v);//打印第一个节点v
	while(visited[v] < mg->n)
	{       //定位
			while (visited[v] < mg->n && (mg->a[v][visited[v]] == mg->noEdge || mg->a[v][visited[v]] == 0 || visited[visited[v]] != 0))//在visited没有越界的前提下,在没有边或u等于v或visited[v]行已经被访问的情况下visited向后移位，直到找到合理的visited值
			{
				visited[v]++;
			}
			if(visited[v] < mg->n)//当当前行有对应边
			{
				printf("(%d) ", mg->a[v][visited[v]]);//打印边权值
				printf("%d ", visited[v]);//打印后一个节点
				temp = visited[v];
				visited[v]++;
				v = temp;
			}
	}
	printf("\n");
}
void DFSMGraph(MGraph *mg) {
	int i;
	int* visited = (int*)malloc(mg->n * sizeof(int));
	memset(visited, 0, sizeof(int)*mg->n);//数组整体赋值0
	printf("图的深度优先遍历\n");
	for (i = 0;i < mg->n;i++)
	{
		if (visited[i]==0)//visited[i] ==0表示此行未被访问
			MDFS(i, visited, mg);
	}
	free(visited);
}
//图的宽度优先遍历
void MBFS(int v, int visited[], MGraph* mg) {
	deque<int> q;
	int i;
	q.push_back(v);
	visited[v] = 0;//赋值为0代表这行已经被访问过，具体为执行了入队的操作，-1代表未访问过
	visited[visited[v]] = 0;
	while (q.empty()==0)
	{
		v=q.front();//拿到队列头元素
		q.pop_front();//删除这个元素
		printf("%d ", v);
		while (visited[v] < mg->n)
		{
			if (mg->a[v][visited[v]] != 0 && mg->a[v][visited[v]] != mg->noEdge && visited[visited[v]]==-1)//找到当前节点v的没有访问过的邻节点
			{
				visited[visited[v]] = 0;
				q.push_back(visited[v]);//把visited[v]对应元素插入队列尾
			}
			visited[v]++;
		}
	}
	printf("\n");
}
void BFSMGraph(MGraph* mg) {
	int i;
	int* visited = (int*)malloc(mg->n * sizeof(int));
	memset(visited, -1, sizeof(int) * mg->n);//数组整体赋值-1
	printf("图的宽度优先遍历\n");
	for (i = 0;i < mg->n;i++)
	{
		if (visited[i] ==-1)//visited[i]==-1表示此行未被访问
			MBFS(i, visited, mg);
	}
	free(visited);
}
int main()
{
	int size=0,n1,n2,w;
	MGraph mg;
	printf("输入节点数：");
	scanf_s("%d", &size);
	InitMGraph(&mg,size,2147483647);
	printf_s("\n输入节点节点n1之间的边：(格式为：节点1 节点2 权值)\n");
	while(cin >> n1 >> n2 >> w)
	{
		InsertMGraph(&mg, n1, n2, w);
	}
	DFSMGraph(&mg);
	BFSMGraph(&mg);
	DestroyMGraph(&mg);
	return 1;
}
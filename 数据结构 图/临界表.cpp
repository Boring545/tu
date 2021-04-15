#include<iostream>
#include<stdio.h>
#include<deque>
#include<stack>
#define INFTY 2147483647
using namespace std;
typedef int Elemtype;
typedef struct eNode {
	int adjvex;//序偶中的后一个元素
	Elemtype w;//边的权值
	struct eNode* nextarc;//指向下一个边界点
}ENode;
typedef struct lGraph {
	int n;//图的节点数
	int e;//图的边数
	ENode** a;//指向序偶中的后一个元素节点的指针数组
}LGraph;
//初始化邻接表
bool InitLGraph(LGraph* lg, int size)
{
	int i;
	if (size == 0)
		return 0;
	lg->a = (ENode**)malloc(size * sizeof(ENode*));
	if (lg->a == NULL)
	{
		printf("Error_InitLGraph:分配空间失败\n");
		return 0;
	}
	else
	{
		lg->n = size;
		lg->e = 0;
		for (i = 0;i < lg->n;i++)
			lg->a[i] = NULL;
		return 1;
	}
}
//释放邻接表空间
void DestroyLGraph(LGraph* lg) {
	int i;
	ENode* p, * q;
	for (i = 0;i < lg->n;i++)
	{
		p = lg->a[i];
		q = p;
		while (p!=NULL)
		{
			p = p->nextarc;
			free(q);
			q = p;
		}
	}
	free(lg->a);
}
//查找边是否存在
bool ExistLGraph(LGraph* lg, int n1, int n2) {
	ENode* p;
	if (n1<0 || n2<0 || n1>lg->n - 1 || n2>lg->n - 1 || n1 == n2)//越界或自回路
	{
		printf("Error_ExistLGraph:参数错误\n");
		return 0;
	}
	else
	{
		p = lg->a[n1];//定位到n1行
		while (p != NULL&&p->adjvex!=n2)//找<n1,n2>存在的序偶对
			p = p->nextarc;
		if (p == NULL)//边不存在
		{
			return 1;
		}
		else//边已存在
		{
			printf_s("边<%d,%d>已经存在",n1,n2);
			return 0;
		}
	}
}
//在图上插入<n1,n2>为序偶，权值为w的边
bool InsertLGraph(LGraph* lg, int n1, int n2, Elemtype w) {
	ENode* p;
	if (ExistLGraph(lg,n1,n2)==0)
	{
		printf("Error_InsertGraph:插入失败\n");
		return 0;
	}
	p = (ENode*)malloc(sizeof(ENode));
	p->w = w;
	p->adjvex = n2;
	p->nextarc = lg->a[n1];
	lg->a[n1] = p;
	lg->e++;
	return 1;
}
//在图上删除一个边
bool RemoveEdgeL(LGraph* lg, int n1, int n2) {
	ENode* p, * q;
	if (ExistLGraph(lg, n1, n2) == 0)
	{
		printf("Error_RemoveEdgeL:删除失败\n");
		return 0;
	}
	p = lg->a[n1];
	q = NULL;
	while (p != NULL && p->adjvex != n2)
	{
		q = p;
		p = p->nextarc;
	}
	if (p == NULL)
	{
		printf("边不存在，删除失败\n");
		return 0;
	}
	if (q != NULL)//p不是第一个元素
		q->nextarc = p->nextarc;
	else //p是第一个元素
		lg->a[n1] = p->nextarc;
	free(p);
	lg->e--;
	return 0;
}
//以v为起点，执行图lg中的深度优先遍历的一部分（此函数被DFSLGraph调用）
void LDFS(int v, int visited[], LGraph* lg) {
	ENode* w;
	printf_s("%d", v);
	visited[v] = 1;
	for (w = lg->a[v];w != NULL;w = w->nextarc)
	{
		if (visited[w->adjvex] == 0)
			LDFS(w->adjvex, visited, lg);
	}
}
//图lg中的深度优先遍历
void DFSLGraph(LGraph* lg) {
	int i;
	int* visited = (int*)malloc(sizeof(int) * lg->n);
	memset(visited, 0, sizeof(int) * lg->n);
	printf("深度优先遍历\n");
	for (i = 0;i < lg->n;i++)
	{
		if (visited[i] == 0)
			LDFS(i, visited, lg);
	}
	printf("\n");
	free(visited);
}
//图lg的宽度优先遍历
void LBFS(int v, int visited[], LGraph* lg) {
	ENode* w;
	deque<int> q;
	visited[v] = 1;
	printf_s("%d", v);
	q.push_back(v);
	while (q.empty() == 0)
	{
		v = q.front();
		q.pop_front();
		for (w = lg->a[v];w != NULL;w = w->nextarc)
		{
			if (visited[w->adjvex] == NULL)
			{
				visited[w->adjvex] = 1;
				printf_s("%d", w->adjvex);
				q.push_back(w->adjvex);
			}
		}

	}
}
////以v为起点，执行图lg中的宽度优先遍历的一部分（此函数被BFSLGraph调用）
void BFSLGraph(LGraph* lg) {
	int i = 0;
	int* visited = (int*)malloc(sizeof(int) * lg->n);
	memset(visited, 0, sizeof(int) * lg->n);
	printf("宽度优先遍历\n");
	for (i = 0;i < lg->n;i++)
	{
		if (visited[i] == 0)
			LBFS(i, visited, lg);
	}
	printf("\n");
	free(visited);
}
//查找未访问过的最短路径边（d数组记录<v,，标号>的最短路径长度，
//s数组确定<v,标号>边最短路径是否确定，n表示图中节点数）该函数被Dijkstra调用
int choose(int* d, int* s, int n) {
	int i, minpos=-1;
	Elemtype min=INFTY;
	for (i = 1;i < n;i++)
	{
		if (d[i] < min && s[i] == 0)
		{
			min = d[i];
			minpos = i;
		}
	}
	return minpos;
}
//迪杰斯特拉算法找在图lg中从v到各个路径的最短长度，
//d和path要求为长度为图的节点数，该函数被dj调用
bool Dijkstra(int v,Elemtype* d ,int* path,LGraph* lg) {
	int i,k,w;
	ENode* temp;
	if (v<0 || v>lg->n - 1)
	{
		printf("Error_Dijkstra:越界错误 v");
		return 0;
	}
	int* s = (int*)malloc(sizeof(int) * lg->n);
	memset(s, 0, sizeof(int) * lg->n);
	for (i = 0;i < lg->n;i++)
		d[i] = INFTY;
	temp = lg->a[v];
	for (i=0;i<lg->n;i++)//初始化数组d
	{
		if (temp!=NULL&&temp->adjvex==i)
		{
			d[i] = temp->w;
			temp = temp->nextarc;
			if (v != i && d[i] < INFTY)
				path[i] = v;
			else
				path[i] = -1;
		}
	}
	s[v] = 1;//<v,v>路径已经确定
	d[v] = 0;//<v,v>路径长为0
	for (i = 1;i < lg->n - 1;i++)
	{
		k = choose(d, s, lg->n);
		s[k] = 1;
		if (k == -1)
			break;
		printf_s("%d ", k);

		temp = lg->a[k];
		while ( temp != NULL)
		{
			if (s[temp->adjvex]==0 && temp->w + d[k] < d[temp->adjvex])//v到？的最短长度还未确定且<v,k>的最短长度+w<k,?> < <v,?>的最短长度时
			{
				d[temp->adjvex] = temp->w + d[k];//更新<v,?>的最短长度
				path[temp->adjvex] = k;//添加到？的路径
			}
			temp = temp->nextarc;
		}
	}
	printf_s("是有实际意义的终点\n");
	int t, min = INFTY,minpos=-1;
	stack<int>st;
	for (i = 0;i < lg->n;i++)//计算<v,i>的所有路径长度
	{
		if (d[i] != INFTY)//<v,i>有边
		{
			printf_s("%d到%d的距离为%d  路径为: ", v, i, d[i]);
			if (min > d[i] && d[i]!=0)//找到最短的那条边，边权值存为min，<v,i>节点i存于minpos
			{
				min = d[i];
				minpos = i;
			}
			t = i;
			st.push(t);
			while (t!=v)//把<v,i>中的路径存在堆栈st中
			{
				st.push(path[t]);
				t = path[t];
			}
			printf("%d ", st.top());//打印路径
			st.pop();
			while (st.empty() == 0)
			{
				printf_s("-> ");
				printf("%d ", st.top());
				st.pop();
			}
			printf("\n");
		}
		else
			printf_s("%d到%d不连通\n", v, i);
	}
	printf("最短路径为%d到%d,长度为%d", v, minpos, min);
	return 1;

}
//获取迪杰斯特拉算法的一些初始化条件
void dj(LGraph* lg) {
	Elemtype *d = (Elemtype*)malloc(sizeof(Elemtype) * lg->n);
	int *path=(int*)malloc(sizeof(int) * lg->n);
	int x;
	printf_s("输入路径计算的源点：\n");
	scanf_s("%d", &x);
	Dijkstra(x, d, path, lg);
}
int main()
{
	int size = 0, n1, n2, w;
	LGraph lg;
	printf("输入节点数：");
	scanf_s("%d", &size);
	InitLGraph(&lg, size);
	printf_s("\n输入节点节点n1之间的边：(格式为：节点1 节点2 权值，每输完一组按回车，全部输入完毕后按回车＋CTRL Z)\n");
	while (cin >> n1 >> n2 >> w)
	{
		InsertLGraph(&lg, n1, n2, w);
	}
	DFSLGraph(&lg);
	BFSLGraph(&lg);
	dj(&lg);
	
	DestroyLGraph(&lg);
	return 1;
	return 1;
}
#include<iostream>
#include<stdio.h>
#include<deque>
#include<stack>
#define INFTY 2147483647
using namespace std;
typedef int Elemtype;
typedef struct eNode {
	int adjvex;//��ż�еĺ�һ��Ԫ��
	Elemtype w;//�ߵ�Ȩֵ
	struct eNode* nextarc;//ָ����һ���߽��
}ENode;
typedef struct lGraph {
	int n;//ͼ�Ľڵ���
	int e;//ͼ�ı���
	ENode** a;//ָ����ż�еĺ�һ��Ԫ�ؽڵ��ָ������
}LGraph;
//��ʼ���ڽӱ�
bool InitLGraph(LGraph* lg, int size)
{
	int i;
	if (size == 0)
		return 0;
	lg->a = (ENode**)malloc(size * sizeof(ENode*));
	if (lg->a == NULL)
	{
		printf("Error_InitLGraph:����ռ�ʧ��\n");
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
//�ͷ��ڽӱ�ռ�
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
//���ұ��Ƿ����
bool ExistLGraph(LGraph* lg, int n1, int n2) {
	ENode* p;
	if (n1<0 || n2<0 || n1>lg->n - 1 || n2>lg->n - 1 || n1 == n2)//Խ����Ի�·
	{
		printf("Error_ExistLGraph:��������\n");
		return 0;
	}
	else
	{
		p = lg->a[n1];//��λ��n1��
		while (p != NULL&&p->adjvex!=n2)//��<n1,n2>���ڵ���ż��
			p = p->nextarc;
		if (p == NULL)//�߲�����
		{
			return 1;
		}
		else//���Ѵ���
		{
			printf_s("��<%d,%d>�Ѿ�����",n1,n2);
			return 0;
		}
	}
}
//��ͼ�ϲ���<n1,n2>Ϊ��ż��ȨֵΪw�ı�
bool InsertLGraph(LGraph* lg, int n1, int n2, Elemtype w) {
	ENode* p;
	if (ExistLGraph(lg,n1,n2)==0)
	{
		printf("Error_InsertGraph:����ʧ��\n");
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
//��ͼ��ɾ��һ����
bool RemoveEdgeL(LGraph* lg, int n1, int n2) {
	ENode* p, * q;
	if (ExistLGraph(lg, n1, n2) == 0)
	{
		printf("Error_RemoveEdgeL:ɾ��ʧ��\n");
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
		printf("�߲����ڣ�ɾ��ʧ��\n");
		return 0;
	}
	if (q != NULL)//p���ǵ�һ��Ԫ��
		q->nextarc = p->nextarc;
	else //p�ǵ�һ��Ԫ��
		lg->a[n1] = p->nextarc;
	free(p);
	lg->e--;
	return 0;
}
//��vΪ��㣬ִ��ͼlg�е�������ȱ�����һ���֣��˺�����DFSLGraph���ã�
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
//ͼlg�е�������ȱ���
void DFSLGraph(LGraph* lg) {
	int i;
	int* visited = (int*)malloc(sizeof(int) * lg->n);
	memset(visited, 0, sizeof(int) * lg->n);
	printf("������ȱ���\n");
	for (i = 0;i < lg->n;i++)
	{
		if (visited[i] == 0)
			LDFS(i, visited, lg);
	}
	printf("\n");
	free(visited);
}
//ͼlg�Ŀ�����ȱ���
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
////��vΪ��㣬ִ��ͼlg�еĿ�����ȱ�����һ���֣��˺�����BFSLGraph���ã�
void BFSLGraph(LGraph* lg) {
	int i = 0;
	int* visited = (int*)malloc(sizeof(int) * lg->n);
	memset(visited, 0, sizeof(int) * lg->n);
	printf("������ȱ���\n");
	for (i = 0;i < lg->n;i++)
	{
		if (visited[i] == 0)
			LBFS(i, visited, lg);
	}
	printf("\n");
	free(visited);
}
//����δ���ʹ������·���ߣ�d�����¼<v,�����>�����·�����ȣ�
//s����ȷ��<v,���>�����·���Ƿ�ȷ����n��ʾͼ�нڵ������ú�����Dijkstra����
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
//�Ͻ�˹�����㷨����ͼlg�д�v������·������̳��ȣ�
//d��pathҪ��Ϊ����Ϊͼ�Ľڵ������ú�����dj����
bool Dijkstra(int v,Elemtype* d ,int* path,LGraph* lg) {
	int i,k,w;
	ENode* temp;
	if (v<0 || v>lg->n - 1)
	{
		printf("Error_Dijkstra:Խ����� v");
		return 0;
	}
	int* s = (int*)malloc(sizeof(int) * lg->n);
	memset(s, 0, sizeof(int) * lg->n);
	for (i = 0;i < lg->n;i++)
		d[i] = INFTY;
	temp = lg->a[v];
	for (i=0;i<lg->n;i++)//��ʼ������d
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
	s[v] = 1;//<v,v>·���Ѿ�ȷ��
	d[v] = 0;//<v,v>·����Ϊ0
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
			if (s[temp->adjvex]==0 && temp->w + d[k] < d[temp->adjvex])//v��������̳��Ȼ�δȷ����<v,k>����̳���+w<k,?> < <v,?>����̳���ʱ
			{
				d[temp->adjvex] = temp->w + d[k];//����<v,?>����̳���
				path[temp->adjvex] = k;//��ӵ�����·��
			}
			temp = temp->nextarc;
		}
	}
	printf_s("����ʵ��������յ�\n");
	int t, min = INFTY,minpos=-1;
	stack<int>st;
	for (i = 0;i < lg->n;i++)//����<v,i>������·������
	{
		if (d[i] != INFTY)//<v,i>�б�
		{
			printf_s("%d��%d�ľ���Ϊ%d  ·��Ϊ: ", v, i, d[i]);
			if (min > d[i] && d[i]!=0)//�ҵ���̵������ߣ���Ȩֵ��Ϊmin��<v,i>�ڵ�i����minpos
			{
				min = d[i];
				minpos = i;
			}
			t = i;
			st.push(t);
			while (t!=v)//��<v,i>�е�·�����ڶ�ջst��
			{
				st.push(path[t]);
				t = path[t];
			}
			printf("%d ", st.top());//��ӡ·��
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
			printf_s("%d��%d����ͨ\n", v, i);
	}
	printf("���·��Ϊ%d��%d,����Ϊ%d", v, minpos, min);
	return 1;

}
//��ȡ�Ͻ�˹�����㷨��һЩ��ʼ������
void dj(LGraph* lg) {
	Elemtype *d = (Elemtype*)malloc(sizeof(Elemtype) * lg->n);
	int *path=(int*)malloc(sizeof(int) * lg->n);
	int x;
	printf_s("����·�������Դ�㣺\n");
	scanf_s("%d", &x);
	Dijkstra(x, d, path, lg);
}
int main()
{
	int size = 0, n1, n2, w;
	LGraph lg;
	printf("����ڵ�����");
	scanf_s("%d", &size);
	InitLGraph(&lg, size);
	printf_s("\n����ڵ�ڵ�n1֮��ıߣ�(��ʽΪ���ڵ�1 �ڵ�2 Ȩֵ��ÿ����һ�鰴�س���ȫ��������Ϻ󰴻س���CTRL Z)\n");
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
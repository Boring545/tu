#include<iostream>
#include<stdio.h>
#include<deque>
using namespace std;
typedef int Elemtype;
//ͼ���ڽӾ���
typedef struct mGraph {
	Elemtype** a;
	int n;//ͼ�ڵ���
	int e;//ͼ����
	Elemtype noEdge;
}MGraph;
//��ʼ��ͼ
bool InitMGraph(MGraph* mg, int size, Elemtype noEdgeValue) {
	int i, j;
	if (size == 0)
		return 0;
	mg->a = (Elemtype**)malloc(size*sizeof(Elemtype*));//������һ���ڽӾ���
	if (mg->a == NULL)
	{
		printf("Error_InitGraph:�����ڴ�ʧ��\n");
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
			printf("Error_InitGraph:�����ڴ�ʧ��,λ��%d\n",i);
			return 0;
		}
		for (j = 0;j < size;j++)
			mg->a[i][j] = noEdgeValue;
		mg->a[i][i] = 0;
	}
	return 1;
}
//�ͷ�ͼ�ռ�xx
void DestroyMGraph(MGraph* mg) {
	int i;
	for (i = 0;i < mg->n;i++)
		free(mg->a[i]);
	free(mg->a);
}
//�жϱ��Ƿ����
bool ExistMGraph(MGraph* mg, int n1, int n2) {
	if (n1<0 || n2<0 || n1>mg->n - 1 || n2>mg->n - 1 ||n1==n2)
	{
		printf("�߲�����\n");
		return 0;
	}
	else if (mg->a[n1][n2] == mg->noEdge)
	{
		//printf("������Ч\n");
		return 1;
	}
	else
		return 1;
}
//��ͼ�ϲ���<n1,n2>Ϊ��ż��ȨֵΪw�ı�(ȨֵĬ��Ϊ1)������1��
bool InsertMGraph(MGraph* mg, int n1, int n2, Elemtype w=1) {
	if (ExistMGraph(mg,n1,n2)==0)
	{
		if (mg->a[n1][n2] != mg->noEdge)
			printf("���Ѵ���\n");
		printf("Error_InsertGraph:����ʧ��\n");
		return 0;
	}
	else
	{
		mg->a[n1][n2] = w;
		mg->e++;
		return 1;
	}
}
//��ͼ��ɾ����<n1, n2>Ϊ��ż�ı�
bool RemoveEdgeM(MGraph* mg, int n1, int n2)
{
	if (n1<0 || n2<0 || n1>mg->n - 1 || n2>mg->n - 1 || n1 == n2||mg->a[n1][n2]==mg->noEdge)
	{
		printf("Error_InsertGraph:ɾ��ʧ��\n");
		return 0;
	}
	else
	{
		mg->a[n1][n2] = mg->noEdge;
		mg->e--;
		return 1;
	}
}
//ͼ��������ȱ���
void MDFS(int v, int visited[], MGraph* mg) {//ִ��һ�λ��һ������
	int x,temp;
	printf("%d ", v);//��ӡ��һ���ڵ�v
	while(visited[v] < mg->n)
	{       //��λ
			while (visited[v] < mg->n && (mg->a[v][visited[v]] == mg->noEdge || mg->a[v][visited[v]] == 0 || visited[visited[v]] != 0))//��visitedû��Խ���ǰ����,��û�б߻�u����v��visited[v]���Ѿ������ʵ������visited�����λ��ֱ���ҵ������visitedֵ
			{
				visited[v]++;
			}
			if(visited[v] < mg->n)//����ǰ���ж�Ӧ��
			{
				printf("(%d) ", mg->a[v][visited[v]]);//��ӡ��Ȩֵ
				printf("%d ", visited[v]);//��ӡ��һ���ڵ�
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
	memset(visited, 0, sizeof(int)*mg->n);//�������帳ֵ0
	printf("ͼ��������ȱ���\n");
	for (i = 0;i < mg->n;i++)
	{
		if (visited[i]==0)//visited[i] ==0��ʾ����δ������
			MDFS(i, visited, mg);
	}
	free(visited);
}
//ͼ�Ŀ�����ȱ���
void MBFS(int v, int visited[], MGraph* mg) {
	deque<int> q;
	int i;
	q.push_back(v);
	visited[v] = 0;//��ֵΪ0���������Ѿ������ʹ�������Ϊִ������ӵĲ�����-1����δ���ʹ�
	visited[visited[v]] = 0;
	while (q.empty()==0)
	{
		v=q.front();//�õ�����ͷԪ��
		q.pop_front();//ɾ�����Ԫ��
		printf("%d ", v);
		while (visited[v] < mg->n)
		{
			if (mg->a[v][visited[v]] != 0 && mg->a[v][visited[v]] != mg->noEdge && visited[visited[v]]==-1)//�ҵ���ǰ�ڵ�v��û�з��ʹ����ڽڵ�
			{
				visited[visited[v]] = 0;
				q.push_back(visited[v]);//��visited[v]��ӦԪ�ز������β
			}
			visited[v]++;
		}
	}
	printf("\n");
}
void BFSMGraph(MGraph* mg) {
	int i;
	int* visited = (int*)malloc(mg->n * sizeof(int));
	memset(visited, -1, sizeof(int) * mg->n);//�������帳ֵ-1
	printf("ͼ�Ŀ�����ȱ���\n");
	for (i = 0;i < mg->n;i++)
	{
		if (visited[i] ==-1)//visited[i]==-1��ʾ����δ������
			MBFS(i, visited, mg);
	}
	free(visited);
}
int main()
{
	int size=0,n1,n2,w;
	MGraph mg;
	printf("����ڵ�����");
	scanf_s("%d", &size);
	InitMGraph(&mg,size,2147483647);
	printf_s("\n����ڵ�ڵ�n1֮��ıߣ�(��ʽΪ���ڵ�1 �ڵ�2 Ȩֵ)\n");
	while(cin >> n1 >> n2 >> w)
	{
		InsertMGraph(&mg, n1, n2, w);
	}
	DFSMGraph(&mg);
	BFSMGraph(&mg);
	DestroyMGraph(&mg);
	return 1;
}
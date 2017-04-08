/* ����ģ���˻��㷨���tsp���� */
#include<iostream>  
#include<ctime>  
#include<cstdio>  
#include<cstdlib>  
#include<cmath>

#define MAX 10000  
#define INF 10000000   
#define E 0.000000001  // �������   
#define L 20000        // ��������   
#define AT 0.999       // ����ϵ��   
#define T 1            // ��ʼ�¶�   
using namespace std;

struct element{     //������������ݽṹ   
	int data;       // ����   
	int index;      // ���   
};
int tsp(int d[][MAX], int n, double e, int l, double at, double t, int s0[]);  //����ģ���˻��㷨������·��   
int cmp(const void *a, const void *b); //��������   
void rand_of_n(int a[], int n);        //���� 1-n ��������в��浽 a[] ��  
int random(int m, int n);
int dis[MAX][MAX];                     // d[i][j] ��ʾ�ͻ�i���ͻ�j�ľ��룬0 ��ʾ��ʼ��   


int main()
{
	int a = 5;
	++(a++);
/*******���м�·����Ϣ׼������*******/
	int i, j;
	int n = 100;                 // ��ĸ���   
	for (i = 0; i <= n; i++)     // ����������� 1-100  
		for (j = i; j <= n; j++)
			if (i == j)
				dis[i][j] = 0;
			else
				dis[i][j] = dis[j][i] = random(1, 100);

	FILE*fp = fopen("data.txt", "w");  // ����������ļ���   
	for (i = 0; i <= n; i++)
	{
		for (j = 0; j <= n; j++)
			fprintf(fp, "%d ", dis[i][j]);
		fprintf(fp, "\n");
	}
	fclose(fp);
/*******ģ���˻𷨿�ʼ�������������*******/
	int sum, sum0, s0[MAX];
	sum0 = 0;                       //˳�����ʱ��·��   
	for (i = 0; i<n; i++)
		sum0 = sum0 + dis[i][i + 1];
	sum0 = sum0 + dis[n][0];        //�ټ��ϴ�n�ٷ��ص�0���еľ���
	sum = tsp(dis, n, E, L, AT, T, s0);
/*******�������Ľ��*******/
	fp = fopen("answer.txt", "w");  //����������ļ���  
	for (i = 1; i <= n; i++)
		fprintf(fp, "%d ", s0[i]);
	fclose(fp);
	cout << "case" << "#: " << endl;
	cout << "�������鱣�����ļ�data.txt�У��Ż���ķ���˳�򱣴����ļ�answer.txt��" << endl;
	cout << "˳�������·��Ϊ��" << sum0 << " �Ż���Ϊ��" << sum << endl;
	cout << endl;

	return 0;
}

int cmp(const void *a, const void *b){   // ��������  
	return((struct element*)a)->data - ((struct element*)b)->data;
}

/*
@�������n�����ӣ�Ȼ���n����������
a�д洢������������ӣ��Ӵ�С��������ֵ
*/
void rand_of_n(int a[], int n){
	int i;
	struct element ele[MAX];
	srand((int)time(0));        // ��ʼ�����������   
	for (i = 0; i<n; i++){
		ele[i].data = rand();   // �������һ����   
		ele[i].index = i + 1;
	}
	qsort(ele, n, sizeof(ele[0]), cmp);  //����   
	for (i = 0; i<n; i++){
		a[i] = ele[i].index;
	}
}

int random(int m, int n){   //����m-n�������  
	int a;
	double x = (double)rand() / RAND_MAX;
	a = (int)(x*(n - m) + 0.5) + m;
	return a;
}

int tsp(int d[][MAX], int n, double e, int l, double at, double t, int s0[])
{
	int i, j, s[MAX], sum, temp;
	sum = INF;
	/****ѡ��1000�Σ�ѡȡ��һ��·���ܺ���С��****/
	for (i = 0; i<1000; i++){    //�������ؿ��巽��������ʼ��  
		rand_of_n(&s[1], n);
		s[0] = 0; s[n + 1] = 0;  //��һ�������һ����Ϊ��ʼ��   
		temp = 0;
		for (j = 0; j <= n; j++)
			temp = temp + d[s[j]][s[j + 1]];
		if (temp<sum){
			for (j = 0; j <= n + 1; j++)
				s0[j] = s[j];
			sum = temp;
		}
	}
	/****�˻���̣��������������������У������µ�·����****/
	for (i = 0; i<l; i++)
	{       
		//ѡȡc1��c2������һ���ĳ��б�ţ�ǰ��������
		int c1, c2;
		c1 = random(1, n);
		c2 = random(1, n);
		if (c1>c2)
		{
			int temp = c2; c2 = c1; c1 = temp;
		}
		if (c1 == c2)
			continue;
		//������ۺ��� (�ȵ�c2�������ٵ�c2���ü۸�)
		int df = d[s0[c1 - 1]][s0[c2]] + d[s0[c1]][s0[c2 + 1]] - d[s0[c1 - 1]][s0[c1]] - d[s0[c2]][s0[c2 + 1]];  
		if (df<0){            //����׼��  �Ը���1�����µ�·����������exp(-df / t)���ʽ����µ�·��
			while (c1<c2){
				int temp = s0[c2]; s0[c2] = s0[c1]; s0[c1] = temp;
				c1++;
				c2--;
			}
			sum = sum + df;
		}
		else if (exp(-df / t)>((double)rand() / RAND_MAX)){  //((double)rand() / RAND_MAX)������һ��������ʽ��ܽ��
			while (c1<c2){
				int temp = s0[c2]; s0[c2] = s0[c1]; s0[c1] = temp;
				c1++;
				c2--;
			}

			sum = sum + df;
		}
		t = t*at;   //��ʼ������at���н���
		if (t<e)    //��tС�ڽ����¶���ֹͣ���������ǰ��״̬
			break;
	}
	return sum;
}
/* 利用模拟退火算法求解tsp问题 */
#include<iostream>  
#include<ctime>  
#include<cstdio>  
#include<cstdlib>  
#include<cmath>

#define MAX 10000  
#define INF 10000000   
#define E 0.000000001  // 迭代误差   
#define L 20000        // 迭代次数   
#define AT 0.999       // 降温系数   
#define T 1            // 初始温度   
using namespace std;

struct element{     //用来排序的数据结构   
	int data;       // 数据   
	int index;      // 序号   
};
int tsp(int d[][MAX], int n, double e, int l, double at, double t, int s0[]);  //利用模拟退火算法求解最短路径   
int cmp(const void *a, const void *b); //升序排列   
void rand_of_n(int a[], int n);        //产生 1-n 的随机排列并存到 a[] 中  
int random(int m, int n);
int dis[MAX][MAX];                     // d[i][j] 表示客户i到客户j的距离，0 表示起始点   


int main()
{
	int a = 5;
	++(a++);
/*******城市间路径信息准备工作*******/
	int i, j;
	int n = 100;                 // 点的个数   
	for (i = 0; i <= n; i++)     // 随机产生距离 1-100  
		for (j = i; j <= n; j++)
			if (i == j)
				dis[i][j] = 0;
			else
				dis[i][j] = dis[j][i] = random(1, 100);

	FILE*fp = fopen("data.txt", "w");  // 将距离存入文件中   
	for (i = 0; i <= n; i++)
	{
		for (j = 0; j <= n; j++)
			fprintf(fp, "%d ", dis[i][j]);
		fprintf(fp, "\n");
	}
	fclose(fp);
/*******模拟退火法开始求解旅行商问题*******/
	int sum, sum0, s0[MAX];
	sum0 = 0;                       //顺序遍历时的路程   
	for (i = 0; i<n; i++)
		sum0 = sum0 + dis[i][i + 1];
	sum0 = sum0 + dis[n][0];        //再加上从n再返回到0城市的距离
	sum = tsp(dis, n, E, L, AT, T, s0);
/*******保存最后的结果*******/
	fp = fopen("answer.txt", "w");  //将结果存入文件中  
	for (i = 1; i <= n; i++)
		fprintf(fp, "%d ", s0[i]);
	fclose(fp);
	cout << "case" << "#: " << endl;
	cout << "距离数组保存在文件data.txt中，优化后的访问顺序保存在文件answer.txt中" << endl;
	cout << "顺序遍历总路程为：" << sum0 << " 优化后为：" << sum << endl;
	cout << endl;

	return 0;
}

int cmp(const void *a, const void *b){   // 升序排序  
	return((struct element*)a)->data - ((struct element*)b)->data;
}

/*
@随机产生n个种子，然后对n个种子排序，
a中存储的是排序后种子（从大到小）的索引值
*/
void rand_of_n(int a[], int n){
	int i;
	struct element ele[MAX];
	srand((int)time(0));        // 初始化随机数种子   
	for (i = 0; i<n; i++){
		ele[i].data = rand();   // 随机生成一个数   
		ele[i].index = i + 1;
	}
	qsort(ele, n, sizeof(ele[0]), cmp);  //排序   
	for (i = 0; i<n; i++){
		a[i] = ele[i].index;
	}
}

int random(int m, int n){   //产生m-n的随机数  
	int a;
	double x = (double)rand() / RAND_MAX;
	a = (int)(x*(n - m) + 0.5) + m;
	return a;
}

int tsp(int d[][MAX], int n, double e, int l, double at, double t, int s0[])
{
	int i, j, s[MAX], sum, temp;
	sum = INF;
	/****选择1000次，选取出一个路径总和最小的****/
	for (i = 0; i<1000; i++){    //利用蒙特卡洛方法产生初始解  
		rand_of_n(&s[1], n);
		s[0] = 0; s[n + 1] = 0;  //第一个和最后一个点为起始点   
		temp = 0;
		for (j = 0; j <= n; j++)
			temp = temp + d[s[j]][s[j + 1]];
		if (temp<sum){
			for (j = 0; j <= n + 1; j++)
				s0[j] = s[j];
			sum = temp;
		}
	}
	/****退火过程：交换其中任意两个城市，产生新的路径解****/
	for (i = 0; i<l; i++)
	{       
		//选取c1和c2两个不一样的城市编号（前者索引大）
		int c1, c2;
		c1 = random(1, n);
		c2 = random(1, n);
		if (c1>c2)
		{
			int temp = c2; c2 = c1; c1 = temp;
		}
		if (c1 == c2)
			continue;
		//计算代价函数 (先到c2后，依次再到c2费用价格)
		int df = d[s0[c1 - 1]][s0[c2]] + d[s0[c1]][s0[c2 + 1]] - d[s0[c1 - 1]][s0[c1]] - d[s0[c2]][s0[c2 + 1]];  
		if (df<0){            //接受准则  以概率1接受新的路径，否则以exp(-df / t)概率接受新的路径
			while (c1<c2){
				int temp = s0[c2]; s0[c2] = s0[c1]; s0[c1] = temp;
				c1++;
				c2--;
			}
			sum = sum + df;
		}
		else if (exp(-df / t)>((double)rand() / RAND_MAX)){  //((double)rand() / RAND_MAX)产生的一个随机概率接受结果
			while (c1<c2){
				int temp = s0[c2]; s0[c2] = s0[c1]; s0[c1] = temp;
				c1++;
				c2--;
			}

			sum = sum + df;
		}
		t = t*at;   //开始以速率at进行降温
		if (t<e)    //若t小于结束温度则停止迭代输出当前的状态
			break;
	}
	return sum;
}
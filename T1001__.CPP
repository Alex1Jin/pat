#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <cstring>
#include <map>
#include <vector>
#include <set>
#include <algorithm>
using namespace std;
 
struct road {
	int city1, city2, weight, status;
};
 
const int MAX = 10000000;
const int M = 500 * 499;
int cmp(const void *r1, const void *r2);
int findroot(int cityset[], int index);
int unionset(int cityset[], int index1, int index2);
 
int main(void) {
	int n, m, i, j, usecnt = 0, destcnt = 0;
	road tmp;
	road *use, *destroyed;
	use = new road[M];
	destroyed = new road[M];
    // 加快输入
	setvbuf(stdin, new char[1 << 20], _IOFBF, 1 << 20);
	setvbuf(stdout, new char[1 << 20], _IOFBF, 1 << 20);
	scanf("%d %d", &n, &m);
	for (i = 0; i < m; i++) {
		scanf("%d %d %d %d", &tmp.city1, &tmp.city2, &tmp.weight, &tmp.status);
		if (tmp.status) {
			use[usecnt++] = tmp;
		}
		else {
			destroyed[destcnt++] = tmp;
		}
	}
	qsort(destroyed, destcnt, sizeof(road), cmp); // 将被摧毁的道路按照修复费用从小到大排序
	int *cityset = new int[n + 1];
	set<int> pro;
	int max_cost = 0, cnt, cost;
	for (i = 1; i < n + 1; i++) { // 遍历所有的城市，假定该城市已被敌人占领
		for (j = 0; j < n + 1; j++) {
			cityset[j] = -1;
		}
		cnt = cost = 0; // cnt的绝对值表示最大的连通图结点个数，cost表示最小的修复费用
		for (j = 0; j < usecnt && cnt != -n + 1; j++) {  // 遍历所有正常道路，寻找最大的连通图
			if (use[j].city1 == i || use[j].city2 == i) {
				continue; // 如果道路两端的某城市已被占领，则跳过
			}
			int index = unionset(cityset, use[j].city1, use[j].city2);
			if (index > 0 && cityset[index] < cnt) {
				cnt = cityset[index];
			}
		}
		for (j = 0; j < destcnt && cnt != -n + 1; j++) { // 遍历已被摧毁的道路
			if (destroyed[j].city1 == i || destroyed[j].city2 == i) {
				continue; // 道路两端有城市被占领，则放弃修复
			}
			int index = unionset(cityset, destroyed[j].city1, destroyed[j].city2);
			if (index > 0) { // 如果道路两端的城市属于不同的连通图
				cnt = cityset[index] < cnt ? cityset[index] : cnt;
				cost += destroyed[j].weight;
			}
		}
		if (cnt == -n + 1 && cost && cost == max_cost) { // 如果联通了n-1个城市，且修复费用和目前大值齐平
			pro.insert(i);
		}
		else if (cnt == -n + 1 && cost && cost > max_cost) { // 如果费用高于目前最大值
			pro.clear();
			pro.insert(i);
			max_cost = cost;
		}
		else if (cnt > -n + 1 && max_cost == MAX) { // 如果不能联通所有未被占领城市，且pro集合中没有代价更小的城市
			pro.insert(i);
		}
		else if (cnt > -n + 1 && max_cost != MAX) { // 如果不能联通所有未被占领城市，且pro集合中有代价更小的城市
			pro.clear();
			pro.insert(i);
			max_cost = MAX;
		}
	}
	if (pro.size()) { // 输出结果
		set<int>::iterator it = pro.begin();
		printf("%d", *it++);
		for (; it != pro.end(); it++) {
			printf(" %d", *it);
		}
	}
	else {
		puts("0");
	}
	delete[] cityset;
	delete[] use;
	delete[] destroyed;
	return 0;
}
 
int cmp(const void *v1, const void *v2) {
	road *r1, *r2;
	r1 = (road *)v1;
	r2 = (road *)v2;
	return r1->weight - r2->weight;
}
int findroot(int cityset[], int index) { // 寻找根的过程中，把所有的结点直接连接到根上
	if (cityset[index] < 0) {
		return index;
	}
	else {
		return cityset[index] = findroot(cityset, cityset[index]);
	}
}
int unionset(int cityset[], int index1, int index2) {
	int root1, root2;
	root1 = findroot(cityset, index1);
	root2 = findroot(cityset, index2);
	if (root1 == root2) {
		return -1;
	}
	else { // cityset[root]的值是连通图结点数的相反数
		if (cityset[root1] > cityset[root2]) {
			cityset[root2] += cityset[root1];
			cityset[root1] = root2;
			return root2;
		}
		else {
			cityset[root1] += cityset[root2];
			cityset[root2] = root1;
			return root1;
		}
	}
}
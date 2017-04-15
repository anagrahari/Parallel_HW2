#include<stdio.h>
#include<stdlib.h>
#include<algorithm>
#include<functional>
#include<iostream>
#include<time.h>
#include<cilk/cilk.h>
#include<vector>
int n;
int m;
using namespace std;
typedef struct{
	int u;
	int v;

} E;

vector<int> par_prefix_sum(vector<int> &x, int n){
	vector<int> s(n+1);
	if (n == 1) {
		s[1] = x[1];
	}
	else {
		vector<int> y(n/2+1);
		cilk_for (int i = 1; i <= n/2; i++) {
			y[i] = x[2*i-1]+x[2*i];
		}
		vector<int> z(n/2+1);
		z = par_prefix_sum(y, n/2);
		cilk_for (int i = 1; i <= n; i++ ) {
			if (i == 1) s[1] = x[1];
			else if (i % 2 == 0) s[i] = z[i/2];
			else s[i] = z[(i-1)/2] + x[i];
		}
	}
	return s;
}


vector<int> par_randomized_cc(int n, vector<E> &e, vector<int> &L, int m){
	if (m == 0) return L;
	vector<int> C(n+1);
	vector<int> M(n+1);
	vector<int> S(m+1);

	cilk_for (int v = 1; v <= n; v++) {
		C[v] = rand() % 2;
	}
	cilk_for(int i = 1; i <= m; i++) {
		if(C[e[i].u] == 1 && C[e[i].v] == 0) {
			L[e[i].u] = L[e[i].v];
		}
		else if (C[e[i].v] == 1 && C[e[i].u] == 0) {
			L[e[i].v] = L[e[i].u];
		}
	}
	cilk_for (int i = 1; i <= m; i++) {
		if (L[e[i].u] != L[e[i].v]) {
			S[i] = 1;
		} else {
			S[i] = 0;
		}
	}
	S = par_prefix_sum(S, m);
	vector<E> F(S[m]+1);
	cilk_for (int i =1; i <= m; i++){
		if (L[e[i].u] != L[e[i].v]){
			F[S[i]].u = L[e[i].u];
			F[S[i]].v = L[e[i].v];
		}
	}
	M = par_randomized_cc(n, F, L, S[m]);
	cilk_for (int i = 1; i <= m; i++) {
		if (e[i].v == L[e[i].u]){
			M[e[i].u] = M[e[i].v];
		}
		else if(e[i].u == L[e[i].v]) {
			M[e[i].v] = M[e[i].u];
		}
	}
	return M;
}

void print_output(vector<int> &L, int n) {
	int count = 0;
	vector<int> size(n+1);
	for (int i = 1; i <= n; i++){
		size[i]=0;
	}
	for (int i = 1; i <= n; i++){
		if(size[L[i]]==0) count++;
		size[L[i]]++;
	}
	std::sort(size.begin(), size.end(), std::greater<int>());
	cout << count << "\n";
	for (int i = 1; i <= n; i++){
		if(size[i]!=0) cout << size[i] << "\n";
	}
}


void print_input(vector<E> &e) {
	vector<int> L(n+1);
	cilk_for (int v = 1; v <= n; v++) {
		L[v] = v;
	}
	clock_t start, end;
	double cpu_time_used;
	start = clock();
	L = par_randomized_cc(n, e, L, m);
	end = clock();
	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	cout << "CPU time used  is :" << cpu_time_used << "\n";
	print_output(L, n);
}

int main() {
	cin >> n;
	cin >> m;
	vector<E> e(m+1);
	for (int i = 1; i <= m; i++) {
		cin >> e[i].u;
		cin >> e[i].v;
	}
	print_input(e);

}

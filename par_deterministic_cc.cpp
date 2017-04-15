#include<algorithm>
#include<iostream>
#include<sys/time.h>
#include<cilk/cilk.h>
#include "par_cc_common.h"

void find_roots(int n, vector<int> &P, vector<int> &S){
	cilk_for (int v = 1; v <= n; v++){
		S[v] = P[v];
	}
	bool flag = true;
	while (flag == true){
		flag = false;
		cilk_for (int v = 1; v <= n; v++){
			S[v] = S[S[v]];
			if (S[v] != S[S[v]]) flag = true;
		}
	}
	return;
}

void optimized_find_roots(int n, vector<int> &P, vector<int> &S){
	cilk_for(int v = 1; v <= n; v++){
		S[v] = P[v];
		while (S[v] != S[S[v]]){
			S[v] = S[S[v]];
		}
	}
	return;

}

int par_sum(vector<int> &x, int len){
	vector<int> s(n+1);
	if (n == 1) {
		s[1] = x[1];
	}
	else {
		vector<int> y(n/2 + 1);
		cilk_for (int i = 1; i <= n/2; i++) {
			y[i] = x[2*i-1]+x[2*i];
		}
		vector<int> z(n/2 + 1);
		z = par_prefix_sum(y, n/2);
		cilk_for (int i = 1; i <= n; i++ ) {
			if (i == 1) s[1] = x[1];
			else if (i % 2 == 0) s[i] = z[i/2];
			else s[i] = z[(i-1)/2] + x[i];
		}
	}
	return s[n];
}


vector<int> par_deterministic_cc(int n, vector<Edge> &e, vector<int> L, int m){
	if (m == 0) return L;
	vector<int> l2h(n+1);
	vector<int> h2l(n+1);
	vector<int> S(m+1);
	cilk_for (int v = 1; v <= n; v++){
		l2h[v] = 0;
		h2l[v] = 0;
	}
	cilk_for (int i = 1; i <= m; i++){
		if (E[i].u < E[i].v) l2h[E[i].u] = 1;
		else h2l[E[i].u] = 1;
	}
	int n1 = par_sum(l2h, n);
	int n2 = par_sum(h2l, n);
	cilk_for (int i = 1; i <= m; i++){
		if (n1 >= n2 && E[i].u < E[i].v) L[E[i].u] = E[i].v;
		else if (n1 >= n2 && E[i].v < E[i].u) L[E[i].v] = E[i].u;
		else if (n1 < n2 && E[i].u > E[i].v) L[E[i].u] = E[i].v;
		else if (n1 < n2 && E[i].v > E[i].u) L[E[i].v] = E[i].u;
	}
	find_roots(n,L,L);
	cilk_for (int i = 1; i <= m; i++){
		S[i] = (L[E[i].u] != L[E[i].v]) ? 1 : 0;
	}
	S = par_prefix_sum(S, m);
	vector<Edge> F(S[m]+1);
	cilk_for (int i =1; i <= m; i++){
		if (L[E[i].u] != L[E[i].v]){
			F[S[i]].u = L[E[i].u];
			F[S[i]].v = L[E[i].v];
		}
	}
	L = par_deterministic_cc(n, F, L, S[m]);
	return L;
}

int main() {
	struct timeval start,end;
	
	initialize();
	gettimeofday(&start,NULL); //Start timing the computation
	L = par_deterministic_cc(n, E, L, m);
	gettimeofday(&end,NULL); //Stop timing the computation
	double myTime = (end.tv_sec+(double)end.tv_usec/1000000) -
			 (start.tv_sec+(double)start.tv_usec/1000000);
	cout << "Time taken by algorithm: " << myTime << " seconds.\n";
	dump_output(L, n);
}


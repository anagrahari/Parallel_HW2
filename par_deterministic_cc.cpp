#include<algorithm>
#include<iostream>
#include<sys/time.h>
#include<cilk/cilk.h>
#include "par_cc_common.h"

int optimized_roots = 0;

void find_roots(int n, vector<int> &P, vector<int> &S) {
	bool flag = true;
 
	cilk_for (int v = 1; v <= n; v++) {
		S[v] = P[v];
	}

	while (flag == true) {
		flag = false;
		cilk_for (int v = 1; v <= n; v++) {
			S[v] = S[S[v]];
			if (S[v] != S[S[v]]) 
				flag = true;
		}
	}

	return;
}

void optimized_find_roots(int n, vector<int> &P, vector<int> &S) {
	cilk_for(int v = 1; v <= n; v++) {
		S[v] = P[v];
		while (S[v] != S[S[v]]) { 
			S[v] = S[S[v]];
		}
	}

	return;
}


vector<int> par_deterministic_cc(int n, vector<Edge> &e, vector<int> L, int m) {
	if (m == 0) 
		return L;
	vector<int> L2H(n+1);
	vector<int> H2L(n+1);
	vector<int> S(m+1);

	cilk_for (int v = 1; v <= n; v++) { 
		L2H[v] = 0;
		H2L[v] = 0;
	}

	cilk_for (int i = 1; i <= m; i++) {
		if (E[i].u < E[i].v) 
			L2H[E[i].u] = 1;
		else 
			H2L[E[i].u] = 1;
	}

	int n1 = par_sum(L2H, n);
	int n2 = par_sum(H2L, n);

	cilk_for (int i = 1; i <= m; i++) {
		if (n1 >= n2) {
			if (E[i].u < E[i].v)
				L[E[i].u] = E[i].v;
			else if (E[i].v < E[i].u)
				L[E[i].v] = E[i].u;
		} else {
			if (E[i].u > E[i].v)
				L[E[i].u] = E[i].v;
			else if (E[i].v > E[i].u)
				L[E[i].v] = E[i].u;
		}
	}
	
	if (optimized_roots == 0)
		find_roots(n,L,L);
	else
		optimized_find_roots(n,L,L);

	cilk_for (int i = 1; i <= m; i++) {
		S[i] = (L[E[i].u] != L[E[i].v]) ? 1 : 0;
	}

	S = par_prefix_sum(S);
	vector<Edge> F(S[m]+1);
	cilk_for (int i =1; i <= m; i++) {
		if (L[E[i].u] != L[E[i].v]) {
			F[S[i]].u = L[E[i].u];
			F[S[i]].v = L[E[i].v];
		}
	}

	L = par_deterministic_cc(n, F, L, S[m]);

	return L;
}

int main(int argc, char *args[]) {
	struct timeval start, end;

	optimized_roots = atoi(args[1]);

	if (optimized_roots == 1)
		cout << "Running with optimized_find_roots..";

	initialize();

	gettimeofday(&start, NULL); //Start timing of computation
	L = par_deterministic_cc(N, E, L, M);
	gettimeofday(&end, NULL); //Stop timing of computation
	double time = (end.tv_sec+(double)end.tv_usec/1000000) -
			 (start.tv_sec+(double)start.tv_usec/1000000);
	cout << "Time taken by algorithm: " << time << " seconds.\n";
	dump_output(L, N);
}


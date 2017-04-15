#include<algorithm>
#include<iostream>
#include<sys/time.h>
#include<cilk/cilk.h>
#include "par_cc_common.h"

vector<int> par_randomized_cc(int n, vector<Edge> &E, vector<int> &L, int m){
	if (m == 0) return L;
	vector<int> C(n+1);
	vector<int> M(n+1);
	vector<int> S(m+1);

	cilk_for (int v = 1; v <= n; v++) {
		C[v] = rand() % 2;
	}
	cilk_for(int i = 1; i <= m; i++) {
		if(C[E[i].u] == 1 && C[E[i].v] == 0) {
			L[E[i].u] = L[E[i].v];
		}
		else if (C[E[i].v] == 1 && C[E[i].u] == 0) {
			L[E[i].v] = L[E[i].u];
		}
	}
	cilk_for (int i = 1; i <= m; i++) {
		if (L[E[i].u] != L[E[i].v]) {
			S[i] = 1;
		} else {
			S[i] = 0;
		}
	}
	S = par_prefix_sum(S, m);
	vector<Edge> F(S[m]+1);
	cilk_for (int i =1; i <= m; i++){
		if (L[E[i].u] != L[E[i].v]){
			F[S[i]].u = L[E[i].u];
			F[S[i]].v = L[E[i].v];
		}
	}
	M = par_randomized_cc(n, F, L, S[m]);
	cilk_for (int i = 1; i <= m; i++) {
		if (E[i].v == L[E[i].u]){
			M[E[i].u] = M[E[i].v];
		}
		else if(E[i].u == L[E[i].v]) {
			M[E[i].v] = M[E[i].u];
		}
	}
	return M;
}

void dump_output(vector<int> &L, int n) {
	int count = 0;
	vector<int> output(n+1);
	for (int i = 1; i <= n; i++){
		output[i]=0;
	}
	for (int i = 1; i <= n; i++){
		if(output[L[i]]==0) count++;
		output[L[i]]++;
	}
	std::sort(output.begin(), output.end(), std::greater<int>());
	cout << count << "\n";
	for (int i = 1; i <= n; i++){
		if(output[i]!=0) cout << output[i] << "\n";
	}
}


int main() {
	cin >> n;
	cin >> m;
	vector<Edge> E(m+1);
	for (int i = 1; i <= m; i++) {
		cin >> E[i].u;
		cin >> E[i].v;
	}

	vector<int> L(n+1);
	cilk_for (int v = 1; v <= n; v++) {
		L[v] = v;
	}

	struct timeval start,end;
	gettimeofday(&start,NULL); //Start timing the computation
	L = par_randomized_cc(n, E, L, m);
	gettimeofday(&end,NULL); //Stop timing the computation
	double myTime = (end.tv_sec+(double)end.tv_usec/1000000) -
			 (start.tv_sec+(double)start.tv_usec/1000000);
	cout << "Time take by algorithm: " << myTime << " seconds.\n";
	dump_output(L, n);
}

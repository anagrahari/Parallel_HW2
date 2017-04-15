#include "par_cc_common.h"

typedef enum coinflip {
	HEAD = 0,
	TAIL = 1
} flip;

vector<int> par_randomized_cc(int n, vector<Edge> &E, vector<int> &L, int m){
	if (m == 0) 
		return L;
	vector<flip> C(n+1);
	vector<int> M(n+1);
	vector<int> S(m+1);

	cilk_for (int v = 1; v <= n; v++) {
		C[v] = rand() % 2 ? TAIL : HEAD;
	}

	cilk_for(int i = 1; i <= m; i++) {
		if (C[E[i].u] == TAIL && C[E[i].v] == HEAD) {
			L[E[i].u] = L[E[i].v];
		} else if (C[E[i].v] == TAIL && C[E[i].u] == HEAD) {
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

	S = par_prefix_sum(S);
	vector<Edge> F(S[m]+1);
	cilk_for (int i =1; i <= m; i++) {
		if (L[E[i].u] != L[E[i].v]) {
			F[S[i]].u = L[E[i].u];
			F[S[i]].v = L[E[i].v];
		}
	}

	M = par_randomized_cc(n, F, L, S[m]);
	cilk_for (int i = 1; i <= m; i++) {
		if (E[i].v == L[E[i].u]) {
			M[E[i].u] = M[E[i].v];
		} else if(E[i].u == L[E[i].v]) {
			M[E[i].v] = M[E[i].u];
		}
	}

	return M;
}

int main(int argc, char *args[]) {
	struct timeval start,end;

	if (argc < 2) { 
		cout << "Usage: " << "./par_randomized_cc <cilk_view{0|1}>  < input file\n";
		return 0;
	}

	cilk_view = atoi(args[1]);

	initialize();
	
	if (cilk_view == 1) {
		cout << "Running cilk_view code\n";
		cilkview_data_t d;
		__cilkview_query(d);
		L = par_randomized_cc(N, E, L, M);
		__cilkview_report(&d, NULL, "par_randomized_cc", CV_REPORT_WRITE_TO_RESULTS);
		return 0;
	}
 
	gettimeofday(&start,NULL); //Start timing of computation
	L = par_randomized_cc(N, E, L, M);
	gettimeofday(&end,NULL); //Stop timing of computation
	double time = (end.tv_sec+(double)end.tv_usec/1000000) -
			 (start.tv_sec+(double)start.tv_usec/1000000);
	cout << "Time taken by algorithm: " << time << " seconds.\n";
	dump_output(L, N);
}

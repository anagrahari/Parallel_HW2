#include<vector>
#include<algorithm>
#include<iostream>
#include<sys/time.h>
#include<cilk/cilk.h>
#include<cilktools/cilkview.h>
 
using namespace std;

typedef struct e {
	int u;
	int v;
} Edge;

int N = -1;
int M = -1;
int cilk_view = 0;
vector<Edge> E;
vector<int> L;

void initialize() {
	cin >> N;
	cin >> M;

	E.resize(M+1);
	for (int i = 1; i <= M; i++) {
		cin >> E[i].u;
		cin >> E[i].v;
	}

	L.resize(N+1);
	cilk_for (int v = 1; v <= N; v++) {
		L[v] = v;
	}
}


void dump_output(vector<int> &L, int n) {
	int count = 0;
	vector<int> output(n+1);

	for (int i = 1; i <= n; i++) {
		output[i]=0;
	}

	for (int i = 1; i <= n; i++) {
		if(output[L[i]]==0) 
			count++;

		output[L[i]]++;
	}

	std::sort(output.begin(), output.end(), std::greater<int>());

	cout << count << "\n";
	for (int i = 1; i <= n; i++) {
		if(output[i]!=0) 
			cout << output[i] << "\n";
	}
}


vector<int> par_prefix_sum(vector<int> &input) {
	int n = input.size();
	n = n - 1;
	vector<int> S(n+1);
	if (n == 1) {
		S[1] = input[1];
	} else {
		vector<int> Y(n/2 + 1);
		cilk_for (int i = 1; i <= n/2; i++) {
			Y[i] = input[2*i-1] + input[2*i];
		}

		vector<int> Z(n/2 + 1);
		Z = par_prefix_sum(Y);

		cilk_for (int i = 1; i <= n; i++ ) {
			if (i == 1) 
				S[1] = input[1];
			else if (i % 2 == 0)
				S[i] = Z[i/2];
			else 
				S[i] = Z[(i-1)/2] + input[i];
		}
	}
	return S;
}


int par_sum(vector<int> &X, int n) {
	vector<int> S(n+1);

	if (n == 1) {
		S[1] = X[1];
	} else {
		vector<int> Y(n/2 + 1);
		cilk_for (int i = 1; i <= n/2; i++) {
			Y[i] = X[2*i-1] + X[2*i];
		}

		vector<int> Z(n/2 + 1);
		Z = par_prefix_sum(Y);
		cilk_for (int i = 1; i <= n; i++ ) {
			if (i == 1)
				 S[1] = X[1];
			else if (i % 2 == 0)
				 S[i] = Z[i/2];
			else 
				S[i] = Z[(i-1)/2] + X[i];
		}
	}
	return S[n];
}

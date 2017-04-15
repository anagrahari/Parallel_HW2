#include<vector>
using namespace std;

typedef struct e {
	int u;
	int v;
} Edge;

int n;
int m;
vector<Edge> E;
vector<int> L;

void initialize() {
	cin >> n;
	cin >> m;
	E.resize(m+1);
	for (int i = 1; i <= m; i++) {
		cin >> E[i].u;
		cin >> E[i].v;
	}
	L.resize(n+1);
	cilk_for (int v = 1; v <= n; v++) {
		L[v] = v;
	}
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

vector<int> par_prefix_sum(vector<int> &input, int n) {
	vector<int> S(n+1);
	if (n == 1) {
		S[1] = input[1];
	}
	else {
		vector<int> y(n/2+1);
		cilk_for (int i = 1; i <= n/2; i++) {
			y[i] = input[2*i-1] + input[2*i];
		}

		vector<int> z(n/2 + 1);
		z = par_prefix_sum(y, n/2);
		cilk_for (int i = 1; i <= n; i++ ) {
			if (i == 1) 
				S[1] = input[1];
			else if (i % 2 == 0)
				S[i] = z[i/2];
			else 
				S[i] = z[(i-1)/2] + input[i];
		}
	}
	return S;
}

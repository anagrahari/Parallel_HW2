#include<vector>
int n;
int m;
using namespace std;
typedef struct e {
	int u;
	int v;
} Edge;

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

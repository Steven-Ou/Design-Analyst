#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

double calculateWinProbability(int n, double p, long long &multiplications)
{
    int W = (n / 2) + 1;
    multiplications = 0;

    vector<vector<double>> dp(W + 1, vector<double>(W + 1, 0.0));

    for (int j = 1; j <= W; j++) {
        dp[0][j] = 1.0;
    }
}

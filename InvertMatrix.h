#pragma once
# include <iostream>
# include <vector>
using namespace std;

vector<vector<int>> InvertBoolMatrix(const int& m, const int& n, const vector<vector<int>>& A)
{
	vector<vector<int>> res = A;
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			res[i][j] = 1 - A[i][j];
	return res;
}


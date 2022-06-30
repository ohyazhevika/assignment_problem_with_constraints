#pragma once
#include <iostream>
#include <fstream>
#include <exception>
#include <vector>
using namespace std;

struct AssignmentProblemInput
{
	int m;
	int n;
	vector<vector<int>> A;
	vector<int> d;
};

AssignmentProblemInput GetProblemDataFromFile(string file_name)
{
	AssignmentProblemInput problemInput;
	ifstream file(file_name);
	int m, n;
	file >> m >> n;
	problemInput.m = m;
	problemInput.n = n;
	problemInput.A =vector<vector<int>>(m, vector<int>(n));
	problemInput.d = vector<int>(n);
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			file >> problemInput.A[i][j];
	for (int j = 0; j < n; j++)
		file >> problemInput.d[j];

	return problemInput;
}


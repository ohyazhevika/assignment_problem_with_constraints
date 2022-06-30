#pragma once
#include "CombinationsGenerator.h"
#include <set>
#include <algorithm>


// !������ ������� ������ � ����������� �������� ����� ������� A(m*n) � ��������������� 
// ������������� d ��� ��������!

int lower_bound_for_K(const int& m, const int& n, vector<int> d);
bool is_covering_all_rows(const int& m, const vector<vector<int>>& v);
vector<vector<int>> generate_combinations_from_ed_elems(const int& d, const vector<int>& zero_indexes);
vector<vector<vector<int>>> generate_combinations_from_ed_elems_for_columns
(const int& m, const int& n, const vector<vector<int>>& a, const vector<int>& d);
void form_X(const int& m, const int& n, vector<vector<int>>& X, const vector<vector<int>>& ones_combination,
	const vector<int>& column_set);


// 1 - ���������� ��������, 0 - �����
// ������������ ���������� ��������������� ��������
// � ������� ���������� X ��� ������ � ����������� �������� � �������������
pair<int, vector<vector<int>>> ExactSolution(const int& m, const int& n, const vector<vector<int>>& a, const vector<int>& d)
{
	int number_of_covering_columns = 0;
	vector<vector<int>> X(m, vector<int>(n, 0));	// ������� ����������
	bool is_X_found = false;
	vector<vector<vector<int>>> zero_combinations_in_column = generate_combinations_from_ed_elems_for_columns(m, n, a, d);
	for (int K = lower_bound_for_K(m, n, d); K <= n && !is_X_found; K++)
	{
		// ��������� ������������ ��������� �������� ������� A �� K ����
		vector<vector<int>> K_set = generate_combinations(n, K);	

		for (int set_ind = 0; set_ind < K_set.size() && !is_X_found; set_ind++)
		{
			// ��������������� �� ������ ���� ��������� � ���� ��������
			vector<int> column_set = K_set[set_ind];	
			vector<vector<vector<int>>> combinations_in_columns(column_set.size());
			for (int j = 0; j < combinations_in_columns.size(); j++)
				combinations_in_columns[j] = zero_combinations_in_column[column_set[j]];
			vector<vector<vector<int>>> covers_for_column_set = generate_combinations_from_sets(combinations_in_columns);
			for (int cover_ind = 0; cover_ind < covers_for_column_set.size() && !is_X_found; cover_ind++)
			{
				if (is_covering_all_rows(m, covers_for_column_set[cover_ind]))
				{
					is_X_found = true;
					number_of_covering_columns = K;
					form_X(m, n, X, covers_for_column_set[cover_ind], column_set);
				}
			}
		}
	}
	return make_pair(number_of_covering_columns, X);
}


// ���������� ������ �� �������� ��������� ��������� ��� ������� ������� ������� a
// �.�. ones_indexes_in_column[j_fixed] - ������ �������� ���� ����� ����� i, ��� a[i][j_fixed]=1
vector<vector<int>> column_ones_indexes(const int& m, const int& n, const vector<vector<int>>& a)
{
	vector<vector<int>> ones_indexes_in_column(n);
	for (int j = 0; j < n; j++)
		for (int i = 0; i < m; i++)
			if (a[i][j] == 1)
				ones_indexes_in_column[j].push_back(i);
	return ones_indexes_in_column;
}

// ��� ������� �������� ����� � ��������� ��������� ������� � ������������ d
//  ���������� ������������ ��������� �������� ����� �
// ��������� ��������� �� d ������
vector<vector<int>> generate_combinations_from_ed_elems(const int& d, const vector<int>& ones_indexes)
{
	if (ones_indexes.size() <= d)
	{
		vector<vector<int>> res(1, ones_indexes);
		return res;
	}
	return generate_combinations(ones_indexes.size(), d, ones_indexes);
}

// ones_combinations_in_column[j_fixed] - ������ �� ������������ ����������
// �� �������� ��������� �������� ����� i �����, ��� a[i][j_fixed]=1
vector<vector<vector<int>>> generate_combinations_from_ed_elems_for_columns
(const int& m, const int& n, const vector<vector<int>>& a, const vector<int>& d)
{
	vector<vector<vector<int>>> ones_combinations_in_column(n);
	vector<vector<int>> ones_indexes_in_column = column_ones_indexes(m, n, a);
	for (int j = 0; j < n; j++)
		ones_combinations_in_column[j] = generate_combinations_from_ed_elems(d[j], ones_indexes_in_column[j]);
	return ones_combinations_in_column;
}

// ���������� false, ���� ����� ���������� ��������� ��������� ������� v, ��������� �� ������� ��� �������� v[j]
// (�.�. ��������� ��������� ������ ������� j) ������, ��� m - �� ���� ����� �� ��� ������ �������
bool is_covering_all_rows (const int& m, const vector<vector<int>>& v)
{
	set<int> covered_rows;
	for (int i = 0; i < v.size(); i++)
		for (int j = 0; j < v[i].size(); j++)
			covered_rows.insert(v[i][j]);
	if (covered_rows.size() < m)
		return false;
	else
		return true;
}

// ����������� ������� ���������� X, �������������� ������ ������� ������:
// ������ ������������ m � n ������ � ������� X � �������� ���������� ����������:
// 1) ��������� column_set ��������� ��� �������� �������� j_l, column_set[j_l] - �������� 
// ������ ������� � ������� A;
// 2) ��������� ones_combinations: ones_combinations[j_l] - ��������� �������� �����,
// ����������� �������� � �������� j=column_set[j_l]
void form_X(const int& m, const int& n, vector<vector<int>>& X, const vector<vector<int>>& zero_combination, 
	const vector<int>& column_set)
{
	int curr_cover_ind = 0;
	X = vector<vector<int>>(m, vector<int>(n, 0));
	for (int j = 0; j < n; j++)
	{
		if (curr_cover_ind >= column_set.size())
			break;
		if (j == column_set[curr_cover_ind])
		{
			for (int i = 0; i < zero_combination[curr_cover_ind].size(); i++)
			{
				X[zero_combination[curr_cover_ind][i]][j] = 1;
			}
			curr_cover_ind++;
		}
	}
}


// ������ ������������ ���������� ��������������
// ������� ����� ����� �������������: ����������� ���������� k ���� ����� d[j_l], 
// ��� d[j_1]+d[j_2]+...+d[j_k]>=m
int lower_bound_for_K(const int& m, const int& n, vector<int> d)
{
	sort(d.rbegin(), d.rend());
	int
		K_min = 0,
		covered = 0;
	for (int j = 0; j < n && covered < m; j++)
	{
		K_min++;
		covered += d[j];
	}
	return K_min;
}
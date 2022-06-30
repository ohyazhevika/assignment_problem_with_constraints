#pragma once

#include <iostream>
#include <vector>
#include <set>
#include <map>
using namespace std;

typedef pair<double, pair<int, int>> dii;


double get_P_for_column(const int& d, const int& z);
map<int, set<int>> ones_in_cols(const int& m, const int& n, const vector<vector<int>>& a);
map<int, set<int>> ones_in_rows(const int& m, const int& n, const vector<vector<int>>& a);
set<dii> risks_for_rows
(const int& m, const int& n, const vector<double>& P, map<int, set<int>> ones_in_rows,
	const vector<int>& d, map<int, set<int>> ed_indexes_in_column);
pair<int, set<int>> fill_column(set<dii>& risks_for_rows, const vector<int>& d,
	map<int, set<int>> ones_in_cols, map<int, set<int>> ones_in_rows);
void add_to_the_solution(const int& m, const int& n, vector<vector<int>>& X, vector<vector<int>>& A,
	const int& col, const set<int>& chosen_rows);
pair<int, set<int>> fill_column(const int& col, const int& capacity,
	const set<int>& ones_in_col);


// основная функция, объединяющая обе фазы
// на вход подаются размерности задачи m и n, матрица А и вектор ограничений d
// возвращаются полученное значение целевой функции и матрица назначений X
pair<int, vector<vector<int>>> ApproximateSolution(const int& m, const int& n, vector<vector<int>> a, const vector<int>& d)
{
	int number_of_covering_columns = 0;
	vector<vector<int>> X(m, vector<int>(n, 0));
	map<int, set<int>>
		o_in_cols = ones_in_cols(m, n, a),
		o_in_rows = ones_in_rows(m, n, a);

	/*cout << "ones in columns:\n";
	for (auto it = o_in_cols.begin(); it != o_in_cols.end(); it++)
	{
		int c = it->first;
		cout << "I_" << c << " = {";

		for (auto r_it = o_in_cols[c].begin(); r_it != o_in_cols[c].end(); r_it++)
		{
			if (r_it != o_in_cols[c].begin())
				cout << ", "; cout << *r_it;
		}
		cout << "};\n";
	}
	cout << "ones in rows:\n";
	for (auto it = o_in_rows.begin(); it != o_in_rows.end(); it++)
	{
		int r = it->first;
		cout << "J_" << r << " = {";

		for (auto c_it = o_in_rows[r].begin(); c_it != o_in_rows[r].end(); c_it++)
		{
			if (c_it != o_in_rows[r].begin())
				cout << ", "; cout << *c_it;
		}
		cout << "};\n";
	}*/

	vector<double> P(n);
	for (int j = 0; j < n; j++)
		P[j] = get_P_for_column(d[j], (int)o_in_cols[j].size());
	
	/*cout << "P(j):\n";
	for (auto it = o_in_cols.begin(); it != o_in_cols.end(); it++)
	{
		cout << "P(" << it->first << ") = " << P[it->first];
	}*/
	set<dii> risks = risks_for_rows(m, n, P, o_in_rows, d, o_in_cols);

	/*cout << "Risks:\n";
	for (auto it = risks.crbegin(); it != risks.crend(); it++)
	{
		dii risk = *it;
		cout << "R_" << risk.second.first << "(" << risk.second.second << ") = " << risk.first << "\n";
	}*/
	// основной цикл первой фазы
	while (!risks.empty())
	{
		pair<int, set<int>> covering = fill_column(risks, d, o_in_cols, o_in_rows);

		add_to_the_solution(m, n, X, a, covering.first, covering.second);
		number_of_covering_columns++;

		o_in_cols = ones_in_cols(m, n, a);
		o_in_rows = ones_in_rows(m, n, a);

		/*cout << "ones in columns:\n";
		for (auto it = o_in_cols.begin(); it != o_in_cols.end(); it++)
		{
			int c = it->first;
			cout << "I_" << c << " = {";

			for (auto r_it = o_in_cols[c].begin(); r_it != o_in_cols[c].end(); r_it++)
			{
				if (r_it != o_in_cols[c].begin())
					cout << ", "; cout << *r_it;
			}
			cout << "};\n";
		}
		cout << "ones in rows:\n";
		for (auto it = o_in_rows.begin(); it != o_in_rows.end(); it++)
		{
			int r = it->first;
			cout << "J_" << r << " = {";

			for (auto c_it = o_in_rows[r].begin(); c_it != o_in_rows[r].end(); c_it++)
			{
				if (c_it != o_in_rows[r].begin())
					cout << ", "; cout << *c_it;
			}
			cout << "};\n";
		}*/
		for (int j = 0; j < n; j++)
			P[j] = get_P_for_column(d[j], (int)o_in_cols[j].size());

		/*cout << "P(j):\n";
		for (auto it = o_in_cols.begin(); it != o_in_cols.end(); it++)
		{
			cout << "P(" << it->first << ") = " << P[it->first];
		}*/

		risks = risks_for_rows(m, n, P, o_in_rows, d, o_in_cols);

		/*cout << "Risks:\n";
		for (auto it = risks.crbegin(); it != risks.crend(); it++)
		{
			dii risk = *it;
			cout << "R_" << risk.second.first << "(" << risk.second.second << ") = " << risk.first << "\n";
		}*/
	}

	// на данный момент первая фаза нашего алгоритма считается завершенной, то есть
	// для каждой невычеркнутой строки i существует невычеркнутый столбец j такой, что
	// a[i][j]=1 для исходной матрицы A и z(j)<=d(j), где z(j) - количество невычеркнутых
	// единиц в j-м столбце

	o_in_cols = ones_in_cols(m, n, a);
	o_in_rows = ones_in_rows(m, n, a);
	while (!o_in_rows.empty() && !o_in_cols.empty())
	{
		int
			longest_col = o_in_cols.begin()->first,
			max_capacity = min((int)o_in_cols.begin()->second.size(), d[longest_col]);
		// ищем столбец с максимальной на данном этапе вместимостью
		for (auto it = ++o_in_cols.begin(); it != o_in_cols.end(); it++)
		{
			int capacity = min((int)it->second.size(), d[it->first]);
			if (capacity > max_capacity)
			{
				longest_col = it->first;
				max_capacity = capacity;
			}
		}
		// нашли  такой столбец
		pair<int, set<int>> covering = fill_column(longest_col, max_capacity, o_in_cols[longest_col]);
		add_to_the_solution(m, n, X, a, covering.first, covering.second);
		number_of_covering_columns++;
		o_in_cols = ones_in_cols(m, n, a);
		o_in_rows = ones_in_rows(m, n, a);
		/*cout << "ones in columns:\n";
		for (auto it = o_in_cols.begin(); it != o_in_cols.end(); it++)
		{
			int c = it->first;
			cout << "I_" << c << " = {";

			for (auto r_it = o_in_cols[c].begin(); r_it != o_in_cols[c].end(); r_it++)
			{
				if (r_it != o_in_cols[c].begin())
					cout << ", "; cout << *r_it;
			}
			cout << "};\n";
		}
		cout << "ones in rows:\n";
		for (auto it = o_in_rows.begin(); it != o_in_rows.end(); it++)
		{
			int r = it->first;
			cout << "J_" << r << " = {";

			for (auto c_it = o_in_rows[r].begin(); c_it != o_in_rows[r].end(); c_it++)
			{
				if (c_it != o_in_rows[r].begin())
					cout << ", "; cout << *c_it;
			}
			cout << "};\n";
		}*/
	}

	return make_pair(number_of_covering_columns, X);
}

// функция _P(j): для столбца j с пропускной способностью d и
// количеством невычеркнутых единиц в нем z возвращает P
double get_P_for_column(const int& d, const int& z)
{
	return (z <= d) ? 0 : 1 - 1.0 * d / z;
}


// функция, возвращающая множество невычеркнутых столбцов и соответствующее каждому такому столбцу
// j множество невычеркнутых единичных элементов нем ones_in_cols[j]
map<int, set<int>> ones_in_cols(const int& m, const int& n, const vector<vector<int>>& a)
{
	map<int, set<int>> ed_indexes_in_column;
	for (int j = 0; j < n; j++)
		for (int i = 0; i < m; i++)
			if (a[i][j] == 1)
			{
				ed_indexes_in_column[j].insert(i);
			}
	return ed_indexes_in_column;
}

// функция, возвращающая множество невычеркнутых строк и соответствующее каждой такой строке
// i множество невычеркнутых единичных элементов в ней ones_in_cols[i]
map<int, set<int>> ones_in_rows(const int& m, const int& n, const vector<vector<int>>& a)
{
	map<int, set<int>> ed_indexes_in_row;
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			if (a[i][j] == 1)
				ed_indexes_in_row[i].insert(j);
	return ed_indexes_in_row;
}

// функция, возвращающая для заданной строки row ее текущий максимально рискованный
// элемент с риском R(row) и соответстующим ему столбцом j(row),
// то есть пару чисел {R(row), j(row)};
// для строк, не являющимися рискованными, возвращается пара {0, -1}
pair<double, int> get_risk_and_col_for_row
(const int& row, const set<int>& ones_in_row, const vector<double>& P, 
	map<int, set<int>> ed_indexes_in_columns, const vector<int>& d)
{
	if (ones_in_row.empty())
	{
		return make_pair(0, -1);
	}
	double min_P = 2;
	int res_col = -1;
	for (auto it = ones_in_row.begin(); it != ones_in_row.end(); it++)
	{
		int col = *it;
		double P_j = P[col];
		if (P_j == 0)
			return make_pair(0, -1);
		if (P_j < min_P)
		{
			min_P = P_j;
			res_col = col;
		}
		else if (P_j == min_P)
		{
			if (min((int)ed_indexes_in_columns[col].size(), d[col]) >
				min((int)ed_indexes_in_columns[res_col].size(), d[res_col]))
				res_col = col;
		}
	}
	double risk = 1.0;
	for (auto it = ones_in_row.begin(); it != ones_in_row.end(); it++)
	{
		int col = *it;
		if (col != res_col)
			risk *= P[col];
	}
	return make_pair(risk, res_col);
}


// возвращает переопределенное множество R максимально "рискованных" элементов для
// каждой "рискованной" строки, то есть множество наборов {R(row), {row, j(row)}}
set<dii> risks_for_rows
(const int& m, const int& n, const vector<double>& P, map<int, set<int>> ones_in_rows,
	const vector<int>& d, map<int, set<int>> ed_indexes_in_column)
{
	set<dii> res;
	// последовательно рассматриваем строки
	for (int i = 0; i < m; i++)
	{
		pair<double, int> p = get_risk_and_col_for_row(i, ones_in_rows[i], P, ed_indexes_in_column, d);
		if (p.first != 0)
			res.insert(make_pair(p.first, make_pair(i, p.second)));
	}
	return res;
}

// функция, определяющая столбец, выбранный для вычеркивания на текущей итерации ПЕРВОЙ фазы,
// и множество выбранных для вычеркивания строк
// возвращаются индекс стоблца и индексы покрытых данным столбцом строк
pair<int, set<int>> fill_column(set<dii>& risks_for_rows, const vector<int>& d, 
	map<int, set<int>> ones_in_cols, map<int, set<int>> ones_in_rows)
{
	if (risks_for_rows.size() == 0)
		return make_pair(-1, set<int>());
	dii riskiest = *risks_for_rows.crbegin();	// элемент с наибольшим риском нашли
	int col = riskiest.second.second;	// наполняемый столбец
	int used_ports_cnt = 0;	// количество строк, покрытых выбранным столбцом
	set<int> chosen_rows;	// покрытые выбранным столбцом единиц
	for (auto it = --risks_for_rows.end(); it != risks_for_rows.end() && used_ports_cnt <d[col]; )
	{
		auto curr = it;
		if (it != risks_for_rows.begin())
			--it;
		else it = risks_for_rows.end();

		dii rrc = *curr;
		if(ones_in_cols[col].find(rrc.second.first)!= ones_in_cols[col].end())
		{
			chosen_rows.insert(rrc.second.first);
			used_ports_cnt++;
			risks_for_rows.erase(curr);
		}
	}
	if (used_ports_cnt < d[col])
	{
		for (auto it = ones_in_cols[col].begin(); it != ones_in_cols[col].end() && used_ports_cnt<d[col]; it++)
		{
			int row = *it;
			if (chosen_rows.find(row) == chosen_rows.end() )
			{
				used_ports_cnt++;
				chosen_rows.insert(row);
			}
		}
	}
	return make_pair(col, chosen_rows);
}


// Вычеркивание заполненного на данного итерации стоблца и покрытых им строк и пополнение решения X
void add_to_the_solution(const int& m, const int& n, vector<vector<int>>& X, vector<vector<int>>& A,
	const int& col, const set<int>& chosen_rows)
{
	for (auto it = chosen_rows.begin(); it != chosen_rows.end(); ++it)
	{
		int row = *it;
		X[row][col] = 1;
		for (int j = 0; j < n; j++)
			A[row][j] = 0;
	}
	for (int i = 0; i < m; i++)
		A[i][col] = 0;
}

// функция, определяющая столбец, выбранный для вычеркивания на текущей итерации ВТОРОЙ фазы,
// и множество выбранных для вычеркивания строк
// возвращаются индекс стоблца и индексы покрытых данным столбцом строк
pair<int, set<int>> fill_column(const int& col, const int& capacity, 
	const set<int>& ones_in_col)
{
	set <int> rows;
	auto it = ones_in_col.begin();
	for (int i = 0; i < capacity; i++)
	{
		rows.insert(*it);
		it++;
	}
	return make_pair(col, rows);
}

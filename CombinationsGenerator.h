#pragma once
#include <iostream>
#include <vector>
#include <math.h>
using namespace std;

vector<int> next_combination(vector<int> a, int n);


// ищутся все сочетания без повторений из n по k для элементов из множества {0, 1, 2,..., n-1}
vector<vector<int>> generate_combinations(const int& n, const int& k)
{
	vector<vector<int>> combinations;
	vector<int> comb0(k);
	for (int i = 0; i < k; i++)
		comb0[i] = i;
	combinations.push_back(comb0);
	vector<int> next_comb = next_combination(combinations[0], n);
	int i = 0;
	while (next_comb.size() != 0)
	{
		i++;
		combinations.push_back(next_comb);
		next_comb = next_combination(combinations[i], n);
	}
	return combinations;

}

// возвращает следующую в лексикографическом порядке комбинацию-сочетание после заданного
// (ищутся сочетания без повторений из n по k)
vector<int> next_combination(vector<int> a, int n) {
	int k = a.size();
	for (int i = k - 1; i >= 0; --i)
		if (a[i] < n - k + i) {
			++a[i];
			for (int j = i + 1; j < k; ++j)
				a[j] = a[j - 1] + 1;
			return a;
		}
	return vector<int>(0);
}

// производится маппинг индексов массива elems с соответствующими значениями
// или: ищутся все сочетания без повторений из n по k для элементов заданного множества elems
vector<vector<int>> generate_combinations(const int& n, const int& k, const vector<int>& elems)
{
	vector<vector<int>> combinations = generate_combinations(n, k);
	for (int i = 0; i < combinations.size(); i++)
		for (int j = 0; j < k; j++)
			combinations[i][j] = elems[combinations[i][j]];
	return combinations;
}


// ищутся всевозможные k-элементные комбинации {m_1, m_2,..., m_k}, где m_j - элемент множества M_j
// мощности множеств M_j различны
// elems_in_set[j] - векторов из элементов множества M_j
// функция возвращает вектор векторов-найденных комбинаций
template <typename T>
vector<vector<T>> generate_combinations_from_sets(const vector<vector<T>> elems_in_set)
{
	int num_of_sets = elems_in_set.size();
	int num_of_combinations = 1;	// общее количество комбинаций посчитаем
	for (int i = 0; i < num_of_sets; i++)
		num_of_combinations *= elems_in_set[i].size();

	vector<vector<T>> combinations(num_of_combinations, vector<T>(num_of_sets));	// результат - множество всех комбинаций

	int num_of_repetitions = num_of_combinations;	//сколько раз подряд будут вставляться на i-e место элементы из i-го множества

	for (int j = 0; j < num_of_sets; j++)	//последовательно заполняем j-ые места в каждой из num_of_combinations комбинаций
	{
		num_of_repetitions /= elems_in_set[j].size();
		int num_of_iters = num_of_combinations / num_of_repetitions;
		int combination_index = 0;
		int elem_ind = 0;
		for (int it = 0; it < num_of_iters; it++)
		{
			for (int rep = 0; rep < num_of_repetitions; rep++)
			{
				combinations[combination_index][j] = elems_in_set[j][elem_ind];
				combination_index++;
			}
			elem_ind = (elem_ind == elems_in_set[j].size() - 1) ? 0 : (elem_ind + 1);
		}
	}
	return combinations;
}
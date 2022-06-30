#include "CombinationsGenerator.h"
#include "ExactSolution.h"
#include "DataInput.h"
#include "ApproximateSolution.h"
#include "InvertMatrix.h"
#include <string>
#include <iostream>
#include <fstream>
#include <windows.h>
using namespace std;

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
int classic = 240, special = 244;

template <typename T>
ostream& operator <<(ostream& output, const vector<vector<T>>& data)
{
	for (int i = 0; i < data.size(); i++)
	{
		output << " | ";
		for (int j = 0; j < data[i].size(); j++)
		{
			if (data[i][j] == 0)
			{
				SetConsoleTextAttribute(console, classic);
			}
			else 
				SetConsoleTextAttribute(console, special);

			output << data[i][j] << " ";
		}
		SetConsoleTextAttribute(console, classic);
		output << " | \n";
	}
	return output;
}
ostream& operator <<(ostream& output, const vector<int>& data)
{
	output << " | ";
	for (int i = 0; i < data.size(); i++)
	{
		output << data[i] << " ";
	}
	output << " | \n";
	return output;
}

template <typename T>
void print_vector(const vector<T>& v, string c = "")
{
	for (int i = 0; i < v.size(); i++)
		cout << v[i] << c;
}

template <typename T>
void print_matrix(const vector<vector<T>>& m)
{
	for (int i = 0; i < m.size(); i++)
	{
		print_vector(m[i], " ");
		cout << '\n';
	}
}
void main()
{
	setlocale(LC_ALL, "Russian");
	AssignmentProblemInput input = GetProblemDataFromFile("Test1.txt");
	SetConsoleTextAttribute(console, classic);
	cout << "  Исходные данные задачи о покрытии с булевой матрицей и дополнительными ограничениями.\n";
	cout << "  m = " << input.m << '\t' << "n = " << input.n << "\n\n";
	cout << "\tМатрица A:\n" << input.A << "\n\n" << "\tОграничения d[j]:\n"<< input.d << "\n\n";
	cout << "---------------------------------------------------------------------------------------\n";
	pair<int, vector<vector<int>>> appoximate_solution
		= ApproximateSolution(input.m, input.n, input.A, input.d);
	cout << " Приближенное решение: \n Значение целевой функции:\t"<< appoximate_solution.first
		<<"\n Матрица назначений X:\n"<< appoximate_solution.second;
	

	cout << "---------------------------------------------------------------------------------------\n";
	pair<int, vector<vector<int>>> exact_solution
		= ExactSolution(input.m, input.n, input.A, input.d);
	cout << " Точное решение: \n Значение целевой функции:\t" << exact_solution.first
		<< "\n Матрица назначений X:\n" << exact_solution.second;
	/*cout << InvertBoolMatrix(input.m, input.n, input.A);*/
}


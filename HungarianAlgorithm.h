#pragma once
#include <iostream>
#include <vector>
using namespace std;

static class HungarianAlgorithm
{
public:
    vector<int> Run2(int** A, int n, int m)
    {
        int** a = new int* [n+1];
        for (int i = 0; i <= n; i++)
            a[i] = new int[m + 1];
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= m; j++)
                a[i][j] = A[i - 1][j - 1];
        int INF = 10000;

        vector<int> u(n + 1), v(m + 1), p(m + 1), way(m + 1);
        for (int i = 1; i <= n; ++i) {
            p[0] = i;
            int j0 = 0;
            vector<int> minv(m + 1, INF);
            vector<char> used(m + 1, false);
            do {
                used[j0] = true;
                int i0 = p[j0], delta = INF, j1;
                for (int j = 1; j <= m; ++j)
                    if (!used[j]) {
                        int cur = a[i0][j] - u[i0] - v[j];
                        if (cur < minv[j])
                            minv[j] = cur, way[j] = j0;
                        if (minv[j] < delta)
                            delta = minv[j], j1 = j;
                    }
                for (int j = 0; j <= m; ++j)
                    if (used[j])
                        u[p[j]] += delta, v[j] -= delta;
                    else
                        minv[j] -= delta;
                j0 = j1;
            } while (p[j0] != 0);
            do {
                int j1 = way[j0];
                p[j0] = p[j1];
                j0 = j1;
            } while (j0);
        }
        vector<int> ans(n + 1);
        for (int j = 1; j <= m; ++j)
            ans[p[j]] = j;
        return ans;
    }
    int Run(int** a, int n)
    {
        vector<vector<int>> H(n);   //граф жестких ребер двудольный
        vector<int> M(n);           //текущее паросочетание
        vector<int> u(n, 0);
        vector<int> v(n, 0);

        //первый шаг к поиску жёстких рёбер - ищем потенциалы u и v
        for (int i = 0; i < n; i++)
        {
            u[i] = a[i][0];
            for (int k = 1; k < n; k++)
            {
                if (u[i] > a[i][k])
                    u[i] = a[i][k];
            }
        }
        for (int j = 0; j < n; j++)
        {
            v[j] = a[0][j] - u[0];
            for (int k = 1; k < n; k++)
            {
                if (v[j] > a[k][j] - u[k])
                    v[j] = a[k][j] - u[k];
            }
        }
        //заполняем H имеющимися жёсткими рёбрами (i, j): (a[i][j] - u[i] - v[j] = 0)
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
                if (a[i][j] - u[i] - v[j] == 0)
                    H[i].push_back(j);
        }
        //находим некоторое паросочетание в графе H - необязательно максимальное
        //вытаскиваем ли из графа Н полностью паросочетание текущее? 
        for (int i = 0; i < n; i++)
        {
            if (!H[i].empty())
            {
                int t = H[i].back();
                H[i].pop_back();
                M[t] = i;

            }
        }

        return 0;
    }
};
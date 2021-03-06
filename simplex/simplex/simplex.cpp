#include "pch.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class Vector
{
private:
	int n;
	double *Item;
public:
	Vector()
	{

	}
	Vector(double *A, int nn)
	{
		n = nn;
		Item = new double[n];
		for (int i = 0; i < n; i++)
			Item[i]=A[i];
	}
	/*Vector(Vector &vec)
	{
		n = vec.n;
		Item = new double[n];
		for (int i = 0; i < n; i++)
			Item[i] = vec[i];
	}*/
	Vector operator*(double a)
	{
		double *Item2 = new double[n];
		for (int i = 0; i < n; i++)
			Item2[i] = Item[i] * a;
		return Vector(Item2,n);
	}
	Vector operator+(Vector v)
	{
		double *Item2 = new double[n];
		for (int i = 0; i < n; i++)
			Item2[i] = Item[i] +v.Item[i];
		return Vector(Item2, n);
	}

	double operator[](int k)
	{
		return Item[k];
	}
	/*Vector& operator=(Vector &v)
	{
		double *Item2 = new double[v.n];
		for (int i = 0; i < v.n; i++)
			Item2[i] = v.Item[i];
		return Vector(Item2, v.n);
	}*/

	void VectorDivNumber(double a)
	{
		for (int i = 0; i < n; i++)
			Item[i] = Item[i]/a;
	}
	void Show()
	{
		for (int i = 0; i < n; i++)
		{
			cout << Item[i] << "\t";
		}
	}

};
class Matrix
{
	Vector *Row;
	int n, m;
public:
	Matrix()
	{

	}
	Matrix(double** A,int nn, int mm)
	{
		n = nn; m = mm;
		Row = new Vector[n];
		for (int i = 0; i < n; i++)
				Row[i] = Vector(A[i],m);
	}
	void ZeroCol(int x, int y)
	{
		Row[x].VectorDivNumber(Row[x][y]);
		int k = (x + 1) % n;
		for (int i = 0; i < n-1; i++)
		{
			double a = Row[k][y];
			Row[k] = Row[k] + Row[x] * (-Row[k][y]);
			k = (k + 1) % n;
		}
	}
	Vector operator[](int k)
	{
		return Row[k];
	}
	void Show()
	{
		for (int i = 0; i < n; i++)
		{
			Row[i].Show();
			cout << endl;
		}
	}
	void find_last_col_min(double& min, int &min_i, bool &exists)
	{
		min = 0;
		for (int i = 0; i < n - 1; i++)
		{
			double last = Row[i][m - 1];
			if (last < 0 && last < min)
			{
				min = last;
				min_i = i;
				exists = true;
			}
		}
	}
	void find_col_min_ratio(double& min, int min_i,int &min_j, bool &exists)
	{
		double ratio = 0;
		for (int j = 0; j < m - 1; j++)
		{
			if (Row[min_i][j] < 0)
			{
				ratio = abs(Row[min_i][m - 1] / Row[min_i][j]);
				if (!exists)
				{
					min = ratio; exists = true;
				}
				if (ratio <= min)
				{
					min_j = j; min = ratio;
				}
				//cout << j << "\t";
			}
		}
		//cout << endl;
	}
	void find_ResCol(int &res_j, bool find_min,bool &exists, bool *marked_col)
	{
		double max = 0; res_j = -1;
		for (int i = 0; i < m-1; i++)
		{
			double row_i = Row[n - 1][i]; 
			if (!marked_col[i] && (find_min == row_i > 0) && (abs(row_i) > max))
			{
					exists = true;
					res_j = i;
					max = abs(row_i);
					//cout << res_j << endl;
			}
		}
	}
	void find_ResRow(int &res_i, int res_j, bool &exists)
	{
		exists = false; double min = 0;double ratio = 0;
		for (int i = 0; i < n-1; i++)
		{
			if (Row[i][res_j] > 0)
			{
				ratio = Row[i][m - 1] / Row[i][res_j];
				//cout << Row[i][res_j] << endl;
				if (!exists)
				{
					min = ratio; exists = true;
				}
				if (ratio <= min)
				{
					res_i = i; min = ratio;
				}
			}
		}
	}
};
const int MarkSign[] = { -1,0,1 };
const string Neq[] = {">=","=","<="};
class SimplexMetod
{
	int n, m,NeqCount,rank;
	int new_n;
	Vector ObjectFunction;
	Matrix matrix;
	int* basis_numbers;
	bool Find_min;
	double* Solution;
public:
	SimplexMetod(bool find_min)
	{
		Find_min = find_min;
		NeqCount = 0; rank = 0;
		ReadFile();
	}
	Matrix origin;
	void ReadFile()
	{
		NeqCount = 0;
		ifstream f("problem.dat"); // открыли файл для чтения
		if (!f) { cout << "Нет файла входных данных problem.dat\n"; return; }
		f >> n>>m;
		int *Sign = new int[m];
		for (int i = 0; i < m; i++)
		{
			f >> Sign[i];
			if(Sign[i]!=0) NeqCount++;
		}
		new_n = n+ NeqCount;
		double **A = new double*[m];
		for (int i = 0; i < m; i++) A[i] = new double[new_n+1];
		int k = 0;
		for (int i = 0; i < m; i++)
		{
			memset(A[i], 0, sizeof(double)*(new_n+1));
			for (int j = 0; j < n; j++)
				f >> A[i][j];
			if (Sign[i] != 0)
			{
				A[i][n + k] = Sign[i];
				k++;
			}
		}
		double *obj_vec = new double[new_n + 1];
		memset(obj_vec, 0, sizeof(double)*(new_n + 1));
		for (int i = 0; i < m; i++)
			f >> A[i][new_n];
		for (int i = 0; i <=n; i++)
			f >> obj_vec[i];
		f.close();
		ShowTask(obj_vec, A,Sign);
		/*for (int i = 0; i < m; i++)
		{
			for (int j = 0; j <= new_n; j++) cout << A[i][j]<<"\t";
				cout << endl;
		}*/
		origin= Matrix(A, m, new_n + 1);
		/*
		for (int i = 0; i < m; i++)
		{
			for (int j = 0; j <= new_n; j++) cout << origin[i][j] << "\t";
			cout << endl;
		}*/

		bool *basis_col = new bool[new_n];
		bool *basis_row = new bool[m];
		memset(basis_col, 0, sizeof(bool)*new_n);
		memset(basis_row, 0, sizeof(bool)*m);

		for (int i = new_n - 1; i >= 0; i--)
		{
			for (int j = m - 1; j >= 0; j--)
				if (origin[j][i] != 0 && !basis_row[j])
				{
					rank++;
					origin.ZeroCol(j, i);
					basis_col[i] = true;
					basis_row[j] = true;
					break;
				}
		}
		basis_numbers = new int[rank];
		int* basis_rows = new int[rank];
		k = 0;
		for (int i = 0; i < new_n; i++)
			if (basis_col[i])
			{				
				basis_numbers[k] = i;
				k++;
			}
		k = 0;
		for (int i = 0; i < m; i++)
		{
			if (basis_row[i])
			{
				basis_rows[k] = i;
				k++;
			}
		}
		double *new_obj_vec = new double[new_n + 1];
		obj_vec[new_n] = -obj_vec[new_n];
		for (int i = 0; i <= new_n; i++)
		{
			if (basis_col[i])
			{
				new_obj_vec[i] = 0;
			}
			else
			{
				new_obj_vec[i] = obj_vec[i];
				
				for (int j = 0; j < rank; j++)
				{
					int p = basis_numbers[j]; int q = basis_rows[j];
					new_obj_vec[i] = new_obj_vec[i] - origin[q][i] * obj_vec[p];
				}
			}
		}
		//for (int j = 0; j <= new_n; j++) cout << new_obj_vec[j] << "\t";
		double** simplex_matrix = new double*[rank+1];
		for (int i = 0; i < rank; i++)
		{
			simplex_matrix[i] = new double[new_n + 1];
			int q = basis_rows[i];
			for (int j = 0; j <= new_n; j++)
				simplex_matrix[i][j] = origin[q][j];
		}
		simplex_matrix[rank] = new double[new_n + 1];
		for (int j = 0; j <= new_n; j++) simplex_matrix[rank][j] = -new_obj_vec[j];
		matrix = Matrix(simplex_matrix,rank+1,new_n+1);
		//matrix.Show();
	}
	void Execute()
	{
		bool no_solution = false;
		Solution = new double[new_n];
		memset(Solution, 0, sizeof(double)*(new_n-1));
		for (int i = 0; i < rank; i++)
		{
			Solution[basis_numbers[i]] = matrix[i][new_n];
			//cout << basis_numbers[i] << "\t" << matrix[i][new_n] << endl;
		}
		//matrix.Show();
		DualSimplex(no_solution);
		//matrix.Show();

		if (no_solution)
		{
			cout << "Нет оптимального решения\n";
			return;
		}
		bool optimal_value_exists = false;
		while (!(optimal_value_exists ||no_solution))
		{
			int res_i = -1;
			int k = 0;
			bool *marked_col = new bool[new_n];
			memset(marked_col, 0, sizeof(bool)*new_n);
			bool exists = false;
			int res_j = -1;
			while (!exists)
			{
				exists = false; res_i = -1; res_j = -1;
				matrix.find_ResCol(res_j, Find_min, exists, marked_col);
				//cout << exists <<"l"<< endl;
				if (!exists && k == 0)
				{
					optimal_value_exists = true;
					break;
				}
				if (!exists && k>0)
				{
					optimal_value_exists = false;
					no_solution = true;
					break;
				}
				exists = false;marked_col[res_j] = true;
				matrix.find_ResRow(res_i, res_j, exists);
				if (exists)
				{
					//Solution[basis_numbers[res_i]] = 0;
					matrix.ZeroCol(res_i,res_j);
					//matrix.Show();
					//cout << res_i << " "<<res_j << endl;
					basis_numbers[res_i] = res_j;
					//Solution[res_j] = matrix[res_i][new_n];
					break;
				}
				k++;
			}

			//matrix.Show();

		}
		for (int i = 0; i < rank; i++)
		{
			Solution[basis_numbers[i]] = matrix[i][new_n];
			//cout << basis_numbers[i] << "\t" << matrix[i][new_n] << endl;
		}
		if (optimal_value_exists)
		{
			cout << "\nx*=(";
			for (int i = 0;i<n;i++)
			{
				cout << Solution[i] << ";";
			}
			cout << "); F(x*)="<< matrix[rank][new_n];
			if (Find_min) cout << "->min\n"; else cout << "->max\n";
		}
		else
			cout << "Нет оптимального решения\n";
	}
	void DualSimplex(bool &no_solution)
	{
		while (true) 
		{
			double min = 0; int min_i =-1;
			bool min_exists = false;
			matrix.find_last_col_min(min, min_i, min_exists);
			if (!min_exists) return;
			min = 0; min_exists = false;
			int min_j = -1;
			matrix.find_col_min_ratio(min, min_i,min_j, min_exists);
			if (!min_exists)
			{
				no_solution = true;
				return;
			}
			//Solution[basis_numbers[min_i]] = 0;
			//cout << min_i << " " << min_j << endl;
			matrix.ZeroCol(min_i, min_j);
			//matrix.Show();

			basis_numbers[min_i] = min_j;
			//Solution[min_j] = matrix[min_i][new_n];
			//for(j=0;j<m;j++)
		}
	}
	void ShowTask(double *obj, double **A,int* sign)
	{
		cout << "f(x)=";
		for (int i = 0; i < n; i++)
		{
			PrintCoef( obj[i],i, "x");
		}
		PrintCoef(obj[n], "");
		cout << endl;
		for (int i = 0; i < m; i++)
		{
			for (int j = 0; j < n; j++)
			{
				PrintCoef(A[i][j], j, "x");
			}
			cout << Neq[sign[i]+1] << A[i][new_n];
			cout << endl;
		}
	}
	void PrintCoef(double a, int k,string x)
	{
		if (a != 0)
		{
			if (a > 0 && k != 0)
				cout << "+" << a<< x<< (k+ 1);
			else
				cout <<a << x << (k + 1);
		}
	}
	void PrintCoef(double a, string x)
	{
		if (a != 0)
		{
			if (a > 0)
				cout << "+" << a << x;
			else
				cout << a << x;
		}
	}
};

int main()
{
	setlocale(LC_ALL, "Russian");
	SimplexMetod simplex(true);
	simplex.Execute();
	system("pause");
}

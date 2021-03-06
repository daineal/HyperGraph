#include "pch.h"
#include <iostream>
#include <fstream>
using namespace std;
void ExceptionInputs(void);
class HyperGraph
{
	int ArcCount;
	int VertexCount;
	bool** IncidenceMatrix;
public:
	HyperGraph()
	{
		ReadFile();
	}

	void ReadFile()
	{
		ifstream f("HyperGraph.dat"); // открыли файл для чтения
		if (!f) { cout << "Нет файла входных данных\n"; exit(0); }
		f >> VertexCount >> ArcCount;
		InitMatrix(IncidenceMatrix, VertexCount, ArcCount);
		for (int i = 0; i < VertexCount; i++)
			for (int j = 0; j < ArcCount; j++)
			{
				f >> IncidenceMatrix[i][j];
			}
		f.close();
	}
	void InitMatrix(bool** &A, int n,int m)
	{
		A = new bool*[m];
		for (int i = 0; i < n; i++)
		{
			A[i] = new bool[m];
			memset(A[i], 0, m * sizeof(bool));
		}
	}
	void ShowIncidenceMatrix()
	{
		cout << "Матрица инциденций:\n";
		for (int i = 0; i < VertexCount; i++)
		{
			for (int j = 0; j < ArcCount; j++)
			{
				cout << IncidenceMatrix[i][j] << "\t";
			}
			cout << endl;
		}
		cout << "Число вершин: " << VertexCount << endl;
		cout << "Число ребер: " << ArcCount << endl;
		if (SimpleGraph()) cout << "Гипреграф простой\n";
		else
		{
			cout << "Гипреграф непростой\n";
			if (MultiGraph()) cout << "Мультигиперграф\n";
			if (LoopExists()) cout << "Есть петли\n";
		}

	}
	void ShowAdjacencyList()
	{
		cout << "Список смежности:\n";
		for (int i = 0; i < VertexCount; i++)
		{
			cout << "Вершина " << i + 1 << ": ";
			for (int j = 0; j < VertexCount; j++)
			{
				if (AdjacencyVertex(i, j) && i!=j) cout << j + 1 << ";";
			}
			cout << endl;
		}
	}
	void ShowIncidenceList()
	{
		cout << "Список инцидентности:\n";
		for (int i = 0; i < ArcCount; i++)
		{
			cout << "Ребро " << i + 1 << ": ";
			for (int j = 0; j < ArcCount; j++)
			{
				if (i!=j && IncidenceArcs(i, j)) cout << j + 1 << ";";
			}
			cout << endl;
		}
	}
	bool AdjacencyVertex(int p, int q)//Проверка вершин на смежность
	{
		for (int i = 0; i < ArcCount; i++)
		{
			if (IncidenceMatrix[p][i] && IncidenceMatrix[q][i]) return true;
		}
		return false;
	}
	bool IncidenceArcs(int p, int q)//Проверка рёбер на инцидентность
	{
		for (int i = 0; i < VertexCount; i++)
		{
			if (IncidenceMatrix[i][p] && IncidenceMatrix[i][q]) return true;
		}
		return false;
	}
	bool MultiArcs(int p, int q)//Проверка рёбер на кратность
	{
		for (int i = 0; i < VertexCount; i++)
		{
			if (!(IncidenceMatrix[i][p]==IncidenceMatrix[i][q])) return false;
		}
		return true;
	}
	bool MultiGraph()//Проверка на мультигиперграф
	{
		for (int i = 0; i < ArcCount; i++)
			for (int j = i + 1; j < ArcCount; j++)
				if (MultiArcs(i, j)) return true;
		return false;
	}
	bool Loop(int p)//Проверка рёбер на петлю
	{
		int count = 0;
		for (int i = 0; i < VertexCount; i++)
		{
			count = count + (IncidenceMatrix[i][p]);
		}
		return count==1;
	}
	bool LoopExists()//Проверка на наличие петель
	{
		int count = 0;
		for (int i = 0; i < VertexCount; i++)
		{
			if(Loop(i)) return true;
		}
		return false;
	}

	bool SimpleGraph()
	{
		return !MultiGraph()&& !LoopExists();
	}
	void ShowLoop()
	{
		cout << "Рёбра-петли: ";
		for (int i = 0; i < ArcCount; i++)
		{
			if (Loop(i)) cout << i + 1 << ";";
		}
		cout << endl;
	}
	void ShowMultiArcs()
	{
		cout << "Кратные рёбра: ";
		bool *marked = new bool[ArcCount];
		memset(marked, 0, sizeof(bool)*ArcCount);
		for (int i = 0; i < ArcCount; i++)
		{
			if (!marked[i])
			{
				bool exists = false;
				for (int j = i+1; j < ArcCount; j++)
				{
					if (MultiArcs(i, j))
					{
						marked[j] = true;
						cout << j + 1 << ";";
						exists = true;;
					}
				}
				if (exists)
				{
					cout << i + 1 << "\n";
					marked[i] = true;
				}
			}
		}
		cout << endl;
	}
	void Menu()
	{
		int k = -1;
		while (k != 6)
		{
			cout << "Гиперграф.\n";
			cout << "1) Матрица инциденций.\n";
			cout << "2) Список смежности.\n";
			cout << "3) Список инцидентности.\n";
			cout << "4) Показать рёбра-петли.\n";
			cout << "5) Показать кратные рёбра.\n";
			cout << "6) Выход.\n";
			while (k <= 0||k>6)
			{
				cout << "Введите номер команды: "; cin >> k;
				ExceptionInputs();
			}
			switch (k)
			{
				case 1:ShowIncidenceMatrix(); break;
				case 2:	ShowAdjacencyList(); break;
				case 3:	ShowIncidenceList(); break;
				case 4:	ShowLoop(); break;
				case 5:ShowMultiArcs(); break;
				case 6:exit(0); break;
			}
			k = -1;
			system("pause");
			system("cls");
		}
	}
};


int main()
{
	setlocale(LC_ALL, "Russian");
	HyperGraph A;
	A.Menu();
	system("pause");
}

void ExceptionInputs(void)
{
	if (cin.fail())
	{
		cin.clear();
		cin.ignore(32767, '\n');
	}
}

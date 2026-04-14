#include <fstream>
#include <iostream>
#include <vector>

#include "../lab1Functions.h"

using namespace std;

/* Представленные задания оформить в виде текстового файла.
 *
 * В первом файле хранится k матриц размерности m x n, во втором - l матриц размерности m x n. Те матрицы из первого
 * файла, у которых a00 = 0, перенести в конец второго файла. Вывести на экран содержимое первого и второго файлов. */

// Описываю матрицу как вектор векторов
struct Matrix
{
  vector<vector<double>> data;
};

// Ввод с последующим заполнением вектора структур
vector<Matrix> matricesInput(const int& matricesCount, const int& rows, const int& cols)
{
  ifstream matricesInput("../lab1/testFiles/tSeven/matrices.txt");
  if (!matricesInput.is_open())
  {
    cerr << "Ошибка открытия файла" << endl;
    exit(1);
  }

  // Создаем вектор матриц длинной matricesCount и инициализируем структурами, которые инициализируем через количество
  // векторов строк заполненных векторами столбцов
  vector<Matrix> matrices(matricesCount, Matrix{vector<vector<double>>(rows, vector<double>(cols))});

  for (int i = 0; i < matricesCount; i++)
    for (int m = 0; m < rows; m++)
      for (int n = 0; n < cols; n++)
        matricesInput >> matrices[i].data[m][n];

  matricesInput.close();
  return matrices;
}

void printMatrices(const vector<Matrix>& matrices)
{
  for (const Matrix& matrix : matrices)
  {
    for (const vector<double>& row : matrix.data)
    {
      for (const double& value : row)
        cout << value << " ";
      cout << endl;
    }
  cout << endl;
  }
}

void writeMatrices(const vector<Matrix>& matrices)
{
  ofstream output("../lab1/testFiles/tSeven/output.txt");
  if (!output.is_open())
  {
    cerr << "Ошибка открытия файла для записи" << endl;
    exit(1);
  }

  for (const Matrix& matrix : matrices)
  {
    for (const vector<double>& row : matrix.data)
    {
      for (const double& value : row)
        output << value << " ";
      output << endl;
    }
    output << endl;
  }

  output.close();
  cout << "Обработанные матрицы записаны в файл output.txt" << endl;
}


void taskSeven()
{
  int matricesCount, rows, cols;
  cout << "Введите количество матриц: ";
  cin >> matricesCount;
  cout << "Введите количество строк, затем колонок: ";
  cin >> rows >> cols;
  cout << endl;

  const vector<Matrix> originalMatrices = matricesInput(matricesCount, rows, cols);
  vector<Matrix> firstMatrices, secondMatrices;

  for (const Matrix& matrix : originalMatrices)
    if (matrix.data[0][0] == 0)
      secondMatrices.push_back(matrix);
    else
      firstMatrices.push_back(matrix);

  vector<Matrix> processedMatrices = firstMatrices;
  processedMatrices.insert(processedMatrices.end(), secondMatrices.begin(), secondMatrices.end());

  cout << "--- Изначальные матрицы ---\n" << endl;
  printMatrices(originalMatrices);
  cout << "--- Матрицы после обработки ---\n" << endl;
  printMatrices(processedMatrices);
  writeMatrices(processedMatrices);
}

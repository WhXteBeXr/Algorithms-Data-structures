#include <sstream>
#include "SparseMatrix.h"

int main()
{
  // 1. Создание матриц и заполнение (хранятся только ненулевые элементы)
  std::cout << "1. Матрицы A и B (3x3):\n";
  SparseMatrix a(3, 3), b(3, 3);
  a.set(0, 0, 1);
  a.set(1, 2, 2);
  a.set(2, 1, 3);
  b.set(0, 1, 4);
  b.set(1, 2, 5);
  b.set(2, 0, 6);
  std::cout << "A (ненулевых: " << a.getSize() << "):\n" << a;
  std::cout << "B (ненулевых: " << b.getSize() << "):\n" << b << "\n";

  // 2. Арифметические операторы
  std::cout << "2. Арифметика:\nA + B:\n" << a + b;
  std::cout << "A - B:\n" << a - b;
  std::cout << "A * B (матричное):\n" << a * b;
  std::cout << "A / 2:\n" << a / 2 << "\n";

  // 3. Дружественные функции: матрица * число и число * матрица
  std::cout << "3. Умножение на число:\nA * 3:\n" << a * 3;
  std::cout << "0.5 * B:\n" << 0.5 * b << "\n";

  // 4. Составное присваивание
  std::cout << "4. Составное присваивание:\n";
  SparseMatrix c = a;
  c += b;
  std::cout << "C = A; C += B:\n" << c;
  c -= b;
  std::cout << "C -= B:\n" << c;
  c *= 2;
  std::cout << "C *= 2:\n" << c;
  c /= 2;
  std::cout << "C /= 2:\n" << c << "\n";

  // 5. Сравнение
  std::cout << "5. Сравнение:\n";
  std::cout << "C == A? " << (c == a ? "да" : "нет") << "\n";
  std::cout << "A != B? " << (a != b ? "да" : "нет") << "\n";
  std::cout << "норма A = " << a.norm() << ", норма B = " << b.norm() << "\n";
  std::cout << "A < B? " << (a < b ? "да" : "нет") << ", A >= B? " << (a >= b ? "да" : "нет") << "\n\n";

  // 6. Инкремент и декремент (прибавление / вычитание единичной матрицы)
  std::cout << "6. ++ и --:\n";
  SparseMatrix d = a;
  ++d;
  std::cout << "++D:\n" << d;
  SparseMatrix old = d++;
  std::cout << "D++ вернул (старое значение):\n" << old << "D после D++:\n" << d;
  --d;
  --d;
  std::cout << "D после двух --:\n" << d << "\n";

  // 7. Функтор, индексация, преобразования типов
  std::cout << "7. (), [], преобразования:\n";
  std::cout << "A(1, 2) = " << a(1, 2) << ", A(0, 1) = " << a(0, 1) << "\n";
  std::cout << "A[0] (первый хранимый элемент) = " << a[0] << "\n";
  std::cout << "double(A) = " << static_cast<double>(a) << "\n";
  SparseMatrix zero(3, 3);
  std::cout << "Нулевая матрица содержит элементы? " << (static_cast<bool>(zero) ? "да" : "нет") << "\n\n";

  // 8. Ввод матрицы из потока (формат: строки столбцы k, затем k троек)
  std::cout << "8. Ввод матрицы:\n";
  std::istringstream in("2 2 2   0 0 5   1 1 7");
  SparseMatrix e;
  in >> e;
  std::cout << "Прочитана матрица:\n" << e << "\n";

  // 9. Глубокое копирование
  std::cout << "9. Глубокое копирование:\n";
  SparseMatrix f = a;
  f.set(0, 0, 100);
  std::cout << "A(0,0) = " << a(0, 0) << ", F(0,0) = " << f(0, 0) << "\n\n";

  // 10. Исключения
  std::cout << "10. Исключения:\n";
  try
  {
    SparseMatrix bad(-1, 2);
  }
  catch (const std::invalid_argument& ex)
  {
    std::cout << "Ошибка: " << ex.what() << "\n";
  }

  try
  {
    a(5, 5);
  }
  catch (const std::out_of_range& ex)
  {
    std::cout << "Ошибка: " << ex.what() << "\n";
  }

  try
  {
    SparseMatrix s(2, 2);
    SparseMatrix r = a + s;
  }
  catch (const std::invalid_argument& ex)
  {
    std::cout << "Ошибка: " << ex.what() << "\n";
  }

  try
  {
    SparseMatrix s(2, 3);
    SparseMatrix r = s * s;
  }
  catch (const std::invalid_argument& ex)
  {
    std::cout << "Ошибка: " << ex.what() << "\n";
  }

  try
  {
    SparseMatrix r = a / 0;
  }
  catch (const std::invalid_argument& ex)
  {
    std::cout << "Ошибка: " << ex.what() << "\n";
  }

  try
  {
    SparseMatrix s(2, 3);
    ++s;
  }
  catch (const std::invalid_argument& ex)
  {
    std::cout << "Ошибка: " << ex.what() << "\n";
  }

  try
  {
    std::cout << a[100];
  }
  catch (const std::out_of_range& ex)
  {
    std::cout << "Ошибка: " << ex.what() << "\n";
  }

  return 0;
}

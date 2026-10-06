#include <utility> // std::move
#include "IntVector.h"

int main()
{
  std::cout << "--- Демонстрация IntVector ---\n" << std::endl;

  // 1. Конструкторы
  std::cout << "1. Конструкторы:\n";
  IntVector a; // по умолчанию
  IntVector b(3); // размер 3, нули
  IntVector c(4, 7); // размер 4, значение 7
  a.print();
  b.print();
  c.print();

  // 2. pushBack с автоматическим расширением
  std::cout << "\n2. pushBack (расширение памяти):\n";
  for (int i = 1; i <= 12; ++i)
    a.pushBack(i);
  a.print();
  std::cout << "Размер: " << a.getSize() << ", ёмкость: " << a.getCapacity() << "\n";

  // 3. insert, remove, find, popBack, resize
  std::cout << "\n3. insert / remove / find / popBack / resize:\n";
  b.insert(1, 5);
  b.print();
  b.remove(0);
  b.print();
  std::cout << "find(5) = " << b.find(5) << ", find(100) = " << b.find(100) << "\n";
  b.popBack();
  b.print();
  b.resize(4);
  b.print();

  // 4. Вариант 3: оператор +
  std::cout << "\n4. Оператор + (конкатенация):\n";
  IntVector x(2, 1), y(3, 2);
  IntVector z = x + y;
  x.print();
  y.print();
  z.print();

  // 5. Вариант 3: оператор *
  std::cout << "\n5. Оператор * (умножение на число):\n";
  IntVector m = z * 3; // вектор * число
  IntVector n = 2 * z; // число * вектор
  m.print();
  n.print();
  z.print(); // исходный вектор не изменился

  // 6. Правило трёх: глубокое копирование
  std::cout << "\n6. Копирование и присваивание:\n";
  IntVector copy1 = z; // конструктор копирования
  IntVector copy2;
  copy2 = z; // оператор присваивания
  copy2 = copy2; // самоприсваивание безопасно
  z[0] = 999;
  z.print();
  copy1.print();
  copy2.print();

  // 7. Правило пяти: перемещение
  std::cout << "\n7. Перемещение:\n";
  IntVector p(3, 5);
  IntVector q = std::move(p); // конструктор перемещения
  IntVector r;
  r = std::move(q); // оператор присваивания перемещением
  r.print();
  std::cout << "Размер p после перемещения: " << p.getSize() << "\n";

  // 8. Исключения
  std::cout << "\n8. Исключения:\n";
  try
  {
    IntVector bad(-1);
  }
  catch (const std::invalid_argument& e)
  {
    std::cout << "Ошибка: " << e.what() << "\n";
  }

  try
  {
    std::cout << r[100];
  }
  catch (const std::out_of_range& e)
  {
    std::cout << "Ошибка: " << e.what() << "\n";
  }

  try
  {
    IntVector e;
    e.popBack();
  }
  catch (const std::underflow_error& e)
  {
    std::cout << "Ошибка: " << e.what() << "\n";
  }

  return 0;
}

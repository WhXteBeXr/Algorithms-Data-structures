#include <iostream>
#include "University.h"
#include "Student.h"

int main() {
  std::cout << "Студентов всего: " << Student::getTotalCount() << "\n\n";

  // static + создан до getInstance(), поэтому уничтожится ПОСЛЕ университета
  static Student persistent("Дмитрий", "Волков", "105");

  University& uni = University::getInstance();
  University& uni2 = University::getInstance();
  uni.setName("БГУИР");
  std::cout << "Одиночка: uni и uni2 - один объект? "
            << (&uni == &uni2 ? "да" : "нет") << "\n\n";

  Student s1("Иван", "Иванов", "101");
  Student s2("Мария", "Петрова", "102");
  Student s3("Пётр", "Сидоров", "101");

  std::cout << "\n--- Зачисление ---\n";
  uni.addStudent(&persistent);
  uni.addStudent(&s1);
  uni.addStudent(&s2);
  uni.addStudent(&s3);
  uni.print();

  std::cout << "\n--- Граничные случаи ---\n";
  uni.addStudent(nullptr);
  uni.addStudent(&s1);
  Student stranger("Анна", "Николаевна", "103");
  uni.removeStudent(&stranger);

  std::cout << "\n--- Отчисление ---\n";
  uni.removeStudent(&s2);
  uni.print();
  std::cout << "Объектов Student: " << Student::getTotalCount()
            << " (s2 отчислен, но продолжает существовать)\n";

  std::cout << "\n--- Студент во вложенной области видимости ---\n";
  {
    Student temp("Олег", "Кузнецов", "104");
    uni.addStudent(&temp);
    std::cout << "Объектов Student: " << Student::getTotalCount() << "\n";
    // Иначе в векторе останется висячий указатель
    uni.removeStudent(&temp);
  }
  std::cout << "Объектов Student: " << Student::getTotalCount() << "\n";

  std::cout << "\n--- Отчисление остальных ---\n";
  uni.removeStudent(&s1);
  uni.removeStudent(&s3);
  uni.print(); // остался только persistent

  std::cout << "\n--- Конец main ---\n";
  return 0;
}
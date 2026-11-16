#pragma once

#include <cstddef>
#include <iosfwd>
#include <memory>
#include <string>
#include <vector>

// Для указателей достаточно имени класса. Полное определение подключается в .cpp.
class Employee;

class Company
{
public:
  Company(const Company&) = delete;
  Company& operator=(const Company&) = delete;
  Company(Company&&) = delete;
  Company& operator=(Company&&) = delete;

  // Все обращения к компании проходят через общую точку доступа.
  static Company* getInstance();
  // После вызова все ранее полученные указатели на Company недействительны.
  static void destroyInstance();

  // Невладеющие указатели: перед уничтожением сотрудника его нужно исключить
  // из компании. nullptr и повторное добавление одного адреса отклоняются.
  bool addEmployee(Employee* employee);
  bool removeEmployee(Employee* employee);
  std::size_t getStaffCount() const;
  void print(std::ostream& out) const;

private:
  explicit Company(const std::string& name);
  ~Company();
  // Разрешаем стандартному удалителю unique_ptr вызвать закрытый деструктор.
  friend struct std::default_delete<Company>;

  // unique_ptr освобождает одиночку и при завершении программы, если явное
  // уничтожение пропущено. Закрытый деструктор запрещает внешний delete.
  static std::unique_ptr<Company> instance;
  std::string name;
  std::vector<Employee*> staff; // Адреса внешних объектов, владение не передаётся.
};

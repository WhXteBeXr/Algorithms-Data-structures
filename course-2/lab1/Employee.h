#pragma once

#include <cstddef>
#include <iosfwd>
#include <string>

class Employee
{
public:
  Employee(const std::string& name, const std::string& position, double salary);
  ~Employee();

  // Компания хранит адрес сотрудника: копирование и перемещение запрещены,
  // чтобы идентичность объекта и его адрес оставались неизменными.
  Employee(const Employee&) = delete;
  Employee& operator=(const Employee&) = delete;
  Employee(Employee&&) = delete;
  Employee& operator=(Employee&&) = delete;

  // Статические методы вызываются через имя класса, отдельный объект не нужен.
  static std::size_t getCount();
  static double getAverageSalary();

  const std::string& getName() const;
  const std::string& getPosition() const;
  double getSalary() const;

  // Отрицательные и неконечные значения вызывают std::invalid_argument.
  // При ошибке оклад и общая статистика остаются прежними.
  void setSalary(double newSalary);
  // Поток передаётся снаружи, поэтому вывод возможен и в консоль, и в файл.
  void print(std::ostream& out) const;

private:
  std::string name;
  std::string position;
  double salary;

  // Статистика всех живых Employee, включая не добавленных в Company.
  static std::size_t count;
  static long double totalSalary;
};

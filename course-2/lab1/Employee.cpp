#include "Employee.h"

#include <cmath>
#include <ostream>
#include <stdexcept>

namespace
{
  // Функция видна только в этом файле. Единое правило для конструктора и setter.
  void validateSalary(double salary)
  {
    // isfinite отбрасывает NaN и бесконечность, сравнение отбрасывает отрицательные числа.
    if (!std::isfinite(salary) || salary < 0)
    {
      throw std::invalid_argument("Оклад должен быть конечным неотрицательным числом");
    }
  }
}

// Определения общих полей размещаются один раз в .cpp, а не в каждом объекте.
std::size_t Employee::count = 0;
long double Employee::totalSalary = 0;

Employee::Employee(const std::string& name, const std::string& position, double salary) :
    name(name), position(position), salary(salary)
{
  // Проверяем до изменения статистики: неудачное создание не считается объектом.
  validateSalary(salary);
  ++count;
  totalSalary += salary;
}

Employee::~Employee()
{
  // Объект перестал существовать, поэтому исключаем его из общей статистики.
  --count;
  totalSalary -= salary;
  if (count == 0)
  {
    totalSalary = 0; // Убираем остаток округления перед следующей группой объектов.
  }
}

std::size_t Employee::getCount()
{
  return count;
}

double Employee::getAverageSalary()
{
  // Для пустой системы возвращаем ноль, чтобы не делить на ноль.
  return count == 0 ? 0.0 : static_cast<double>(totalSalary / count);
}

const std::string& Employee::getName() const
{
  return name;
}

const std::string& Employee::getPosition() const
{
  return position;
}

double Employee::getSalary() const
{
  return salary;
}

void Employee::setSalary(double newSalary)
{
  validateSalary(newSalary);
  // Заменяем вклад сотрудника в общей сумме, число сотрудников не меняется.
  totalSalary = totalSalary - salary + newSalary;
  salary = newSalary;
}

void Employee::print(std::ostream& out) const
{
  out << "  " << name << " (" << position << "): " << salary << " руб.\n";
}

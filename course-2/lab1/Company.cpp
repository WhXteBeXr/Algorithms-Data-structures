#include "Company.h"
#include "Employee.h"

#include <algorithm>
#include <ostream>

std::unique_ptr<Company> Company::instance;

Company::Company(const std::string& name) : name(name) {}

// Уничтожается только вектор указателей. Сотрудники принадлежат вызывающему коду.
Company::~Company() = default;

Company* Company::getInstance()
{
  // Ленивое создание: компания появляется только при первом обращении.
  if (!instance)
  {
    instance.reset(new Company("ТехноСофт"));
  }
  return instance.get(); // Возвращаем адрес без передачи владения компанией.
}

void Company::destroyInstance()
{
  // reset удаляет компанию и обнуляет unique_ptr. Для пустого указателя это безопасно.
  instance.reset();
}

bool Company::addEmployee(Employee* employee)
{
  // find сравнивает адреса. Один и тот же объект нельзя добавить дважды.
  // Оператор || прекращает проверку сразу, если передан nullptr.
  if (employee == nullptr || std::find(staff.begin(), staff.end(), employee) != staff.end())
  {
    return false;
  }
  staff.push_back(employee); // Сохраняем адрес, новый Employee не создаётся.
  return true;
}

bool Company::removeEmployee(Employee* employee)
{
  if (employee == nullptr)
  {
    return false;
  }
  // end() означает, что такого адреса нет в списке.
  const auto it = std::find(staff.begin(), staff.end(), employee);
  if (it == staff.end())
  {
    return false;
  }
  staff.erase(it); // Удаляется запись списка, сотрудник продолжает жить.
  return true;
}

std::size_t Company::getStaffCount() const
{
  // Размер компании может отличаться от общего числа живых Employee.
  return staff.size();
}

void Company::print(std::ostream& out) const
{
  out << "Сотрудники компании \"" << name << "\":\n";
  if (staff.empty())
  {
    out << "  (пусто)\n";
  }
  // Разыменование допустимо, пока вызывающий код сохраняет сотрудников живыми.
  for (const Employee* employee : staff)
  {
    employee->print(out);
  }
}

#include "Company.h"
#include "Employee.h"

#include <iomanip>
#include <iostream>
#include <limits>
#include <stdexcept>

namespace
{
  void printStatistics()
  {
    // Данные принадлежат классу Employee и не зависят от существования Company.
    std::cout << "Живых объектов Employee: " << Employee::getCount()
              << ", средняя зарплата: " << Employee::getAverageSalary() << " руб.\n";
  }

  void demonstrateCompany()
  {
    // Локальные сотрудники существуют независимо от одиночки.
    // При выходе из функции их деструкторы автоматически обновят статистику.
    Employee developer("Иванов Иван", "Программист", 80000);
    Employee tester("Петров Пётр", "Тестировщик", 60000);

    // Совпадение адресов показывает, что второй вызов не создаёт новую компанию.
    Company* company = Company::getInstance();
    std::cout << "Два вызова getInstance() вернули один объект: "
              << (company == Company::getInstance()) << '\n';
    std::cout << "Добавление Иванова: " << company->addEmployee(&developer) << '\n';
    std::cout << "Добавление Петрова: " << company->addEmployee(&tester) << '\n';
    std::cout << "Повторное добавление Петрова: " << company->addEmployee(&tester) << '\n';
    std::cout << "Добавление nullptr: " << company->addEmployee(nullptr) << '\n';
    company->print(std::cout);
    printStatistics();

    tester.setSalary(70000);
    std::cout << "\nПосле повышения зарплаты Петрова:\n";
    printStatistics();

    // Ошибка оклада перехватывается, чтобы демонстрация продолжалась.
    // Проверка предшествует изменению полей, поэтому среднее остаётся прежним.
    for (double invalidSalary : {-1.0, std::numeric_limits<double>::quiet_NaN(),
                                 std::numeric_limits<double>::infinity()})
    {
      try
      {
        tester.setSalary(invalidSalary);
      }
      catch (const std::invalid_argument& error)
      {
        std::cout << "Некорректная зарплата отклонена: " << error.what() << '\n';
      }
    }
    printStatistics();

    std::cout << "\nВременный сотрудник:\n";
    {
      Employee designer("Сидорова Мария", "Дизайнер", 65000);
      company->addEmployee(&designer);
      printStatistics();
      // Агрегация не продлевает жизнь designer: сначала убираем его адрес.
      std::cout << "Исключение Сидоровой: " << company->removeEmployee(&designer) << '\n';
      std::cout << "Повторное исключение: " << company->removeEmployee(&designer) << '\n';
      std::cout << "Исключение не уничтожило сотрудника: ";
      printStatistics();
    }
    std::cout << "После выхода сотрудника из области видимости:\n";
    printStatistics();
    std::cout << "Исключение nullptr: " << company->removeEmployee(nullptr) << '\n';

    std::cout << "\nЗакрываем компанию. Сотрудники продолжают существовать:\n";
    Company::destroyInstance();
    company = nullptr; // Старый адрес больше не указывает на существующую компанию.
    developer.print(std::cout);
    tester.print(std::cout);
    printStatistics();

    // У нового экземпляра пустой список, старые сотрудники ещё существуют.
    Company* reopened = Company::getInstance();
    std::cout << "\nВ повторно созданной компании сотрудников: " << reopened->getStaffCount() << '\n';
    reopened->print(std::cout);
    Company::destroyInstance();
    Company::destroyInstance(); // Повторное закрытие пустого одиночки безопасно.
  }
}

int main()
{
  // boolalpha печатает true/false, fixed и setprecision задают два знака после точки.
  std::cout << std::boolalpha << std::fixed << std::setprecision(2);
  std::cout << "До создания сотрудников:\n";
  printStatistics();
  demonstrateCompany();
  // Функция завершилась, все её локальные сотрудники уже уничтожены.
  std::cout << "\nПосле уничтожения всех сотрудников:\n";
  printStatistics();
}

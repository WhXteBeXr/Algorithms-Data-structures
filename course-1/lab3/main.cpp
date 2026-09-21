#include <iostream>
#include "banknote.h"
#include "date.h"

using namespace std;

void taskOne()
{
  // Создание экземпляров класса
  cout << "\n--- Объявление и инициализация экземпляров класса ---" << endl;
  Banknote firstBanknote;
  Banknote secondBanknote(1000, 50);
  Banknote thirdBanknote = secondBanknote;

  // Вызов сеттеров
  cout << "\n--- Замена на значений на новые у первой банкноты ---" << endl;
  firstBanknote.setAmount(-1);
  firstBanknote.setAmount(1000);
  firstBanknote.setBillDenominator(-1);
  firstBanknote.setBillDenominator(5000);

  cout << "\n--- Вывод значений всех банкнот ---" << endl;
  cout << endl << "Первая:";
  firstBanknote.printBill();
  cout << "Общий номинал " << firstBanknote.printTotalAmount() << endl;

  cout << endl << "Вторая:";
  secondBanknote.printBill();
  cout << "Общий номинал " << secondBanknote.printTotalAmount() << endl;

  cout << endl << "Третья:";
  thirdBanknote.printBill();
  cout << "Общий номинал " << thirdBanknote.printTotalAmount() << endl;
}

void taskTwo()
{
  cout << "\n--- Объявление и инициализация экземпляров класса ---" << endl;
  Date firstDate;
  Date secondDate(10, "Января", 2077);
  Date thirdDate = secondDate;
  cout << "Количество созданных экземпляров: " << Date::getObjectsCount() << endl;

  cout << "\n--- Замена значений на новые ---" << endl;
  firstDate.setDay(100);
  firstDate.setDay(1);
  firstDate.setMonth("Март");
  firstDate.setYear(1001);
  firstDate.setYear(2006);
  firstDate.addFiveDays();
  firstDate.isThisALeapYear();

  cout << "\n--- Вывод всех дат ---" << endl;
  cout << "Первая дата (Отдельные вызовы get методов): " << firstDate.getDay() << " / " << firstDate.getMonth() << " / "
       << firstDate.getYear() << endl;
  firstDate.printDate();
  secondDate.printDate();
  thirdDate.printDate();
}

int main()
{
  int userChoice;
  cout << "Выберите задание:"
          "\n1 - Банкноты"
          "\n2 - Даты"
       << endl;
  cin >> userChoice;
  switch (userChoice)
  {
  case 1:
    taskOne();
    break;
  case 2:
    taskTwo();
    cout << "\nКоличество созданных экземпляров: " << Date::getObjectsCount() << endl;
    break;
  default:
    cout << "Выбранного задания нет" << endl;
  }

  return 0;
}

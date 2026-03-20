#include <iostream>
#include <string>
#include "date.h"

using namespace std;

unsigned int Date::objectsCount = 0;

Date::Date() : day(1), month("January"), year(2026)
{
  cout << "Инициализация стандартной даты" << endl;
  objectsCount++;
}

Date::Date(const int day, const string month, const int year) : day(day), month(month), year(year)
{
  cout << "Инициализация даты с помощью параметров" << endl;
  objectsCount++;
}

Date::Date(const Date& date) : day(date.day), month(date.month), year(date.year)
{
  cout << "Инициализация даты копированием" << endl;
  objectsCount++;
}

Date::~Date()
{
  cout << "\n--- Удаление даты ---"
          "\n" << day << " / " << month << " / " << year << endl;
  objectsCount--;
}

int Date::getDay() const
{
  return day;
}

string Date::getMonth() const
{
  return month;
}

int Date::getYear() const
{
  return year;
}

unsigned int Date::getObjectsCount() {
  return objectsCount;
}

void Date::setDay(const int day)
{
  if (1 <= day and day <= 31)
  {
    cout << "Обновление дня" << endl;
    this->day = day;
  }
  else
    cout << "Недопустимый формат для дня" << endl;
}

void Date::setMonth(string month)
{
  {
    cout << "Обновление месяца" << endl;
    this->month = month;
  }
}

void Date::setYear(const int year)
{
  if (year >= 1900)
  {
    cout << "Обновление года" << endl;
    this->year = year;
  }
  else
    cout << "Недопустимый формат для года" << endl;
}

void Date::setDate(int day, string month, int year)
{
  if ((1 <= day and day <= 31) and (year >= 1900))
  {
    this->day = day;
    this->month = month;
    this->year = year;
  }
  else
    cout << "Недопустимый формат даты" << endl;
}

void Date::printDay() const
{
  cout << "День: " << day << endl;
}

void Date::printMonth() const
{
  cout << "Месяц: " << month << endl;
}

void Date::printYear() const
{
  cout << "Год: " << year << endl;
}

void Date::printDate() const
{
  cout << "Дата: " << day << " / " << month << " / " << year << endl;
}

void Date::isThisALeapYear() const
{
  if ((year % 4 == 0) and (year % 100 != 0))
    cout << year << " год является високосным" << endl;
  else
    cout << year << " не является високосным" << endl;
}

void Date::addFiveDays()
{
  cout << "Текущий день: " << day <<  endl;
  if (day <= 26) // Нет проверки для месяцев с меньшим колчиством дней
  {
    day += 5;
    cout << "День после изменения: " << day << endl;
  }

  else
    cout << "День не может быть больше 31";
}

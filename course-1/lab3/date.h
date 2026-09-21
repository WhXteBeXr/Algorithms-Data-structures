#pragma once
#include <string>

using std::string;

class Date
{
  private:
  int day;
  string month;
  int year;
  static unsigned int objectsCount;

  public:
  Date();
  Date(int day, const string month, int year);
  Date(const Date& date);

  ~Date();

  int getDay() const;
  string getMonth() const;
  int getYear() const;
  static unsigned int getObjectsCount();

  void setDay(int day);
  void setMonth(string month);
  void setYear(int year);
  void setDate(int day, string month, int year);

  void printDay() const;
  void printMonth() const;
  void printYear() const;
  void printDate() const;
  void isThisALeapYear() const;
  void addFiveDays();
};

#pragma once
#include <string>

using namespace std;

class Date
{
  private:
  int day;
  string month;
  int year;

  public:
  Date();
  Date(int day, const string month, int year);
  Date(const Date& date);

  ~Date();

  int getDay() const;
  string getMonth() const;
  int getYear() const;

  void setDay(int day);
  void setMonth(string month);
  void setYear(int year);
  void setDate(int day, string month, int year);

  void printDay() const;
  void printMonth() const;
  void printYear() const;
  void printDate() const;
};

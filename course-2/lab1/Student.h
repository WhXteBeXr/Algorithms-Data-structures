#pragma once
#include <string>

class Student {
private:
  std::string name;
  std::string surname;
  std::string group;

  static int totalCount;

public:
  Student(const std::string& name, const std::string& surname, const std::string& group);
  ~Student();

  // Копирование запрещено, иначе счётчик объектов станет неверным
  Student(const Student&) = delete;
  Student& operator=(const Student&) = delete;

  const std::string& getName() const;
  const std::string& getSurname() const;
  const std::string& getGroup() const;

  void print() const;

  static int getTotalCount();
};
#include "Student.h"
#include <iostream>

int Student::totalCount = 0;

Student::Student(const std::string& name, const std::string& surname, const std::string& group)
    : name(name), surname(surname), group(group) {
  ++totalCount;
  std::cout << "Создан студент: " << surname << " " << name << std::endl;
}

Student::~Student() {
  --totalCount;
  std::cout << "Удалён студент: " << surname << " " << name << std::endl;
}

const std::string& Student::getName() const { return name; }
const std::string& Student::getSurname() const { return surname; }
const std::string& Student::getGroup() const { return group; }

void Student::print() const {
  std::cout << "  " << surname << " " << name << ", группа " << group << std::endl;
}

int Student::getTotalCount() {
  return totalCount;
}
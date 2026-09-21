#include "University.h"
#include "Student.h"
#include <algorithm>
#include <iostream>

University::University() : name("Университет") {
  std::cout << "Создан университет" << std::endl;
}

University& University::getInstance() {
  // Локальная статическая переменная создаётся при первом вызове,
  // уничтожается автоматически при завершении программы
  static University instance;
  return instance;
}

University::~University() {
  // Студентов не трогаем: они могут быть уже уничтожены
  std::cout << "Закрыт университет, студентов в списке: " << students.size() << std::endl;
}

void University::setName(const std::string& newName) {
  name = newName;
}

bool University::addStudent(Student* student) {
  if (student == nullptr) {
    std::cout << "Ошибка: передан nullptr" << std::endl;
    return false;
  }
  if (std::find(students.begin(), students.end(), student) != students.end()) {
    std::cout << "Ошибка: студент " << student->getSurname() << " уже зачислен" << std::endl;
    return false;
  }
  students.push_back(student);
  std::cout << "Зачислен: " << student->getSurname() << " " << student->getName() << std::endl;
  return true;
}

bool University::removeStudent(Student* student) {
  auto it = std::find(students.begin(), students.end(), student);
  if (it == students.end()) {
    std::cout << "Ошибка: студент не найден в университете" << std::endl;
    return false;
  }
  std::cout << "Отчислен: " << (*it)->getSurname() << " " << (*it)->getName() << std::endl;
  students.erase(it); // сам объект Student не удаляется
  return true;
}

std::size_t University::getStudentCount() const {
  return students.size();
}

void University::print() const {
  std::cout << name << ", студентов: " << students.size() << std::endl;
  if (students.empty()) {
    std::cout << "  (список пуст)" << std::endl;
    return;
  }
  for (const Student* s : students) {
    s->print();
  }
}
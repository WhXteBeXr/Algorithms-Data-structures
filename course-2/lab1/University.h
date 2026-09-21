#pragma once
#include <string>
#include <vector>

class Student;

class University {
private:
  std::string name;
  std::vector<Student*> students; // агрегация: университет не владеет студентами

  University();

public:
  University(const University&) = delete;
  University& operator=(const University&) = delete;
  ~University();

  static University& getInstance();

  void setName(const std::string& newName);

  bool addStudent(Student* student);
  bool removeStudent(Student* student);

  std::size_t getStudentCount() const;
  void print() const;
};
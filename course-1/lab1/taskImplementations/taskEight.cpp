#include <array>
#include <fstream>
#include <iostream>
#include <vector>

#include "../labFunctions.h"

using namespace std;

struct Student
{
  char name[100];
  int group;
  array<int, 3> grades;
  double scholarShip;
};

vector<Student> readAndFilterStudents(const string& filePath)
{
  ifstream allStudents(filePath, ios::binary);
  if (!allStudents.is_open())
  {
    cerr << "Ошибка открытия файла для чтения" << endl;
    exit(1);
  }

  Student currentStudent{};
  vector<Student> goodGradedStudents;

  // Передаем адрес на структуру currentStudent для передачи данных в нее и читаем информацию длиной в sizeof(Student)
  while (allStudents.read(reinterpret_cast<char*>(&currentStudent), sizeof(Student)))
  {
    bool goodGrades = true;
    for (const int& grade : currentStudent.grades)
      if (grade < 30)
      {
        goodGrades = false;
        break;
      }

    if (goodGrades)
      goodGradedStudents.push_back(currentStudent);
  }
  return goodGradedStudents;
}

void printStudents(const vector<Student>& students)
{
  // Сразу распаковываем в отдельные переменные каждую сущность вектора
  for (const auto& [name, group, grades, scholarShip] : students)
  {
    cout << "ФИО: " << name << " ";
    cout << "Группа студента: " << group << " ";
    cout << "Оценки студента: ";
    for (const int& grade : grades)
      cout << grade << " ";
    cout << "Стипендия: " << scholarShip << endl;
  }
  cout << endl;
}

void writeStudents(const string& filePath, const vector<Student>& students)
{
  // Читаем в бинарном представлении
  ofstream studentsFile(filePath, ios::binary);
  if (!studentsFile.is_open())
  {
    cerr << "Ошибка открытия файла для записи" << endl;
    exit(1);
  }

  // Записываем в файл структуру, которая была представлена как набор байтов с помощью reinterpret_cast,
  // размером в sizeof от структуры
  for (const Student& student : students)
    studentsFile.write(reinterpret_cast<const char*>(&student), sizeof(Student));

  studentsFile.close();
}

void taskEight()
{
  const vector<Student> students = {{"Иванов И. И.", 5, {80, 21, 45}, 1200.52},
                                    {"НеИванов НеИ. НеИ.", 12, {100, 100, 99}, 2511.12},
                                    {"ПочтиИванов ПочтиИ. ПочтиИ.", 12, {61, 58, 74}, 1200.53},
                                    {"ВовсеНеИванов ВовсеНеИ. ВовсеНеИ.", 12, {32, 40, 51}, 1001.13},
                                    {"СовсемНеИванов СовсемНеИ. СовсемНеИ.", 1, {20, 19, 100}, 1001.13}};
  writeStudents("../lab1/testFiles/tEight/allStudents.bin", students);
  const vector<Student> goodGradedStudents = readAndFilterStudents("../lab1/testFiles/tEight/allStudents.bin");
  writeStudents("../lab1/testFiles/tEight/filteredStudents.bin", goodGradedStudents);

  cout << "Все студенты:" << endl;
  printStudents(students);
  cout << "Студенты с хорошими оценками:" << endl;
  printStudents(goodGradedStudents);
}

#include <array>
#include <fstream>
#include <iostream>
#include <vector>

#include "../labFunctions.h"

using namespace std;

/* Представленные задания оформить в виде бинарного файла.
 *
 * Создать бинарный файл, компонентами которого является структура, содержащая следующие поля: − фамилия и инициалы
 * студентов; − номер группы; − успеваемость (массив из трех дисциплин); − размер стипендии.
 */

struct Student
{
  char name[100]{};
  unsigned int group{};
  array<unsigned int, 3> disciplines{};
  double scholarship{};
};

void writeStudents(const vector<Student>& students, const string& filePath)
{
  ofstream studentsFile(filePath, ios::binary);
  if (!studentsFile.is_open())
  {
    cerr << "Ошибка открытия файла для записи" << endl;
    exit(1);
  }

  for (const Student& student : students)
    studentsFile.write(reinterpret_cast<const char*>(&student), sizeof(Student));
  studentsFile.close();
}

vector<Student> readStudents(const string& filePath)
{
  ifstream studentsFile(filePath, ios::binary);
  if (!studentsFile.is_open())
  {
    cerr << "Ошибка открытия файла для чтения" << endl;
    exit(1);
  }

  Student student;
  vector<Student> allStudents;

  while (studentsFile.read(reinterpret_cast<char*>(&student), sizeof(Student)))
    allStudents.push_back(student);
  studentsFile.close();

  return allStudents;
}

void printStudents(const string& filePath)
{
  const vector<Student> students = readStudents(filePath);

  for (const Student& student : students)
  {
    cout << "ФИО: " << student.name << " ";
    cout << "Группа: " << student.group << " ";
    cout << "Оценки по дисциплинам: ";

    for (const auto& grade : student.disciplines)
      cout << grade << " ";

    cout << "Стипендия: " << student.scholarship << endl;
  }
}

void increaseScholarship(const string& filePath)
{
  vector<Student> students = readStudents(filePath);
  for (Student& student : students)
  {
    bool gradesAllGood = true;
    for (const unsigned int& grade : student.disciplines)
      if (grade <= 2)
      {
        gradesAllGood = false;
        break;
      }
    if (gradesAllGood)
      student.scholarship *= 1.3;
  }
  writeStudents(students, filePath);
}

void taskTen()
{
  const vector<Student> students = {{"Иванов И. И.", 15, {5, 4, 5}, 1500.12},
                                    {"НеИванов НеИ. НеИ.", 1, {3, 2, 5}, 1500.13},
                                    {"ПочтиИванов ПочтиИ. ПочтиИ.", 14, {4, 3, 3}, 1500.13}};

  const string filePath = "../lab1/testFiles/tTen/students.bin";

  cout << "Путь к файлу: " << filePath << endl;

  cout << "\n--- Запись структур в файл ---" << endl;
  writeStudents(students, filePath);

  cout << "\n--- Исходные данные студентов ---" << endl;
  printStudents(filePath);

  cout << "\n--- Увеличение стипендии студентам с хорошими оценками ---" << endl;
  increaseScholarship(filePath);

  cout << "\n--- Вывод измененной информации о студентах ---" << endl;
  printStudents(filePath);
}

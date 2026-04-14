#include <iostream>

#include "./tasksClasses/IntVector.h"

using namespace std;

void vectorClassTask()
{
  cout << "\n--- Объявление и инициализация экзепляра класса ---" << endl;
  IntVector firstCustomVector{};
  IntVector secondCustomVector{3, 13};

  cout << "\n--- Вызов сеттеров ---" << endl;
  firstCustomVector.setVectorElement(7);
  firstCustomVector.setSize(5);
  secondCustomVector.setVectorElement(66);
  secondCustomVector.setSize(13);

  cout << "\n--- Вызов геттеров ---" << endl;
  cout << "Размер первого вектора: " << firstCustomVector.getSize() << endl;
  cout << "Размер второго вектора: " << secondCustomVector.getSize() << endl;

  cout << "\n--- Вызов методов классов ---" << endl;
  cout << "Первый исходный вектор: ";
  firstCustomVector.print();
  cout << "Второй исходный вектор: ";
  secondCustomVector.print();
  cout << endl;

  firstCustomVector.popBack();
  secondCustomVector.popBack();
  cout << "Длина первого вектора после удаления последнего элемента: " << firstCustomVector.getSize() << endl;
  cout << "Длина второго вектора после удаления последнего элемента: " << secondCustomVector.getSize() << endl;
  cout << endl;

  firstCustomVector.pushBack(77);
  secondCustomVector.pushBack(88);
  cout << "Первый вектор после добавления элемента: ";
  firstCustomVector.print();
  cout << "Второй вектор после добавления элемента: ";
  secondCustomVector.print();
  cout << endl;

  cout << "Резервируем пространство" << endl;
  firstCustomVector.vectorReserve(15);
  secondCustomVector.vectorReserve(55);
  cout << "Уменьшаем память векторов \"shrink to fit\"" << endl;
  firstCustomVector.vectorShrink();
  secondCustomVector.vectorShrink();

  cout << "\n--- Работа деконструкторов ---" << endl;
}

int main()
{
  unsigned int userChoice;
  cout << "Выберите задание:"
          "\n1 - "
       << endl;
  cin >> userChoice;

  switch (userChoice)
  {
  case 1:
    vectorClassTask();
    break;
  case 2:
    break;
  case 3:
    break;
  default:
    cout << "Выбранного задания нет в списке" << endl;
  }
  return 0;
}

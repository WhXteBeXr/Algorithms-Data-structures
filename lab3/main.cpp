#include <iostream>
#include "banknote.h"

using namespace std;

// Реализация конструкторов
Banknote::Banknote() : billDenominator(0), amount(0)
{
  cout << endl << "Создана купюра по умолчанию" << endl;
}

Banknote::Banknote(const int billDenominator, const int amount) : billDenominator(billDenominator), amount(amount)
{
  cout << endl << "Создана купюра с указанием параметров" << endl;
}

Banknote::Banknote(const Banknote& bn) : billDenominator(bn.billDenominator), amount(bn.amount)
{
  cout << endl << "Копирование купюры" << endl;
}

// Реализация деконструктора
Banknote::~Banknote()
{
  cout << endl << "Удаление купюр количеством в " << amount << " и номиналом в " << billDenominator << endl;
}

// Реализация геттеров
int Banknote::getBillDenominator() const {
  return billDenominator;
}

int Banknote::getAmount() const
{
  return amount;
}

// Реализация сеттеров
void Banknote::setBillDenominator(int billDenominator)
{
  if (billDenominator >= 0)
    this->billDenominator = billDenominator;
  else
    cout << endl << "Номинал должен быть не меньше 0" << endl;
}

void Banknote::setAmount(int amount)
{
  if (amount >= 0)
    this->amount = amount;
  else
    cout << endl << "Количество не должно быть меньше 0" << endl;
}

// Реализация методов
int Banknote::printTotalAmount() const {
  return amount * billDenominator;
}

void Banknote::printBill() const
{
  cout << endl << "Количество банкнот: " << amount << " с номиналом " << billDenominator <<endl;
}

int main()
{
  // Создание экземпляров класса
  Banknote firstBanknote;
  Banknote secondBanknote(1000, 50);
  Banknote thirdBanknote = secondBanknote;

  // Вызов сеттеров
  cout << endl << "Замена на значений на новые у первой банкноты" << endl;
  firstBanknote.setAmount(-1);
  firstBanknote.setAmount(1000);
  firstBanknote.setBillDenominator(-1);
  firstBanknote.setBillDenominator(5000);

  cout << endl << "Вывод значений всех банкнот" << endl;
  cout << endl << "Первая:";
  firstBanknote.printBill();
  cout << "Общий номинал " << firstBanknote.printTotalAmount() << endl;

  cout << endl << "Вторая:";
  secondBanknote.printBill();
  cout << "Общий номинал " << secondBanknote.printTotalAmount() << endl;

  cout << endl << "Третья:";
  thirdBanknote.printBill();
  cout << "Общий номинал " << thirdBanknote.printTotalAmount() << endl;

  // Вызов деконструкторов
  firstBanknote.~Banknote();
  secondBanknote.~Banknote();
  thirdBanknote.~Banknote();
}

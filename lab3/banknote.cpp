#include <iostream>
#include "banknote.h"

using namespace std;

// Реализация конструкторов
Banknote::Banknote() : billDenominator(0), amount(0)
{
  cout << "Создана купюра по умолчанию" << endl;
}

Banknote::Banknote(const int billDenominator, const int amount) : billDenominator(billDenominator), amount(amount)
{
  cout << "Создана купюра с указанием параметров" << endl;
}

Banknote::Banknote(const Banknote& bn) : billDenominator(bn.billDenominator), amount(bn.amount)
{
  cout << "Копирование купюры" << endl;
}

// Реализация деконструктора
Banknote::~Banknote()
{
  cout << "\n--- Удаление купюры ---"
          "\nКоличество: " << amount << " Номинал: " << billDenominator << endl;
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
    cout << "Номинал должен быть не меньше 0" << endl;
}

void Banknote::setAmount(int amount)
{
  if (amount >= 0)
    this->amount = amount;
  else
    cout << "Количество не должно быть меньше 0" << endl;
}

// Реализация методов
int Banknote::printTotalAmount() const {
  return amount * billDenominator;
}

void Banknote::printBill() const
{
  cout << "Количество банкнот: " << amount << " с номиналом " << billDenominator <<endl;
}
#pragma once

class Banknote
{
  private:
  int billDenominator;
  int amount;

  public:
  // Конструкторы
  Banknote();
  Banknote(int billDenominator, int amount);
  Banknote(const Banknote& bn);

  // Деконструктор
  ~Banknote();

  // Геттеры
  int getBillDenominator() const;
  int getAmount() const;

  // Сеттеры
  void setBillDenominator(int billDenominator);
  void setAmount(int amount);

  // Методы
  int printTotalAmount() const;
  void printBill() const;
};

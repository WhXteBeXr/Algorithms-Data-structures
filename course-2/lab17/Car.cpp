#include "Car.h"

#include <ostream>
#include <stdexcept>
#include <utility>

Car::Car(std::string brand, int year, std::int64_t price, int power, int detail)
  : Vehicle(std::move(brand), year, price, power), seats(detail)
{
  // Сначала проверена общая часть, теперь проверяем собственное поле наследника.
  if (detail < 1 || detail > 9)
  {
    throw std::out_of_range("Мест: допустимо 1..9");
  }
}

void Car::describe(std::ostream& out) const
{
  out << "мест: " << seats;
}

void Car::save(std::ostream& out) const
{
  saveCommon(out);
  out << static_cast<int>(seats); // Формат собственного поля всегда целочисленный.
}

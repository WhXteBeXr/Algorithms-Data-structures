#include "Truck.h"

#include <ostream>
#include <stdexcept>
#include <utility>

Truck::Truck(std::string brand, int year, std::int64_t price, int power, int detail)
  : Vehicle(std::move(brand), year, price, power), capacity(detail)
{
  // Сначала проверена общая часть, теперь проверяем собственное поле наследника.
  if (detail < 1 || detail > 100000)
  {
    throw std::out_of_range("Грузоподъёмность, кг: допустимо 1..100000");
  }
}

void Truck::describe(std::ostream& out) const
{
  out << "грузоподъёмность, кг: " << capacity;
}

void Truck::save(std::ostream& out) const
{
  saveCommon(out);
  out << static_cast<int>(capacity); // Формат собственного поля всегда целочисленный.
}

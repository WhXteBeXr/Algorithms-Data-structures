#include "Motorcycle.h"

#include <ostream>
#include <stdexcept>
#include <utility>

Motorcycle::Motorcycle(std::string brand, int year, std::int64_t price, int power, int detail)
  : Vehicle(std::move(brand), year, price, power), sidecar(detail != 0)
{
  // Сначала проверена общая часть, теперь проверяем собственное поле наследника.
  if (detail < 0 || detail > 1)
  {
    throw std::out_of_range("Коляска: допустимо 0..1");
  }
}

void Motorcycle::describe(std::ostream& out) const
{
  out << "коляска: " << (sidecar ? "да" : "нет");
}

void Motorcycle::save(std::ostream& out) const
{
  saveCommon(out);
  out << static_cast<int>(sidecar); // Формат собственного поля всегда целочисленный.
}

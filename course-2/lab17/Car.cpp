#include "Car.h"

#include <ostream>
#include <stdexcept>
#include <utility>

// Передаёт общие поля основе Vehicle, detail задаёт число мест.
Car::Car(std::string brand, int year, std::int64_t price, int power, int detail)
  : Vehicle(std::move(brand), year, price, power), seats(detail)
{
  // Сначала проверена общая часть, теперь проверяем собственное поле наследника.
  if (detail < 1 || detail > 9)
  {
    throw std::out_of_range("Мест: допустимо 1..9");
  }
}

// Дописывает собственное поле в общий вывод Vehicle::print, не меняя объект.
void Car::describe(std::ostream& out) const
{
  out << "мест: " << seats;
}

// Сохраняет общий префикс и дополнительное поле, соответствующее тегу этого наследника.
void Car::save(std::ostream& out) const
{
  // Перевод строки добавляет Fleet, чтобы все записи имели одинаковые границы.
  saveCommon(out);
  out << static_cast<int>(seats); // Формат собственного поля всегда целочисленный.
}

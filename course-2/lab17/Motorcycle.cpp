#include "Motorcycle.h"

#include <ostream>
#include <stdexcept>
#include <utility>

// Передаёт общие поля основе Vehicle, detail задаёт наличие коляски.
Motorcycle::Motorcycle(std::string brand, int year, std::int64_t price, int power, int detail)
  : Vehicle(std::move(brand), year, price, power), sidecar(detail != 0)
{
  // Сначала проверена общая часть, теперь проверяем собственное поле наследника.
  // Проверяем исходное число: преобразование в bool само приняло бы любое ненулевое значение.
  if (detail < 0 || detail > 1)
  {
    throw std::out_of_range("Коляска: допустимо 0..1");
  }
}

// Дописывает собственное поле в общий вывод Vehicle::print, не меняя объект.
void Motorcycle::describe(std::ostream& out) const
{
  out << "коляска: " << (sidecar ? "да" : "нет");
}

// Сохраняет общий префикс и дополнительное поле, соответствующее тегу этого наследника.
void Motorcycle::save(std::ostream& out) const
{
  // Перевод строки добавляет Fleet, чтобы все записи имели одинаковые границы.
  saveCommon(out);
  out << static_cast<int>(sidecar); // Формат собственного поля всегда целочисленный.
}

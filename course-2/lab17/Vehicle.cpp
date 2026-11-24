#include "Vehicle.h"
#include "Car.h"
#include "Truck.h"
#include "Motorcycle.h"

#include <iomanip>
#include <istream>
#include <ostream>
#include <stdexcept>
#include <utility>

// Создаёт общие поля транспорта. Строка переходит во владение объекту, цена хранится в целых рублях.
Vehicle::Vehicle(std::string brand, int year, std::int64_t price, int power)
  : brand(std::move(brand)), year(year), price(price), power(power)
{
  // Эти ограничения проверяются независимо от источника: меню или файл.
  if (this->brand.find_first_not_of(" \t\r\n") == std::string::npos ||
      this->brand.find_first_of("\r\n") != std::string::npos)
  {
    throw std::invalid_argument("Марка должна быть непустой однострочной строкой");
  }
  // Фиксированные учебные границы защищают объект и при вводе, и при загрузке файла.
  if (year < 1886 || year > 2100 || price < 0 || price > maxPrice || power < 1 || power > 10000)
  {
    throw std::out_of_range("Недопустимые год, цена или мощность");
  }
}

// Печатает общие параметры и вызывает describe фактического наследника через базовый интерфейс.
void Vehicle::print(std::ostream& out) const
{
  out << type() << ' ' << std::quoted(brand) << ", год " << year
      << ", цена " << price << " руб., мощность " << power << " л.с., ";
  describe(out); // Детали выбираются по реальному типу объекта через виртуальный вызов.
  out << '\n';
}

// Записывает тег и общие поля в том порядке, в котором их разбирает read.
void Vehicle::saveCommon(std::ostream& out) const
{
  // quoted сохраняет пробелы, кавычки и обратные косые черты в марке.
  out << type() << ' ' << std::quoted(brand) << ' ' << year << ' ' << price << ' ' << power << ' ';
}

// Создаёт конкретный тип по тегу, detail трактуется как места, грузоподъёмность или признак коляски.
// Возвращает unique_ptr основы без копирования и срезки производного объекта.
std::unique_ptr<Vehicle> Vehicle::create(std::string_view type, std::string brand,
                                         int year, std::int64_t price, int power, int detail)
{
  if (type == "CAR")
  {
    return std::make_unique<Car>(std::move(brand), year, price, power, detail);
  }
  if (type == "TRUCK")
  {
    return std::make_unique<Truck>(std::move(brand), year, price, power, detail);
  }
  if (type == "MOTORCYCLE")
  {
    return std::make_unique<Motorcycle>(std::move(brand), year, price, power, detail);
  }
  throw std::invalid_argument("Неизвестный тип транспорта");
}

// Читает поля одной текстовой записи и передаёт их фабрике, конструкторы проверяют значения.
std::unique_ptr<Vehicle> Vehicle::read(std::istream& in)
{
  std::string type, brand;
  int year, power, detail;
  std::int64_t price;
  if (!(in >> type >> std::quoted(brand) >> year >> price >> power >> detail))
  {
    throw std::runtime_error("Неполная или неверная запись транспорта");
  }
  // Контроль лишнего остатка строки остаётся Fleet::load, здесь разбираются только ожидаемые поля.
  return create(type, std::move(brand), year, price, power, detail);
}

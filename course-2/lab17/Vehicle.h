#pragma once

#include <cstdint>
#include <iosfwd>
#include <memory>
#include <string>
#include <string_view>

class Vehicle
{
public:
  static constexpr std::int64_t maxPrice = 1'000'000'000'000;
  virtual ~Vehicle() = default;
  virtual std::string_view type() const = 0;
  virtual void save(std::ostream& out) const = 0;
  const std::string& getBrand() const { return brand; }
  int getYear() const { return year; }
  std::int64_t getPrice() const { return price; }
  int getPower() const { return power; }
  void print(std::ostream& out) const;

  // Фабрика используется и меню, и загрузкой, поэтому проверки модели общие.
  static std::unique_ptr<Vehicle> create(std::string_view type, std::string brand,
                                       int year, std::int64_t price, int power, int detail);
  static std::unique_ptr<Vehicle> read(std::istream& in);

protected:
  Vehicle(std::string brand, int year, std::int64_t price, int power);
  virtual void describe(std::ostream& out) const = 0;
  void saveCommon(std::ostream& out) const;

private:
  std::string brand;
  int year;
  std::int64_t price; // Целые рубли исключают погрешность сравнения денежных сумм.
  int power;
};

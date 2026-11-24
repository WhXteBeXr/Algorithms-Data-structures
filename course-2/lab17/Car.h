#pragma once

#include "Vehicle.h"

class Car final : public Vehicle
{
public:
  Car(std::string brand, int year, std::int64_t price, int power, int detail);
  std::string_view type() const override { return "CAR"; }
  void save(std::ostream& out) const override;

protected:
  void describe(std::ostream& out) const override;

private:
  int seats;
};

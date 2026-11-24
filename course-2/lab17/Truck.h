#pragma once

#include "Vehicle.h"

class Truck final : public Vehicle
{
public:
  Truck(std::string brand, int year, std::int64_t price, int power, int detail);
  std::string_view type() const override { return "TRUCK"; }
  void save(std::ostream& out) const override;

protected:
  void describe(std::ostream& out) const override;

private:
  int capacity;
};

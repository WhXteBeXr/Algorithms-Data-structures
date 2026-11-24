#pragma once

#include "Vehicle.h"

class Motorcycle final : public Vehicle
{
public:
  Motorcycle(std::string brand, int year, std::int64_t price, int power, int detail);
  std::string_view type() const override { return "MOTORCYCLE"; }
  void save(std::ostream& out) const override;

protected:
  void describe(std::ostream& out) const override;

private:
  bool sidecar;
};

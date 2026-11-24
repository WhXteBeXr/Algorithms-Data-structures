#pragma once

#include "Vehicle.h"

#include <cstddef>
#include <functional>
#include <vector>

enum class SortKey { Price, Year, Power };

class Fleet
{
public:
  std::size_t size() const { return items.size(); }
  void add(std::unique_ptr<Vehicle> item);
  const Vehicle& at(std::size_t index) const;
  void remove(std::size_t index);
  void print(std::ostream& out) const;
  void sort(SortKey key);
  std::vector<std::size_t> findBrand(std::string_view brand) const;
  std::vector<std::size_t> findType(std::string_view type) const;
  std::vector<std::size_t> findPrice(std::int64_t minimum, std::int64_t maximum) const;
  void save(const std::string& filename) const;
  void load(const std::string& filename);

private:
  std::vector<std::unique_ptr<Vehicle>> items; // Единственный владелец полиморфных объектов.
  std::vector<std::size_t> matching(const std::function<bool(const Vehicle&)>& predicate) const;
};

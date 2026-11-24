#pragma once

#include "MediaItem.h"

#include <cstddef>
#include <vector>

class Library
{
public:
  std::size_t size() const { return items.size(); }
  void add(std::unique_ptr<MediaItem> item);
  MediaItem& at(std::size_t index);
  void remove(std::size_t index);
  void print(std::ostream& out) const;
  void save(const std::string& filename) const;
  void load(const std::string& filename);

private:
  std::vector<std::unique_ptr<MediaItem>> items; // Коллекция единолично владеет изданиями.
};

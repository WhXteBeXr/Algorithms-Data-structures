#pragma once

#include "MediaItem.h"

class Book final : public MediaItem
{
public:
  Book(std::string title, int year, std::string author, int pages);
  std::string_view type() const override { return "BOOK"; }
  void save(std::ostream& out) const override;

private:
  std::string author;
  int pages;
  void describe(std::ostream& out) const override;
};

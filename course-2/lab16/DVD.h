#pragma once

#include "MediaItem.h"

class DVD final : public MediaItem
{
public:
  DVD(std::string title, int year, std::string director, int minutes);
  std::string_view type() const override { return "DVD"; }
  void save(std::ostream& out) const override;

private:
  std::string director;
  int minutes;
  void describe(std::ostream& out) const override;
};

#pragma once

#include "MediaItem.h"

class Magazine final : public MediaItem
{
public:
  Magazine(std::string title, int year, std::string publisher, int issueNumber);
  std::string_view type() const override { return "MAGAZINE"; }
  void save(std::ostream& out) const override;

private:
  std::string publisher;
  int issueNumber;
  void describe(std::ostream& out) const override;
};

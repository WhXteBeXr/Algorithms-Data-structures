#include "DVD.h"

#include <iomanip>
#include <ostream>
#include <utility>

DVD::DVD(std::string title, int year, std::string director, int minutes)
  : MediaItem(std::move(title), year), director(std::move(director)), minutes(minutes)
{
  checkText(this->director);
  checkAmount(minutes);
}

void DVD::describe(std::ostream& out) const
{
  out << "режиссёр " << std::quoted(director) << ", минут " << minutes;
}

void DVD::save(std::ostream& out) const
{
  saveCommon(out);
  out << std::quoted(director) << ' ' << minutes;
}

#include "Magazine.h"

#include <iomanip>
#include <ostream>
#include <utility>

Magazine::Magazine(std::string title, int year, std::string publisher, int issueNumber)
  : MediaItem(std::move(title), year), publisher(std::move(publisher)), issueNumber(issueNumber)
{
  checkText(this->publisher);
  checkAmount(issueNumber);
}

void Magazine::describe(std::ostream& out) const
{
  out << "издательство " << std::quoted(publisher) << ", выпуск " << issueNumber;
}

void Magazine::save(std::ostream& out) const
{
  saveCommon(out);
  out << std::quoted(publisher) << ' ' << issueNumber;
}

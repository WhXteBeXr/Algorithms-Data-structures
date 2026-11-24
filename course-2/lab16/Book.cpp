#include "Book.h"

#include <iomanip>
#include <ostream>
#include <utility>

Book::Book(std::string title, int year, std::string author, int pages)
  : MediaItem(std::move(title), year), author(std::move(author)), pages(pages)
{
  checkText(this->author);
  checkAmount(pages);
}

void Book::describe(std::ostream& out) const
{
  out << "автор " << std::quoted(author) << ", страниц " << pages;
}

void Book::save(std::ostream& out) const
{
  saveCommon(out);
  out << std::quoted(author) << ' ' << pages;
}

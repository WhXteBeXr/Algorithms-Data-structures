#include "MediaItem.h"
#include "Book.h"
#include "DVD.h"
#include "Magazine.h"

#include <iomanip>
#include <istream>
#include <ostream>
#include <stdexcept>
#include <utility>

void MediaItem::checkText(std::string_view value)
{
  if (value.find_first_not_of(" \t\r\n") == std::string_view::npos ||
      value.find_first_of("\r\n") != std::string_view::npos)
  {
    throw std::invalid_argument("Текстовое поле должно быть непустым и однострочным");
  }
}

void MediaItem::checkAmount(int value)
{
  if (value < 1 || value > 100000)
  {
    throw std::out_of_range("Страницы, выпуск или длительность должны быть от 1 до 100000");
  }
}

MediaItem::MediaItem(std::string title, int year) : title(std::move(title)), year(year)
{
  checkText(this->title);
  if (year < 1 || year > 2100)
  {
    throw std::out_of_range("Год должен быть от 1 до 2100");
  }
}

void MediaItem::issue(std::string reader)
{
  checkText(reader);
  if (isBorrowed())
  {
    throw std::logic_error("Экземпляр уже выдан");
  }
  borrower = std::move(reader);
}

void MediaItem::giveBack()
{
  if (!isBorrowed())
  {
    throw std::logic_error("Экземпляр уже находится в библиотеке");
  }
  borrower.clear();
}

void MediaItem::print(std::ostream& out) const
{
  // Общая часть вывода одна, детали выбираются виртуально по фактическому типу.
  out << type() << ' ' << std::quoted(title) << ", год " << year << ", ";
  describe(out);
  out << (isBorrowed() ? ", выдан: " + borrower : ", доступен") << '\n';
}

void MediaItem::saveCommon(std::ostream& out) const
{
  out << type() << ' ' << std::quoted(title) << ' ' << year << ' ' << std::quoted(borrower) << ' ';
}

std::unique_ptr<MediaItem> MediaItem::create(std::string_view type, std::string title, int year,
                                          std::string creator, int amount)
{
  if (type == "BOOK")
  {
    return std::make_unique<Book>(std::move(title), year, std::move(creator), amount);
  }
  if (type == "MAGAZINE")
  {
    return std::make_unique<Magazine>(std::move(title), year, std::move(creator), amount);
  }
  if (type == "DVD")
  {
    return std::make_unique<DVD>(std::move(title), year, std::move(creator), amount);
  }
  throw std::invalid_argument("Неизвестный тип издания");
}

std::unique_ptr<MediaItem> MediaItem::read(std::istream& in)
{
  std::string type, title, reader, creator;
  int year, amount;
  if (!(in >> type >> std::quoted(title) >> year >> std::quoted(reader) >> std::quoted(creator) >> amount))
  {
    throw std::runtime_error("Неполная или неверная запись издания");
  }
  auto item = create(type, std::move(title), year, std::move(creator), amount);
  if (!reader.empty())
  {
    item->issue(std::move(reader));
  }
  return item;
}

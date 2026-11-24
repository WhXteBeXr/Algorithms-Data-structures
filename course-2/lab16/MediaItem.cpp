#include "MediaItem.h"
#include "Book.h"
#include "DVD.h"
#include "Magazine.h"

#include <iomanip>
#include <istream>
#include <ostream>
#include <stdexcept>
#include <utility>

// Отклоняет пустой, пробельный и многострочный текст, сохраняя формат «один объект - одна строка».
void MediaItem::checkText(std::string_view value)
{
  if (value.find_first_not_of(" \t\r\n") == std::string_view::npos ||
      value.find_first_of("\r\n") != std::string_view::npos)
  {
    throw std::invalid_argument("Текстовое поле должно быть непустым и однострочным");
  }
}

// Общая проверка положительного количества: страниц, номера выпуска или минут.
void MediaItem::checkAmount(int value)
{
  if (value < 1 || value > 100000)
  {
    throw std::out_of_range("Страницы, выпуск или длительность должны быть от 1 до 100000");
  }
}

// Инициализирует проверенную общую часть. Borrower изначально пуст, экземпляр доступен.
MediaItem::MediaItem(std::string title, int year) : title(std::move(title)), year(year)
{
  checkText(this->title);
  if (year < 1 || year > 2100)
  {
    throw std::out_of_range("Год должен быть от 1 до 2100");
  }
}

// Выдаёт доступный экземпляр читателю. Повторная выдача отклоняется до изменения состояния.
void MediaItem::issue(std::string reader)
{
  checkText(reader);
  if (isBorrowed())
  {
    throw std::logic_error("Экземпляр уже выдан");
  }
  // Имя хранится в самом объекте, непустая строка одновременно служит признаком выдачи.
  borrower = std::move(reader);
}

// Возвращает выданный экземпляр в библиотеку, повторный возврат считается ошибкой.
void MediaItem::giveBack()
{
  if (!isBorrowed())
  {
    throw std::logic_error("Экземпляр уже находится в библиотеке");
  }
  // Очистка имени переводит экземпляр в доступное состояние без отдельного флага.
  borrower.clear();
}

// Печатает общую часть, детали наследника и статус выдачи, не меняя объект.
void MediaItem::print(std::ostream& out) const
{
  // Общая часть вывода одна, детали выбираются виртуально по фактическому типу.
  out << type() << ' ' << std::quoted(title) << ", год " << year << ", ";
  describe(out);
  out << (isBorrowed() ? ", выдан: " + borrower : ", доступен") << '\n';
}

// Пишет общий префикс записи, включая тег типа и читателя для восстановления выдачи.
void MediaItem::saveCommon(std::ostream& out) const
{
  out << type() << ' ' << std::quoted(title) << ' ' << year << ' ' << std::quoted(borrower) << ' ';
}

// Фабрика выбирает наследника по тегу и возвращает владельца через единый базовый тип.
// Проверки полей выполняются конструкторами независимо от источника данных.
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

// Разбирает поля одной записи и возвращает готовый объект с восстановленным статусом выдачи.
std::unique_ptr<MediaItem> MediaItem::read(std::istream& in)
{
  std::string type, title, reader, creator;
  int year, amount;
  if (!(in >> type >> std::quoted(title) >> year >> std::quoted(reader) >> std::quoted(creator) >> amount))
  {
    throw std::runtime_error("Неполная или неверная запись издания");
  }
  // Сначала создаётся доступный экземпляр, затем при необходимости восстанавливается читатель.
  auto item = create(type, std::move(title), year, std::move(creator), amount);
  // Обычный issue повторно проверяет имя, поэтому загрузка не обходит правила модели.
  if (!reader.empty())
  {
    item->issue(std::move(reader));
  }
  return item;
}

#include "Book.h"

#include <iomanip>
#include <ostream>
#include <utility>

// Создаёт общую часть издания, затем сохраняет и проверяет автора и число страниц.
// Строки передаются во владение полям через move, ссылок на аргументы не остаётся.
Book::Book(std::string title, int year, std::string author, int pages)
  : MediaItem(std::move(title), year), author(std::move(author)), pages(pages)
{
  // Базовый конструктор уже проверил название и год, здесь проверяются собственные поля.
  checkText(this->author);
  checkAmount(pages);
}

// Дописывает специфичные поля в общий вывод MediaItem::print, перевод строки делает основа.
void Book::describe(std::ostream& out) const
{
  out << "автор " << std::quoted(author) << ", страниц " << pages;
}

// Записывает общие поля и собственную часть в порядке, который ожидает MediaItem::read.
void Book::save(std::ostream& out) const
{
  // Quoted сохраняет пробелы и кавычки в строковом поле, границу записи добавляет Library.
  saveCommon(out);
  out << std::quoted(author) << ' ' << pages;
}

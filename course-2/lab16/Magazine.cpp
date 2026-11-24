#include "Magazine.h"

#include <iomanip>
#include <ostream>
#include <utility>

// Создаёт общую часть издания, затем сохраняет и проверяет издательство и номер выпуска.
// Строки передаются во владение полям через move, ссылок на аргументы не остаётся.
Magazine::Magazine(std::string title, int year, std::string publisher, int issueNumber)
  : MediaItem(std::move(title), year), publisher(std::move(publisher)), issueNumber(issueNumber)
{
  // Базовый конструктор уже проверил название и год, здесь проверяются собственные поля.
  checkText(this->publisher);
  checkAmount(issueNumber);
}

// Дописывает специфичные поля в общий вывод MediaItem::print, перевод строки делает основа.
void Magazine::describe(std::ostream& out) const
{
  out << "издательство " << std::quoted(publisher) << ", выпуск " << issueNumber;
}

// Записывает общие поля и собственную часть в порядке, который ожидает MediaItem::read.
void Magazine::save(std::ostream& out) const
{
  // Quoted сохраняет пробелы и кавычки в строковом поле, границу записи добавляет Library.
  saveCommon(out);
  out << std::quoted(publisher) << ' ' << issueNumber;
}

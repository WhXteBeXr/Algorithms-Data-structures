#include "DVD.h"

#include <iomanip>
#include <ostream>
#include <utility>

// Создаёт общую часть издания, затем сохраняет и проверяет режиссёра и длительность.
// Строки передаются во владение полям через move, ссылок на аргументы не остаётся.
DVD::DVD(std::string title, int year, std::string director, int minutes)
  : MediaItem(std::move(title), year), director(std::move(director)), minutes(minutes)
{
  // Базовый конструктор уже проверил название и год, здесь проверяются собственные поля.
  checkText(this->director);
  checkAmount(minutes);
}

// Дописывает специфичные поля в общий вывод MediaItem::print, перевод строки делает основа.
void DVD::describe(std::ostream& out) const
{
  out << "режиссёр " << std::quoted(director) << ", минут " << minutes;
}

// Записывает общие поля и собственную часть в порядке, который ожидает MediaItem::read.
void DVD::save(std::ostream& out) const
{
  // Quoted сохраняет пробелы и кавычки в строковом поле, границу записи добавляет Library.
  saveCommon(out);
  out << std::quoted(director) << ' ' << minutes;
}

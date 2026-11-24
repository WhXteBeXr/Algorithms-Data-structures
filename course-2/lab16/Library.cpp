#include "Library.h"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <utility>

void Library::add(std::unique_ptr<MediaItem> item)
{
  if (!item)
  {
    throw std::invalid_argument("Нельзя добавить пустой указатель");
  }
  // Владение переходит в vector, параметр после перемещения больше не владеет объектом.
  items.push_back(std::move(item));
}

MediaItem& Library::at(std::size_t index)
{
  if (items.empty())
  {
    throw std::logic_error("Библиотека пуста");
  }
  if (index >= items.size())
  {
    throw std::out_of_range("Нет издания с таким индексом");
  }
  return *items[index];
}

void Library::remove(std::size_t index)
{
  if (index >= items.size())
  {
    throw std::out_of_range("Нет издания для удаления");
  }
  if (items[index]->isBorrowed())
  {
    throw std::logic_error("Сначала верните выданный экземпляр");
  }
  // Сначала уничтожается выбранный объект, затем сдвигаются оставшиеся владельцы.
  items.erase(items.begin() + static_cast<std::ptrdiff_t>(index));
}

void Library::print(std::ostream& out) const
{
  out << "Изданий: " << items.size() << '\n';
  for (std::size_t i = 0; i < items.size(); ++i)
  {
    out << i << ": ";
    items[i]->print(out);
  }
}

void Library::save(const std::string& filename) const
{
  std::ofstream out(filename);
  if (!out)
  {
    throw std::runtime_error("Не удалось открыть файл для записи: " + filename);
  }
  out << "LIBRARY 1 " << items.size() << '\n';
  for (const auto& item : items)
  {
    item->save(out);
    out << '\n';
  }
  out.close(); // Проверяем также ошибки завершающей записи и закрытия файла.
  if (!out)
  {
    throw std::runtime_error("Ошибка записи: " + filename);
  }
}

void Library::load(const std::string& filename)
{
  std::ifstream in(filename);
  if (!in)
  {
    throw std::runtime_error("Не удалось открыть файл для чтения: " + filename);
  }
  std::string line, magic;
  int version;
  long long count;
  if (!std::getline(in, line))
  {
    throw std::runtime_error("Пустой файл библиотеки");
  }
  std::istringstream header(line);
  if (!(header >> magic >> version >> count) || magic != "LIBRARY" || version != 1 || count < 0)
  {
    throw std::runtime_error("Неверный заголовок библиотеки");
  }
  header >> std::ws;
  if (!header.eof())
  {
    throw std::runtime_error("Лишние данные в заголовке");
  }
  // До полной проверки файла действующая коллекция не меняется.
  std::vector<std::unique_ptr<MediaItem>> loaded;
  for (long long i = 0; i < count; ++i)
  {
    if (!std::getline(in, line))
    {
      throw std::runtime_error("Не хватает записей библиотеки");
    }
    std::istringstream record(line);
    try
    {
      auto item = MediaItem::read(record);
      record >> std::ws;
      if (!record.eof())
      {
        throw std::runtime_error("Лишние поля записи");
      }
      loaded.push_back(std::move(item));
    }
    catch (const std::logic_error& error)
    {
      throw std::runtime_error(std::string("Неверные данные издания: ") + error.what());
    }
  }
  while (std::getline(in, line))
  {
    if (line.find_first_not_of(" \t\r") != std::string::npos)
    {
      throw std::runtime_error("Лишние записи после конца библиотеки");
    }
  }
  if (in.bad())
  {
    throw std::runtime_error("Ошибка чтения библиотеки");
  }
  items.swap(loaded); // Старые объекты уничтожатся при выходе, новые уже полностью проверены.
}

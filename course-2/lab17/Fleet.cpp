#include "Fleet.h"

#include <algorithm>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <utility>

void Fleet::add(std::unique_ptr<Vehicle> item)
{
  if (!item)
  {
    throw std::invalid_argument("Нельзя добавить пустой указатель");
  }
  // Владение переходит в vector, параметр после перемещения больше не владеет объектом.
  items.push_back(std::move(item));
}

const Vehicle& Fleet::at(std::size_t index) const
{
  if (items.empty())
  {
    throw std::logic_error("Коллекция пуста");
  }
  if (index >= items.size())
  {
    throw std::out_of_range("Нет транспорта с таким индексом");
  }
  return *items[index];
}

void Fleet::remove(std::size_t index)
{
  if (index >= items.size())
  {
    throw std::out_of_range("Нет транспорта для удаления");
  }
  // Сначала уничтожается выбранный объект, затем сдвигаются оставшиеся владельцы.
  items.erase(items.begin() + static_cast<std::ptrdiff_t>(index));
}

void Fleet::print(std::ostream& out) const
{
  if (items.empty())
  {
    throw std::logic_error("Коллекция пуста");
  }
  out << "Транспортных средств: " << items.size() << '\n';
  for (std::size_t i = 0; i < items.size(); ++i)
  {
    out << i << ": ";
    items[i]->print(out);
  }
}

// Индексы действительны до изменения порядка или состава коллекции.
std::vector<std::size_t> Fleet::matching(const std::function<bool(const Vehicle&)>& predicate) const
{
  std::vector<std::size_t> result;
  auto current = items.cbegin();
  while ((current = std::find_if(current, items.cend(), [&](const auto& item)
          { return predicate(*item); })) != items.cend())
  {
    result.push_back(static_cast<std::size_t>(current - items.cbegin()));
    ++current; // Продолжаем после найденного элемента, а не начинаем поиск заново.
  }
  return result;
}

std::vector<std::size_t> Fleet::findBrand(std::string_view brand) const
{
  return matching([brand](const Vehicle& item) { return item.getBrand() == brand; });
}

std::vector<std::size_t> Fleet::findType(std::string_view type) const
{
  return matching([type](const Vehicle& item) { return item.type() == type; });
}

std::vector<std::size_t> Fleet::findPrice(std::int64_t minimum, std::int64_t maximum) const
{
  if (minimum < 0 || maximum > Vehicle::maxPrice || minimum > maximum)
  {
    throw std::out_of_range("Недопустимый диапазон цен");
  }
  return matching([minimum, maximum](const Vehicle& item)
    { return item.getPrice() >= minimum && item.getPrice() <= maximum; });
}

void Fleet::sort(SortKey key)
{
  // Выбираем ключ один раз, затем один алгоритм переставляет только владельцев.
  std::function<std::int64_t(const Vehicle&)> value;
  switch (key)
  {
  case SortKey::Price: value = [](const Vehicle& v) { return v.getPrice(); }; break;
  case SortKey::Year: value = [](const Vehicle& v) { return v.getYear(); }; break;
  case SortKey::Power: value = [](const Vehicle& v) { return v.getPower(); }; break;
  default: throw std::invalid_argument("Неизвестный критерий сортировки");
  }
  std::sort(items.begin(), items.end(), [&](const auto& left, const auto& right)
    { return value(*left) < value(*right); });
}

void Fleet::save(const std::string& filename) const
{
  std::ofstream out(filename);
  if (!out)
  {
    throw std::runtime_error("Не удалось открыть файл для записи: " + filename);
  }
  out << "FLEET 1 " << items.size() << '\n';
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

void Fleet::load(const std::string& filename)
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
    throw std::runtime_error("Пустой файл каталога");
  }
  std::istringstream header(line);
  if (!(header >> magic >> version >> count) || magic != "FLEET" || version != 1 || count < 0)
  {
    throw std::runtime_error("Неверный заголовок каталога");
  }
  header >> std::ws;
  if (!header.eof())
  {
    throw std::runtime_error("Лишние данные в заголовке");
  }
  // До полной проверки файла действующая коллекция не меняется.
  std::vector<std::unique_ptr<Vehicle>> loaded;
  for (long long i = 0; i < count; ++i)
  {
    if (!std::getline(in, line))
    {
      throw std::runtime_error("Не хватает записей каталога");
    }
    std::istringstream record(line);
    try
    {
      auto item = Vehicle::read(record);
      record >> std::ws;
      if (!record.eof())
      {
        throw std::runtime_error("Лишние поля записи");
      }
      loaded.push_back(std::move(item));
    }
    catch (const std::logic_error& error)
    {
      throw std::runtime_error(std::string("Неверные данные транспорта: ") + error.what());
    }
  }
  while (std::getline(in, line))
  {
    if (line.find_first_not_of(" \t\r") != std::string::npos)
    {
      throw std::runtime_error("Лишние записи после конца каталога");
    }
  }
  if (in.bad())
  {
    throw std::runtime_error("Ошибка чтения каталога");
  }
  items.swap(loaded); // Старые объекты уничтожатся при выходе, новые уже полностью проверены.
}

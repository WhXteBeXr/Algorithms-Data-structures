#include "Fleet.h"

#include <algorithm>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <utility>

// Принимает единоличное владение готовым объектом, пустой указатель отвергает.
void Fleet::add(std::unique_ptr<Vehicle> item)
{
  if (!item)
  {
    throw std::invalid_argument("Нельзя добавить пустой указатель");
  }
  // Владение переходит в vector, параметр после перемещения больше не владеет объектом.
  items.push_back(std::move(item));
}

// Возвращает ссылку без передачи владения. Пустая коллекция и неверный индекс дают исключения.
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
  // Ссылка действует, пока существует объект, удаление или замена коллекции её инвалидирует.
  return *items[index];
}

// Удаляет объект по текущему индексу, последующие индексы сдвигаются на один.
void Fleet::remove(std::size_t index)
{
  if (index >= items.size())
  {
    throw std::out_of_range("Нет транспорта для удаления");
  }
  // Сначала уничтожается выбранный объект, затем сдвигаются оставшиеся владельцы.
  items.erase(items.begin() + static_cast<std::ptrdiff_t>(index));
}

// Выводит текущие индексы и полиморфное описание каждого объекта в переданный поток.
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

// Собирает индексы всех объектов, для которых predicate возвращает true. Поиск занимает O(N) проверок.
// Индексы действительны до изменения порядка или состава коллекции.
std::vector<std::size_t> Fleet::matching(const std::function<bool(const Vehicle&)>& predicate) const
{
  std::vector<std::size_t> result;
  // Итератор отмечает непросмотренную часть. find_if не копирует объекты и не меняет коллекцию.
  auto current = items.cbegin();
  while ((current = std::find_if(current, items.cend(), [&](const auto& item)
          { return predicate(*item); })) != items.cend())
  {
    // Разность итераторов даёт текущий индекс объекта, а не адрес или копию владельца.
    result.push_back(static_cast<std::size_t>(current - items.cbegin()));
    ++current; // Продолжаем после найденного элемента, а не начинаем поиск заново.
  }
  return result;
}

// Ищет все точные совпадения марки с учётом регистра, string_view не сохраняется в коллекции.
std::vector<std::size_t> Fleet::findBrand(std::string_view brand) const
{
  return matching([brand](const Vehicle& item) { return item.getBrand() == brand; });
}

// Возвращает индексы объектов заданного тега CAR, TRUCK или MOTORCYCLE через виртуальный type.
std::vector<std::size_t> Fleet::findType(std::string_view type) const
{
  return matching([type](const Vehicle& item) { return item.type() == type; });
}

// Возвращает индексы объектов в замкнутом диапазоне цен [minimum, maximum] в целых рублях.
std::vector<std::size_t> Fleet::findPrice(std::int64_t minimum, std::int64_t maximum) const
{
  if (minimum < 0 || maximum > Vehicle::maxPrice || minimum > maximum)
  {
    throw std::out_of_range("Недопустимый диапазон цен");
  }
  // Границы захватываются по значению, предикат применяется сразу в текущем вызове matching.
  return matching([minimum, maximum](const Vehicle& item)
    { return item.getPrice() >= minimum && item.getPrice() <= maximum; });
}

// Сортирует по возрастанию выбранного ключа, порядок при равных ключах не гарантируется.
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
  // Сравнение строгое. Переставляются unique_ptr, сами объекты остаются по прежним адресам.
  // Локальная value живёт до завершения сортировки, захват по ссылке безопасен.
  std::sort(items.begin(), items.end(), [&](const auto& left, const auto& right)
    { return value(*left) < value(*right); });
}

// Сохраняет весь каталог в текстовый файл, перезаписывая его. Коллекция в памяти не меняется.
void Fleet::save(const std::string& filename) const
{
  std::ofstream out(filename);
  if (!out)
  {
    throw std::runtime_error("Не удалось открыть файл для записи: " + filename);
  }
  out << "FLEET 1 " << items.size() << '\n';
  // Общий заголовок задаёт формат, виртуальный save каждого наследника записывает его поля.
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

// Заменяет коллекцию содержимым файла только после полной проверки всех записей.
// При ошибке временные объекты освобождаются, прежняя коллекция сохраняется.
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
  // Заголовок разбирается отдельно: проверяем тип файла, версию, число записей и остаток строки.
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
    // Одна строка соответствует одному объекту, поэтому нехватка полей не захватит следующую запись.
    std::istringstream record(line);
    try
    {
      auto item = Vehicle::read(record);
      // После ожидаемых полей допускаются только пробелы, лишние данные означают повреждение записи.
      record >> std::ws;
      if (!record.eof())
      {
        throw std::runtime_error("Лишние поля записи");
      }
      loaded.push_back(std::move(item));
    }
    // Ошибки параметров из конструктора переводим в файловую ошибку с контекстом записи.
    catch (const std::logic_error& error)
    {
      throw std::runtime_error(std::string("Неверные данные транспорта: ") + error.what());
    }
  }
  // После заявленного числа объектов разрешены только пустые или пробельные строки.
  while (std::getline(in, line))
  {
    if (line.find_first_not_of(" \t\r") != std::string::npos)
    {
      throw std::runtime_error("Лишние записи после конца каталога");
    }
  }
  // Физическую ошибку чтения нельзя принять за нормальный конец файла.
  if (in.bad())
  {
    throw std::runtime_error("Ошибка чтения каталога");
  }
  items.swap(loaded); // Старые объекты уничтожатся при выходе, новые уже полностью проверены.
}

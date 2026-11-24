#include "Fleet.h"
#include "Input.h"

#include <array>
#include <limits>
#include <utility>

namespace
{
  constexpr std::array types{"CAR", "TRUCK", "MOTORCYCLE"};

  void menu()
  {
    std::cout << "1 Добавить | 2 Список | 3 Удалить | 4 Сортировать\n"
              << "5 Найти | 6 Сохранить | 7 Загрузить | 8 Меню | 0 Выход\n";
  }

  void addVehicle(Fleet& fleet)
  {
    const int kind = console::number<int>("Тип 1 CAR, 2 TRUCK, 3 MOTORCYCLE: ", 1, 3) - 1;
    auto brand = console::text("Марка: ");
    const int year = console::number<int>("Год (1886..2100): ", 1886, 2100);
    const auto price = console::number<std::int64_t>("Цена, руб.: ", 0, Vehicle::maxPrice);
    const int power = console::number<int>("Мощность, л.с. (1..10000): ", 1, 10000);
    // Только дополнительное поле зависит от выбранного типа, общие поля читаются одинаково.
    const std::array prompts{"Мест (1..9): ", "Грузоподъёмность, кг (1..100000): ", "Коляска (0/1): "};
    const std::array minimum{1, 1, 0};
    const std::array maximum{9, 100000, 1};
    const int detail = console::number<int>(prompts[kind], minimum[kind], maximum[kind]);
    fleet.add(Vehicle::create(types[kind], std::move(brand), year, price, power, detail));
    std::cout << "Транспорт добавлен\n";
  }

  void search(const Fleet& fleet)
  {
    const int kind = console::number<int>("Поиск 1 марка, 2 тип, 3 диапазон цен: ", 1, 3);
    std::vector<std::size_t> indices;
    if (kind == 1)
    {
      indices = fleet.findBrand(console::text("Марка (точное совпадение): "));
    }
    else if (kind == 2)
    {
      const int type = console::number<int>("Тип 1 CAR, 2 TRUCK, 3 MOTORCYCLE: ", 1, 3) - 1;
      indices = fleet.findType(types[type]);
    }
    else
    {
      const auto minimum = console::number<std::int64_t>("Цена от: ", 0, Vehicle::maxPrice);
      const auto maximum = console::number<std::int64_t>("Цена до: ", minimum, Vehicle::maxPrice);
      indices = fleet.findPrice(minimum, maximum);
    }
    // Результаты используются сразу, до сортировки, удаления или загрузки.
    std::cout << "Найдено: " << indices.size() << '\n';
    for (const auto index : indices)
    {
      std::cout << index << ": ";
      fleet.at(index).print(std::cout);
    }
  }

  void saveFleet(const Fleet& fleet)
  {
    for (;;)
    {
      const auto filename = console::line("Файл для сохранения (пусто - отмена): ");
      if (filename.empty())
      {
        return;
      }
      try
      {
        fleet.save(filename);
        std::cout << "Каталог сохранён\n";
        return;
      }
      catch (const std::runtime_error& error)
      {
        // После ошибки записи пользователь может выбрать другое имя без потери данных.
        std::cout << "Ошибка: " << error.what() << "\nУкажите другое имя файла\n";
      }
    }
  }
}

int main()
{
  Fleet fleet;
  std::cout << "Лабораторная 17. Каталог транспорта, продвинутый вариант 3\n";
  menu();
  for (;;)
  {
    try
    {
      const int choice = console::number<int>("\nДействие: ", 0, 8);
      if (choice == 0)
      {
        break;
      }
      switch (choice)
      {
      case 1:
        addVehicle(fleet);
        break;
      case 2:
        fleet.print(std::cout);
        break;
      case 3:
      {
        const int index = console::number<int>("Индекс: ", 0, std::numeric_limits<int>::max());
        fleet.remove(static_cast<std::size_t>(index));
        std::cout << "Транспорт удалён, индексы обновлены\n";
        break;
      }
      case 4:
      {
        const std::array keys{SortKey::Price, SortKey::Year, SortKey::Power};
        const int key = console::number<int>("Критерий 1 цена, 2 год, 3 мощность: ", 1, 3) - 1;
        fleet.sort(keys[key]);
        fleet.print(std::cout); // Сразу виден порядок после выбранной сортировки.
        break;
      }
      case 5:
        search(fleet);
        break;
      case 6:
        saveFleet(fleet);
        break;
      case 7:
        fleet.load(console::text("Файл: "));
        std::cout << "Каталог загружен\n";
        break;
      case 8:
        menu();
        break;
      }
    }
    catch (const std::exception& error)
    {
      if (!std::cin)
      {
        std::cout << "\nВвод завершён\n";
        break;
      }
      std::cout << "Ошибка: " << error.what() << '\n';
    }
  }
}

#pragma once

#include <iosfwd>
#include <memory>
#include <string>
#include <string_view>

class MediaItem
{
public:
  virtual ~MediaItem() = default; // Удаление через unique_ptr<MediaItem> уничтожает и наследника.
  virtual std::string_view type() const = 0;
  virtual void save(std::ostream& out) const = 0;
  void print(std::ostream& out) const;
  bool isBorrowed() const { return !borrower.empty(); }
  void issue(std::string reader);
  void giveBack();

  // Фабрика создаёт конкретный тип, сохраняя общий интерфейс коллекции.
  static std::unique_ptr<MediaItem> create(std::string_view type, std::string title, int year,
                                         std::string creator, int amount);
  static std::unique_ptr<MediaItem> read(std::istream& in);

protected:
  MediaItem(std::string title, int year);
  virtual void describe(std::ostream& out) const = 0;
  void saveCommon(std::ostream& out) const;
  static void checkText(std::string_view value);
  static void checkAmount(int value);

private:
  std::string title;
  int year;
  std::string borrower; // Пустая строка означает, что экземпляр доступен.
};

#include <cmath>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include "Complex.h"
#include "Polynomial.h"

int main()
{
  std::cout << std::fixed << std::setprecision(4);

  const double PI = std::acos(-1.0);
  Complex a(1, 1), b(3, 4);

  std::cout << "a = " << a << ", b = " << b << std::endl;
  std::cout << "arg(a) = " << a.argument() << " (pi/4 = " << PI / 4 << ")" << std::endl;

  a += b;
  std::cout << "a += b -> " << a << std::endl;
  a -= b;
  std::cout << "a -= b -> " << a << std::endl;
  a *= b;
  std::cout << "a *= b -> " << a << std::endl;
  a /= b;
  std::cout << "a /= b -> " << a << std::endl;

  std::cout << "a < b? " << (a < b ? "да" : "нет") << std::endl;
  std::cout << "a > b? " << (a > b ? "да" : "нет") << std::endl;
  std::cout << "a <= b? " << (a <= b ? "да" : "нет") << std::endl;
  std::cout << "a >= b? " << (a >= b ? "да" : "нет") << std::endl;

  std::cout << "++a = " << ++a << std::endl;
  std::cout << "a++ = " << a++ << ", теперь a = " << a << std::endl;
  std::cout << "--a = " << --a << std::endl;
  std::cout << "a-- = " << a-- << ", теперь a = " << a << std::endl;

  Complex z(0, 2);
  std::cout << "z = " << z << ", z(pi/2) = " << z(PI / 2) << std::endl;

  try
  {
    Complex zero;
    std::cout << zero.argument() << std::endl;
  }
  catch (const std::exception& ex)
  {
    std::cout << "Ошибка: " << ex.what() << std::endl;
  }

  std::cout << "\n--------------------\n" << std::endl;

  double ca[] = {1, -3, 2}; // 2x^2 - 3x + 1
  double cb[] = {-1, 1}; // x - 1
  Polynomial p(ca, 2);
  Polynomial q(cb, 1);
  Polynomial copy(p);
  Polynomial zero;

  std::cout << "p = " << p << std::endl;
  std::cout << "q = " << q << std::endl;
  std::cout << "copy = " << copy << ", zero = " << zero << std::endl;
  std::cout << "degree(p) = " << p.getDegree() << std::endl;

  std::cout << "p + q = " << p + q << std::endl;
  std::cout << "p - q = " << p - q << std::endl;
  std::cout << "p * q = " << p * q << std::endl;
  std::cout << "p / q = " << p / q << std::endl;
  std::cout << "p % q = " << p % q << std::endl;

  Polynomial t = p;
  t += q;
  std::cout << "p += q -> " << t << std::endl;
  t = p;
  t -= q;
  std::cout << "p -= q -> " << t << std::endl;
  t = p;
  t *= q;
  std::cout << "p *= q -> " << t << std::endl;
  t = p;
  t /= q;
  std::cout << "p /= q -> " << t << std::endl;

  std::cout << "p == copy? " << (p == copy ? "да" : "нет") << std::endl;
  std::cout << "p != q? " << (p != q ? "да" : "нет") << std::endl;
  std::cout << "p < q? " << (p < q ? "да" : "нет") << std::endl;
  std::cout << "p > q? " << (p > q ? "да" : "нет") << std::endl;
  std::cout << "p <= q? " << (p <= q ? "да" : "нет") << std::endl;
  std::cout << "p >= q? " << (p >= q ? "да" : "нет") << std::endl;

  t = p;
  ++t;
  std::cout << "++p = " << t << std::endl;
  Polynomial old = t++;
  std::cout << "p++ вернул " << old << ", теперь " << t << std::endl;
  --t;
  std::cout << "--p = " << t << std::endl;
  old = t--;
  std::cout << "p-- вернул " << old << ", теперь " << t << std::endl;

  std::cout << "p(2) = " << p(2) << std::endl;
  std::cout << "p[1] = " << p[1] << std::endl;
  t = p;
  t[1] = -5;
  t.setCoefficient(0, 7);
  std::cout << "после записи через [] и setCoefficient: " << t << std::endl;

  std::cout << "p' = " << p.derivative() << std::endl;
  std::cout << "интеграл p (C = 0) = " << p.integral() << std::endl;
  std::cout << "интеграл p (C = 5) = " << p.integral(5) << std::endl;
  std::cout << "интеграл p от 0 до 1 = " << p.definiteIntegral(0, 1) << std::endl;

  Polynomial c(5.0);
  std::cout << "double(c) = " << static_cast<double>(c) << std::endl;
  std::cout << "bool(p) = " << static_cast<bool>(p) << ", bool(zero) = " << static_cast<bool>(zero) << std::endl;

  std::cout << "2 * p = " << 2.0 * p << std::endl;
  std::cout << "p * 0.5 = " << p * 0.5 << std::endl;
  std::cout << "1 + p = " << 1.0 + p << std::endl;
  std::cout << "10 - p = " << 10.0 - p << std::endl;
  std::cout << "p + 3 = " << p + 3.0 << std::endl;

  Polynomial r;
  std::cout << "Введите степень и коэффициенты a0..an: ";
  std::cin >> r;
  std::cout << "Вы ввели: " << r << std::endl;

  try
  {
    std::cout << p / zero << std::endl;
  }
  catch (const std::exception& ex)
  {
    std::cout << "Ошибка: " << ex.what() << std::endl;
  }
  try
  {
    std::cout << p[10] << std::endl;
  }
  catch (const std::exception& ex)
  {
    std::cout << "Ошибка: " << ex.what() << std::endl;
  }
  try
  {
    std::cout << static_cast<double>(p) << std::endl;
  }
  catch (const std::exception& ex)
  {
    std::cout << "Ошибка: " << ex.what() << std::endl;
  }
  try
  {
    Polynomial bad(nullptr, 2);
  }
  catch (const std::exception& ex)
  {
    std::cout << "Ошибка: " << ex.what() << std::endl;
  }
  return 0;
}

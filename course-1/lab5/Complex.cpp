#include "Complex.h"
#include <cmath>
#include <stdexcept>

namespace {
  const double EPS = 1e-9;
}

Complex::Complex() : re(0), im(0) {}
Complex::Complex(double r, double i) : re(r), im(i) {}
Complex::Complex(const Complex& other) : re(other.re), im(other.im) {}
Complex::~Complex() {}

double Complex::getRe() const { return re; }
double Complex::getIm() const { return im; }
void Complex::setRe(double r) { re = r; }
void Complex::setIm(double i) { im = i; }

double Complex::modulus() const {
  return std::sqrt(re * re + im * im);
}

Complex& Complex::operator=(const Complex& other) {
  if (this != &other) {
    re = other.re;
    im = other.im;
  }
  return *this;
}

Complex Complex::operator+(const Complex& other) const {
  return Complex(re + other.re, im + other.im);
}

Complex Complex::operator-(const Complex& other) const {
  return Complex(re - other.re, im - other.im);
}

Complex Complex::operator*(const Complex& other) const {
  return Complex(re * other.re - im * other.im,
                 re * other.im + im * other.re);
}

Complex Complex::operator/(const Complex& other) const {
  double denom = other.re * other.re + other.im * other.im;
  if (denom == 0) {
    throw std::runtime_error("Division by zero");
  }
  return Complex((re * other.re + im * other.im) / denom,
                 (im * other.re - re * other.im) / denom);
}

// Сравнение с допуском, т.к. поля - double
bool Complex::operator==(const Complex& other) const {
  return std::fabs(re - other.re) < EPS && std::fabs(im - other.im) < EPS;
}

bool Complex::operator!=(const Complex& other) const {
  return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, const Complex& c) {
  os << c.re;
  if (c.im >= 0) os << "+";
  os << c.im << "i";
  return os;
}

std::istream& operator>>(std::istream& is, Complex& c) {
  is >> c.re >> c.im;
  return is;
}

double Complex::argument() const {
  if (re == 0 && im == 0) {
    throw std::domain_error("Argument of zero is undefined");
  }
  return std::atan2(im, re);
}

// Выражения справа вычисляются во временный объект, поэтому a *= a безопасно
Complex& Complex::operator+=(const Complex& other) {
  *this = *this + other;
  return *this;
}

Complex& Complex::operator-=(const Complex& other) {
  *this = *this - other;
  return *this;
}

Complex& Complex::operator*=(const Complex& other) {
  *this = *this * other;
  return *this;
}

Complex& Complex::operator/=(const Complex& other) {
  *this = *this / other;
  return *this;
}

bool Complex::operator<(const Complex& other) const {
  return other.modulus() - modulus() > EPS;
}

bool Complex::operator>(const Complex& other) const {
  return other < *this;
}

bool Complex::operator<=(const Complex& other) const {
  return !(other < *this);
}

bool Complex::operator>=(const Complex& other) const {
  return !(*this < other);
}

Complex& Complex::operator++() {
  ++re;
  return *this;
}

Complex Complex::operator++(int) {
  Complex temp = *this;
  ++re;
  return temp;
}

Complex& Complex::operator--() {
  --re;
  return *this;
}

Complex Complex::operator--(int) {
  Complex temp = *this;
  --re;
  return temp;
}

// Поворот на угол angle = умножение на e^(i*angle)
Complex Complex::operator()(double angle) const {
  return Complex(re * std::cos(angle) - im * std::sin(angle),
                 re * std::sin(angle) + im * std::cos(angle));
}
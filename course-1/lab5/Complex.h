#pragma once
#include <iostream>

class Complex {
private:
  double re;
  double im;

public:
  Complex();
  Complex(double r, double i = 0);
  Complex(const Complex& other);
  ~Complex();

  double getRe() const;
  double getIm() const;
  void setRe(double r);
  void setIm(double i);

  double modulus() const;
  double argument() const;

  Complex& operator=(const Complex& other);

  Complex operator+(const Complex& other) const;
  Complex operator-(const Complex& other) const;
  Complex operator*(const Complex& other) const;
  Complex operator/(const Complex& other) const;

  Complex& operator+=(const Complex& other);
  Complex& operator-=(const Complex& other);
  Complex& operator*=(const Complex& other);
  Complex& operator/=(const Complex& other);

  bool operator==(const Complex& other) const;
  bool operator!=(const Complex& other) const;
  bool operator<(const Complex& other) const;
  bool operator>(const Complex& other) const;
  bool operator<=(const Complex& other) const;
  bool operator>=(const Complex& other) const;

  Complex& operator++();
  Complex operator++(int);
  Complex& operator--();
  Complex operator--(int);

  Complex operator()(double angle) const;

  friend std::ostream& operator<<(std::ostream& os, const Complex& c);
  friend std::istream& operator>>(std::istream& is, Complex& c);
};
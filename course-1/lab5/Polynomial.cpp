#include "Polynomial.h"
#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace {
    const double EPS = 1e-9;

    bool nearlyEqual(double a, double b) {
        return std::fabs(a - b) < EPS;
    }
}

// ---------- приватные вспомогательные ----------

// Пересоздаёт массив коэффициентов заданной степени, заполненный нулями
void Polynomial::allocate(int newDegree) {
    delete[] coefficients;
    degree = newDegree;
    coefficients = new double[degree + 1]();
}

// Отбрасывает нулевые старшие коэффициенты
void Polynomial::trim() {
    while (degree > 0 && std::fabs(coefficients[degree]) < EPS) {
        --degree;
    }
}

bool Polynomial::isZero() const {
    return degree == 0 && std::fabs(coefficients[0]) < EPS;
}

// Деление в столбик
void Polynomial::divide(const Polynomial& a, const Polynomial& b,
                        Polynomial& quotient, Polynomial& remainder) {
    if (b.isZero()) {
        throw std::runtime_error("Division by zero polynomial");
    }
    remainder = a;
    quotient = Polynomial();
    if (a.degree < b.degree) return;

    quotient.allocate(a.degree - b.degree);
    for (int i = a.degree - b.degree; i >= 0; --i) {
        double k = remainder.coefficients[i + b.degree] / b.coefficients[b.degree];
        quotient.coefficients[i] = k;
        for (int j = 0; j <= b.degree; ++j) {
            remainder.coefficients[i + j] -= k * b.coefficients[j];
        }
        remainder.coefficients[i + b.degree] = 0;
    }
    quotient.trim();
    remainder.trim();
}

// ---------- конструкторы, деструктор ----------

Polynomial::Polynomial() : coefficients(new double[1]()), degree(0) {}

Polynomial::Polynomial(double c) : coefficients(new double[1]{c}), degree(0) {}

Polynomial::Polynomial(const double* coeffs, int deg) : coefficients(nullptr), degree(0) {
    if (coeffs == nullptr || deg < 0) {
        throw std::invalid_argument("Invalid coefficients or degree");
    }
    coefficients = new double[deg + 1];
    degree = deg;
    for (int i = 0; i <= deg; ++i) {
        coefficients[i] = coeffs[i];
    }
    trim();
}

Polynomial::Polynomial(const Polynomial& other)
    : coefficients(new double[other.degree + 1]), degree(other.degree) {
    for (int i = 0; i <= degree; ++i) {
        coefficients[i] = other.coefficients[i];
    }
}

Polynomial::~Polynomial() {
    delete[] coefficients;
}

Polynomial& Polynomial::operator=(const Polynomial& other) {
    if (this != &other) {
        double* newData = new double[other.degree + 1];
        for (int i = 0; i <= other.degree; ++i) {
            newData[i] = other.coefficients[i];
        }
        delete[] coefficients;
        coefficients = newData;
        degree = other.degree;
    }
    return *this;
}

// ---------- геттеры и сеттеры ----------

int Polynomial::getDegree() const { return degree; }

double Polynomial::getCoefficient(int index) const {
    return (*this)[index];
}

void Polynomial::setCoefficient(int index, double value) {
    (*this)[index] = value;
    trim();
}

// ---------- арифметика ----------

Polynomial Polynomial::operator+(const Polynomial& other) const {
    Polynomial result;
    result.allocate(std::max(degree, other.degree));
    for (int i = 0; i <= degree; ++i) result.coefficients[i] += coefficients[i];
    for (int i = 0; i <= other.degree; ++i) result.coefficients[i] += other.coefficients[i];
    result.trim();
    return result;
}

Polynomial Polynomial::operator-(const Polynomial& other) const {
    Polynomial result;
    result.allocate(std::max(degree, other.degree));
    for (int i = 0; i <= degree; ++i) result.coefficients[i] += coefficients[i];
    for (int i = 0; i <= other.degree; ++i) result.coefficients[i] -= other.coefficients[i];
    result.trim();
    return result;
}

Polynomial Polynomial::operator*(const Polynomial& other) const {
    Polynomial result;
    result.allocate(degree + other.degree);
    for (int i = 0; i <= degree; ++i) {
        for (int j = 0; j <= other.degree; ++j) {
            result.coefficients[i + j] += coefficients[i] * other.coefficients[j];
        }
    }
    result.trim();
    return result;
}

Polynomial Polynomial::operator/(const Polynomial& other) const {
    Polynomial q, r;
    divide(*this, other, q, r);
    return q;
}

Polynomial Polynomial::operator%(const Polynomial& other) const {
    Polynomial q, r;
    divide(*this, other, q, r);
    return r;
}

Polynomial& Polynomial::operator+=(const Polynomial& other) {
    *this = *this + other;
    return *this;
}

Polynomial& Polynomial::operator-=(const Polynomial& other) {
    *this = *this - other;
    return *this;
}

Polynomial& Polynomial::operator*=(const Polynomial& other) {
    *this = *this * other;
    return *this;
}

Polynomial& Polynomial::operator/=(const Polynomial& other) {
    *this = *this / other;
    return *this;
}

// ---------- сравнение ----------

bool Polynomial::operator==(const Polynomial& other) const {
    if (degree != other.degree) return false;
    for (int i = 0; i <= degree; ++i) {
        if (!nearlyEqual(coefficients[i], other.coefficients[i])) return false;
    }
    return true;
}

bool Polynomial::operator!=(const Polynomial& other) const {
    return !(*this == other);
}

bool Polynomial::operator<(const Polynomial& other) const {
    if (degree != other.degree) return degree < other.degree;
    for (int i = degree; i >= 0; --i) {
        if (!nearlyEqual(coefficients[i], other.coefficients[i])) {
            return coefficients[i] < other.coefficients[i];
        }
    }
    return false;
}

bool Polynomial::operator>(const Polynomial& other) const {
    return other < *this;
}

bool Polynomial::operator<=(const Polynomial& other) const {
    return !(other < *this);
}

bool Polynomial::operator>=(const Polynomial& other) const {
    return !(*this < other);
}

// ---------- инкремент / декремент (свободный член) ----------

Polynomial& Polynomial::operator++() {
    coefficients[0] += 1;
    return *this;
}

Polynomial Polynomial::operator++(int) {
    Polynomial temp = *this;
    ++*this;
    return temp;
}

Polynomial& Polynomial::operator--() {
    coefficients[0] -= 1;
    return *this;
}

Polynomial Polynomial::operator--(int) {
    Polynomial temp = *this;
    --*this;
    return temp;
}

// ---------- функтор, индексация, преобразования ----------

// Схема Горнера
double Polynomial::operator()(double x) const {
    double result = 0;
    for (int i = degree; i >= 0; --i) {
        result = result * x + coefficients[i];
    }
    return result;
}

// Запись через неконстантный [] не пересчитывает степень;
// для этого используйте setCoefficient
double& Polynomial::operator[](int index) {
    if (index < 0 || index > degree) {
        throw std::out_of_range("Coefficient index out of range");
    }
    return coefficients[index];
}

const double& Polynomial::operator[](int index) const {
    if (index < 0 || index > degree) {
        throw std::out_of_range("Coefficient index out of range");
    }
    return coefficients[index];
}

Polynomial::operator double() const {
    if (degree != 0) {
        throw std::logic_error("Polynomial is not a constant");
    }
    return coefficients[0];
}

Polynomial::operator bool() const {
    return !isZero();
}

// ---------- дифференцирование и интегрирование ----------

Polynomial Polynomial::derivative() const {
    Polynomial result;
    if (degree == 0) return result;
    result.allocate(degree - 1);
    for (int i = 1; i <= degree; ++i) {
        result.coefficients[i - 1] = i * coefficients[i];
    }
    result.trim();
    return result;
}

// Первообразная с константой интегрирования C
Polynomial Polynomial::integral(double constant) const {
    Polynomial result;
    result.allocate(degree + 1);
    result.coefficients[0] = constant;
    for (int i = 0; i <= degree; ++i) {
        result.coefficients[i + 1] = coefficients[i] / (i + 1);
    }
    result.trim();
    return result;
}

double Polynomial::definiteIntegral(double a, double b) const {
    Polynomial F = integral();
    return F(b) - F(a);
}

// ---------- дружественные функции ----------

Polynomial operator+(double c, const Polynomial& p) {
    Polynomial result = p;
    result.coefficients[0] += c;
    result.trim();
    return result;
}

Polynomial operator-(double c, const Polynomial& p) {
    Polynomial result = p * -1.0;
    result.coefficients[0] += c;
    result.trim();
    return result;
}

Polynomial operator*(double k, const Polynomial& p) {
    Polynomial result = p;
    for (int i = 0; i <= result.degree; ++i) {
        result.coefficients[i] *= k;
    }
    result.trim();
    return result;
}

Polynomial operator*(const Polynomial& p, double k) {
    return k * p;
}

std::ostream& operator<<(std::ostream& os, const Polynomial& p) {
    bool first = true;
    for (int i = p.degree; i >= 0; --i) {
        double c = p.coefficients[i];
        if (std::fabs(c) < EPS && p.degree > 0) continue;

        if (!first) os << (c < 0 ? " - " : " + ");
        else if (c < 0) os << "-";

        double a = std::fabs(c);
        if (i == 0 || a != 1) os << a;
        if (i >= 1) os << "x";
        if (i > 1) os << "^" << i;
        first = false;
    }
    return os;
}

// Формат: степень n, затем n+1 коэффициентов от a0 до an
std::istream& operator>>(std::istream& is, Polynomial& p) {
    int d;
    if (!(is >> d)) return is;
    if (d < 0) {
        is.setstate(std::ios::failbit);
        return is;
    }
    Polynomial tmp;
    tmp.allocate(d);
    for (int i = 0; i <= d; ++i) {
        is >> tmp.coefficients[i];
    }
    if (is) {
        tmp.trim();
        p = tmp;
    }
    return is;
}
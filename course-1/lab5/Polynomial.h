#pragma once
#include <iostream>

class Polynomial {
private:
    double* coefficients;   // coefficients[i] - коэффициент при x^i
    int degree;

    void allocate(int newDegree);
    void trim();
    bool isZero() const;
    static void divide(const Polynomial& a, const Polynomial& b,
                       Polynomial& quotient, Polynomial& remainder);

public:
    Polynomial();
    Polynomial(double c);
    Polynomial(const double* coeffs, int deg);
    Polynomial(const Polynomial& other);
    ~Polynomial();

    int getDegree() const;
    double getCoefficient(int index) const;
    void setCoefficient(int index, double value);

    Polynomial& operator=(const Polynomial& other);

    Polynomial operator+(const Polynomial& other) const;
    Polynomial operator-(const Polynomial& other) const;
    Polynomial operator*(const Polynomial& other) const;
    Polynomial operator/(const Polynomial& other) const;
    Polynomial operator%(const Polynomial& other) const;

    Polynomial& operator+=(const Polynomial& other);
    Polynomial& operator-=(const Polynomial& other);
    Polynomial& operator*=(const Polynomial& other);
    Polynomial& operator/=(const Polynomial& other);

    bool operator==(const Polynomial& other) const;
    bool operator!=(const Polynomial& other) const;
    bool operator<(const Polynomial& other) const;
    bool operator>(const Polynomial& other) const;
    bool operator<=(const Polynomial& other) const;
    bool operator>=(const Polynomial& other) const;

    Polynomial& operator++();
    Polynomial operator++(int);
    Polynomial& operator--();
    Polynomial operator--(int);

    double operator()(double x) const;
    double& operator[](int index);
    const double& operator[](int index) const;

    explicit operator double() const;
    explicit operator bool() const;

    Polynomial derivative() const;
    Polynomial integral(double constant = 0) const;
    double definiteIntegral(double a, double b) const;

    friend Polynomial operator+(double c, const Polynomial& p);
    friend Polynomial operator-(double c, const Polynomial& p);
    friend Polynomial operator*(double k, const Polynomial& p);
    friend Polynomial operator*(const Polynomial& p, double k);

    friend std::ostream& operator<<(std::ostream& os, const Polynomial& p);
    friend std::istream& operator>>(std::istream& is, Polynomial& p);
};
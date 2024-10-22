#ifndef MATRIXPLUS_H
#define MATRIXPLUS_H
#include <iostream>
#include <memory>
#include <new>

class S21Matrix {
  friend S21Matrix operator*(const double, const S21Matrix&);
  friend S21Matrix operator*(const S21Matrix&, const double);

 private:
  int rows_, cols_;
  double* matrix_;

 public:
  S21Matrix() noexcept;
  S21Matrix(int, int);

  ~S21Matrix() noexcept;
  S21Matrix(const S21Matrix& other);
  S21Matrix(S21Matrix&& other) noexcept;
  S21Matrix& operator=(const S21Matrix& other);
  S21Matrix& operator=(S21Matrix&& other) noexcept;
  bool EqMatrix(const S21Matrix& other) const noexcept;
  double& operator()(int i, int j);
  const double& operator()(int i, int j) const;
  bool operator==(const S21Matrix& other) const noexcept;
  void SumMatrix(const S21Matrix& other);
  void SubMatrix(const S21Matrix& other);
  void MulNumber(const double num) noexcept;
  void MulMatrix(const S21Matrix& other);
  // void Fill() noexcept;
  // void Print() const noexcept;
  S21Matrix Transpose() const;
  double Determinant() const;
  S21Matrix CalcComplements() const;
  S21Matrix InverseMatrix() const;
  S21Matrix operator+(const S21Matrix& other) const;
  S21Matrix operator-(const S21Matrix& other) const;
  void operator+=(const S21Matrix& other);
  void operator-=(const S21Matrix& other);
  S21Matrix operator*(const S21Matrix& other) const;
  void operator*=(const S21Matrix& other);
  void operator*=(double n);
  int GetRows() const noexcept { return rows_; }
  int GetCols() const noexcept { return cols_; }
  void SetCols(const int);
  void SetRows(const int);

 private:
  int eqd(double a, double b) const { return std::abs(a - b) < 0.0000001; }
  S21Matrix Create_minor(int excluded_row, int excluded_column) const;
};

S21Matrix operator*(double, const S21Matrix&);
S21Matrix operator*(const S21Matrix&, double);
#endif
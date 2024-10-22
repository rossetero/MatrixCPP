#include "s21_matrix_oop.h"

S21Matrix::S21Matrix() noexcept : rows_(0), cols_(0), matrix_(nullptr) {}

S21Matrix::S21Matrix(int rows, int cols) : rows_(rows), cols_(cols) {
  if (rows_ < 0 || cols_ < 0) {
    throw std::length_error("Matrix size must be greater or equal than 0");
  }
  matrix_ = new double[rows_ * cols_]{};
}

S21Matrix::~S21Matrix() noexcept {
  cols_ = 0;
  rows_ = 0;
  delete[] matrix_;
  matrix_ = nullptr;
}

S21Matrix::S21Matrix(const S21Matrix& other)
    : rows_(other.rows_), cols_(other.cols_) {
  this->matrix_ = new double[rows_ * cols_];
  std::copy(other.matrix_, other.matrix_ + rows_ * cols_, matrix_);
}

S21Matrix::S21Matrix(S21Matrix&& other) noexcept
    : rows_(other.rows_), cols_(other.cols_), matrix_(other.matrix_) {
  other.matrix_ = nullptr;
  other.cols_ = 0;
  other.rows_ = 0;
}

S21Matrix& S21Matrix::operator=(const S21Matrix& other) {
  S21Matrix tmp(other);
  *this = std::move(tmp);

  return *this;
}

S21Matrix& S21Matrix::operator=(S21Matrix&& other) noexcept {
  std::swap(rows_, other.rows_);
  std::swap(cols_, other.cols_);
  std::swap(matrix_, other.matrix_);

  return *this;
}

double& S21Matrix::operator()(int i, int j) {
  if (i >= rows_ || j >= cols_ || j < 0 || i < 0)
    throw std::out_of_range(
        "Trying to access element from index out of matrix\n");
  return matrix_[i * cols_ + j];
}

const double& S21Matrix::operator()(int i, int j) const {
  if (i >= rows_ || j >= cols_ || j < 0 || i < 0)
    throw std::out_of_range(
        "Trying to access element from index out of matrix\n");
  return matrix_[i * cols_ + j];
}

bool S21Matrix::EqMatrix(const S21Matrix& other) const noexcept {
  int eq = true;

  if (rows_ != other.rows_ || cols_ != other.cols_) {
    eq = false;
  }
  for (int i = 0; i < rows_ && eq; ++i) {
    for (int j = 0; j < cols_ && eq; ++j) {
      if (!eqd((*this)(i, j), other(i, j))) {
        eq = false;
      }
    }
  }
  return eq;
}

bool S21Matrix::operator==(const S21Matrix& other) const noexcept {
  return (*this).EqMatrix(other);
}

void S21Matrix::SumMatrix(const S21Matrix& other) {
  if (rows_ != other.rows_ || cols_ != other.cols_)
    throw std::logic_error("Trying to summate matrixes with different size");
  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < cols_; ++j) {
      (*this)(i, j) += other(i, j);
    }
  }
}

void S21Matrix::SubMatrix(const S21Matrix& other) {
  if (rows_ != other.rows_ || cols_ != other.cols_)
    throw std::logic_error("Trying to subtract matrixes with different size");
  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < cols_; ++j) {
      (*this)(i, j) -= other(i, j);
    }
  }
}

void S21Matrix::MulNumber(const double num) noexcept {
  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < cols_; ++j) {
      (*this)(i, j) *= num;
    }
  }
}

void S21Matrix::MulMatrix(const S21Matrix& other) {
  if (cols_ != other.rows_)
    throw std::logic_error(
        "At multipication number of cols in first matrix is not equal to "
        "number of rows in second");
  int r = rows_;
  int q = cols_;
  int c = other.cols_;
  S21Matrix tmp(r, c);
  for (int i = 0; i < r; ++i) {
    for (int j = 0; j < c; ++j) {
      tmp(i, j) = 0;
      for (int k = 0; k < q; ++k) {
        tmp(i, j) += (*this)(i, k) * other(k, j);
      }
    }
  }
  (*this) = std::move(tmp);
}

S21Matrix S21Matrix::Transpose() const {
  S21Matrix t(cols_, rows_);
  for (int i = 0; i < cols_; ++i) {
    for (int j = 0; j < rows_; ++j) {
      t(i, j) = (*this)(j, i);
    }
  }
  return t;
}

double S21Matrix::Determinant() const {
  if (rows_ != cols_) {
    throw std::logic_error("Matrix must be square to find determonant\n");
  }

  double result = 1.0;
  S21Matrix tmp(*this);
  int size = rows_;
  for (int i = 0; i < size; ++i) {
    int ind = i;
    for (int j = i + 1; j < size; ++j) {
      if (std::abs(tmp(j, i)) > std::abs(tmp(ind, i))) {
        ind = j;
      }
    }
    if (std::abs(tmp(ind, i)) < 0.0000001) {
      return 0.0;
    }
    if (i != ind) {
      for (int k = 0; k < cols_; ++k) {
        std::swap(tmp(i, k), tmp(ind, k));
      }
    }
    result *= tmp(i, i);
    if (i != ind) {
      result = -result;
    }
    for (int j = i + 1; j < size; ++j) {
      double koef = tmp(j, i) / tmp(i, i);
      for (int k = i; k < size; ++k) {
        tmp(j, k) -= tmp(i, k) * koef;
      }
    }
  }

  return result;
}

S21Matrix S21Matrix::Create_minor(int excluded_row, int excluded_column) const {
  S21Matrix minor(rows_ - 1, cols_ - 1);
  for (int i = 0, minor_row = 0; i < rows_; ++i) {
    if (i == excluded_row) continue;
    for (int j = 0, minor_column = 0; j < cols_; ++j) {
      if (j == excluded_column) continue;

      minor(minor_row, minor_column) = (*this)(i, j);
      minor_column++;
    }
    minor_row++;
  }
  return minor;
}

S21Matrix S21Matrix::CalcComplements() const {
  if (rows_ != cols_) {
    throw std::logic_error("Matrix must be square to find complements\n");
  }
  S21Matrix result(rows_, cols_);
  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < cols_; ++j) {
      S21Matrix minor = (*this).Create_minor(i, j);
      double det = minor.Determinant();
      int sign = (i + j) % 2 ? -1 : 1;
      result(i, j) = det * sign;
    }
  }
  return result;
}

S21Matrix S21Matrix::InverseMatrix() const {
  double det = (*this).Determinant();
  S21Matrix ret;
  if (rows_ == 1) {
    S21Matrix result(1, 1);
    result(0, 0) = 1 / det;
    ret = std::move(result);
  } else {
    if (eqd(det, 0)) {
      throw std::logic_error("Determinant must be 0 to find inverse matrix\n");
    } else {
      S21Matrix result = CalcComplements().Transpose();
      result.MulNumber(1 / det);
      ret = std::move(result);
    }
  }
  return ret;
}

S21Matrix S21Matrix::operator+(const S21Matrix& other) const {
  S21Matrix tmp(*this);
  tmp.SumMatrix(other);
  return tmp;
}

S21Matrix S21Matrix::operator-(const S21Matrix& other) const {
  S21Matrix tmp(*this);
  tmp.SubMatrix(other);
  return tmp;
}

void S21Matrix::operator+=(const S21Matrix& other) { (*this).SumMatrix(other); }

void S21Matrix::operator-=(const S21Matrix& other) { (*this).SubMatrix(other); }

S21Matrix S21Matrix::operator*(const S21Matrix& other) const {
  S21Matrix tmp(*this);
  tmp.MulMatrix(other);
  return tmp;
}

void S21Matrix::operator*=(const S21Matrix& other) { (*this).MulMatrix(other); }

void S21Matrix::operator*=(double n) { (*this).MulNumber(n); }

S21Matrix operator*(const double num, const S21Matrix& m) {
  S21Matrix tmp(m);
  tmp.MulNumber(num);
  return tmp;
}

S21Matrix operator*(const S21Matrix& m, const double num) {
  S21Matrix tmp(m);
  tmp.MulNumber(num);
  return tmp;
}

void S21Matrix::SetCols(const int new_cols) {
  if (new_cols < 0) {
    throw std::logic_error("Settable size must be greater or equal than 0");
  }
  if (new_cols != cols_) {
    S21Matrix tmp{rows_, new_cols};
    int min = (cols_ > new_cols ? new_cols : cols_);
    for (int i = 0; i < rows_; ++i) {
      for (int j = 0; j < min; ++j) {
        tmp(i, j) = (*this)(i, j);
      }
    }
    *this = std::move(tmp);
  }
}

void S21Matrix::SetRows(const int new_rows) {
  if (new_rows < 0) {
    throw std::logic_error("Settable size must be greater or equal than 0");
  }
  if (new_rows != rows_) {
    S21Matrix tmp{new_rows, cols_};
    int min = (rows_ > new_rows ? new_rows : rows_);
    for (int i = 0; i < min; ++i) {
      for (int j = 0; j < cols_; ++j) {
        tmp(i, j) = (*this)(i, j);
      }
    }
    *this = std::move(tmp);
  }
}

// void S21Matrix::Fill() noexcept {
//   for (int i = 0; i < rows_; ++i) {
//     for (int j = 0; j < cols_; ++j) {
//       std::cin >> (*this)(i, j);
//     }
//   }
//   std::cout << std::endl;
// }

// void S21Matrix::Print() const noexcept {
//   for (int i = 0; i < rows_; ++i) {
//     for (int j = 0; j < cols_; ++j) {
//       std::cout << (*this)(i, j) << " ";
//     }
//     std::cout << std::endl;
//   }

//   std::cout << "----" << std::endl;
// }
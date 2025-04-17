#pragma once
#include <array>

template <unsigned char r, unsigned char c> class Matrix {
  static_assert(r > '\x00' && c > '\x00', "Zero dimension matrix");

  class Iterator {
    float *ptr;

  public:
    inline constexpr Iterator(float *ptr) noexcept : ptr(ptr) {}
    [[nodiscard]] inline constexpr float &operator*() const { return *ptr; }
    [[nodiscard]] inline constexpr float *operator->() const noexcept {
      return ptr;
    }
    [[nodiscard]] inline constexpr Iterator &operator++() noexcept {
      ptr++;
      return *this;
    }
    [[nodiscard]] inline constexpr Iterator operator++(int) noexcept {
      Iterator tmp = *this;
      ptr++;
      return tmp;
    }
    [[nodiscard]] inline bool operator==(const Iterator &ite) const {
      return ptr == ite.ptr;
    }
    [[nodiscard]] inline bool operator!=(const Iterator &ite) const {
      return ptr != ite.ptr;
    }
  };

public:
  std::array<float, r * c> inner;

  template <typename Func>
  inline constexpr Matrix(Func &&func, std::enable_if_t<std::is_invocable_v<Func, unsigned short>, float> = 0.f) : inner{}{
    for (unsigned short i = 0; i < r * c; i++) {
        inner[i] = func(i);
    }
  }
  
  template <typename... Var>
  inline constexpr Matrix(Var &&...args) : inner{static_cast<float>(args)...} {}

  [[nodiscard]] constexpr inline float &index(unsigned char x, unsigned char y) {
    return operator[](x * c + y);
  }
  [[nodiscard]] constexpr inline float index(unsigned char x, unsigned char y) const {
    return operator[](x * c + y);
  }

  [[nodiscard]] constexpr Matrix<r, c> operator+(const Matrix<r, c> &m) const noexcept {
    Matrix<r, c> ret;
    for (unsigned short i(0); i < r * c; i++)
      ret.inner[i] = inner[i] + m.inner[i];
    return ret;
  }
  [[nodiscard]] constexpr Matrix<r, c> operator-(const Matrix<r, c> &m) const noexcept {
    Matrix<r, c> ret;
    for (unsigned short i(0); i < r * c; i++)
      ret.inner[i] = inner[i] - m.inner[i];
    return ret;
  }
  [[nodiscard]] constexpr Matrix<r, c> operator*(float d) const noexcept {
    Matrix<r, c> ret;
    for (unsigned short i(0); i < r * c; i++)
      ret.inner[i] = inner[i] * d;
    return ret;
  }
  [[nodiscard]] constexpr Matrix<r, c> operator/(float d) const noexcept {
    Matrix<r, c> ret;
    for (unsigned short i(0); i < r * c; i++)
      ret.inner[i] = inner[i] / d;
    return ret;
  }
  [[nodiscard]] inline constexpr Iterator begin() noexcept {
    return Iterator(&inner[0]);
  }
  [[nodiscard]] inline constexpr Iterator end() noexcept {
    return Iterator(&inner[r * c]);
  }

  [[nodiscard]] constexpr inline float &operator[](unsigned short index) {
    return inner[index];
  }

  [[nodiscard]] constexpr inline float operator[](unsigned short index) const {
    return inner[index];
  }

  [[nodiscard]] constexpr Matrix<c, r> operator~() const noexcept {
    Matrix<c, r> ret;
    for (unsigned char i(0); i < r; i++)
      for (unsigned char j(0); j < c; j++)
        ret.index(j, i) = index(i, j);
    return ret;
  }

  template <unsigned char n>
  [[nodiscard]] constexpr Matrix<r, n> operator*(const Matrix<c, n> &mat) const noexcept {
    Matrix<r, n> ret;
    for (unsigned char j(0); j < n; j++)
      for (unsigned char k(0); k < c; k++)
        for (unsigned char i(0); i < r; i++)
          ret.index(i, j) += index(i, k) * mat.index(k, j);
    return ret;
  }

  [[nodiscard]] constexpr Matrix<r - '\x01', c - '\x01'> subMatrix(unsigned char x, unsigned char y) const{
    Matrix<r - '\x01', c - '\x01'> ret;
    unsigned short count(0);
    for(unsigned char i(0); i < r; i++){
      if(i == x) continue;
      for(unsigned char j(0); j < c; j++){
        if(j == y) continue;
        ret[count] = index(i, j);
        count++;
      }
    }
    return ret;
  }

  [[nodiscard]] constexpr float operator+() const {
    static_assert(r == c, "Determinant of a non square matrix");
    if constexpr(r == '\x01'){
      return inner[0];
    }
    else if constexpr(r == '\x02'){
      return inner[0] * inner[3] - inner[1] * inner[2];
    }
    else if constexpr(r == '\x03'){
      return inner[0] * (inner[4] * inner[8] - inner[5] * inner[7]) + inner[1] * (inner[5] * inner[6] - inner[3] * inner[8]) + inner[2] * (inner[3] * inner[7] - inner[4] * inner[6]);
    }
    else{
      float coSum(0.f);
      for(unsigned char i(0); i < r; i++)
        coSum += inner[i] * (i % 2 ? -1.f : 1.f) * +subMatrix('\x00', i);
      return coSum;
    }
  }

  constexpr inline void fill(float (*d)(unsigned short)) noexcept {
    for (unsigned short i(0); i < r * c; i++) {
      operator[](i) = d(i);
    }
  }
};

template <typename... Var>
Matrix(Var &&...args) -> Matrix<'\x01', sizeof...(args)>;
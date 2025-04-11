#include <iostream>
#include <array>

template<unsigned char r, unsigned char c>
class Matrix{

    class Iterator{
        double *ptr;
        
    public:
        inline constexpr Iterator(double *ptr) noexcept: ptr(ptr) {}
        [[nodiscard]] inline constexpr double &operator*() const{
            return *ptr;
        }
        [[nodiscard]] inline constexpr double *operator->() const noexcept{
            return ptr;
        }
        [[nodiscard]] inline constexpr Iterator &operator++() noexcept{
            ptr++;
            return *this;
        }  
        [[nodiscard]] inline constexpr Iterator operator++(int) noexcept{
            Iterator tmp = *this;
            ptr++;
            return tmp;
        }
        [[nodiscard]] inline bool operator==(const Iterator& ite) const{
            return ptr == ite.ptr;
        }
        [[nodiscard]] inline bool operator!=(const Iterator& ite) const{
            return ptr != ite.ptr;
        }
    };

public:    
    std::array<double, r * c> inner;
    
    template<typename...Var>
    inline constexpr Matrix(Var &&...args): inner{std::forward<double>(args)...} {
        static_assert(r > '\x00');
        static_assert(c > '\x00');
    }
    
    [[nodiscard]] constexpr Matrix<r, c> operator+(const Matrix<r, c> &m) const noexcept{
        Matrix<r, c> ret;
        for(unsigned short i(0); i < r * c; i++)
            ret.inner[i] = inner[i] + m.inner[i];
        return ret;
    }
    constexpr Matrix<r, c> &operator+=(const Matrix<r, c> &m) noexcept{
        for(unsigned short i(0); i < r * c; i++)
            inner[i] += m.inner[i];
        return *this;
    }
    [[nodiscard]] constexpr Matrix<r, c> operator-(const Matrix<r, c> &m) const noexcept{
        Matrix<r, c> ret;
        for(unsigned short i(0); i < r * c; i++)
            ret.inner[i] = inner[i] - m.inner[i];
        return ret;
    }
    constexpr Matrix<r, c> &operator-=(const Matrix<r, c> &m) noexcept{
        for(unsigned short i(0); i < r * c; i++)
            inner[i] -= m.inner[i];
        return *this;
    }
    [[nodiscard]] constexpr Matrix<r, c> operator*(double d) const noexcept{
        Matrix<r, c> ret;
        for(unsigned short i(0); i < r * c; i++)
            ret.inner[i] = inner[i] * d;
        return ret;
    }
    constexpr Matrix<r, c> &operator*=(double d) noexcept{
        for(double &id : *this)
            id *= d;
        return *this;
    }
    [[nodiscard]] constexpr Matrix<r, c> operator/(double d) const noexcept{
        Matrix<r, c> ret;
        for(unsigned short i(0); i < r * c; i++)
            ret.inner[i] = inner[i] / d;
        return ret;
    }
    constexpr Matrix<r, c> &operator/=(double d) noexcept{
        for(double &id : *this)
            id /= d;
        return *this;
    }
    template<unsigned char e>
    [[nodiscard]] constexpr Matrix<r, e> operator*(const Matrix<c, e> &m) const{
        return {};
    }

    [[nodiscard]] inline constexpr Iterator begin() noexcept{
        return Iterator(&inner[0]);
    }
    [[nodiscard]] inline constexpr Iterator end() noexcept{
        return Iterator(&inner[r * c]);
    }
    
    [[nodiscard]] constexpr inline double &operator[](unsigned char index){
        return inner[index];
    }
    
    [[nodiscard]] constexpr inline double operator[](unsigned char index) const{
        return inner[index];
    }
    
    constexpr inline void fill(double (*d)(const unsigned char)) noexcept{
        for(unsigned char i(0); i < r * c; i++){
            operator[](i) = d(i);
        }
    }
};

template<unsigned char r, unsigned char c>
std::ostream &operator<<(std::ostream &o, const Matrix<r, c> &mat){
    std::operator<<(o, "Matrix [\n");
    for(unsigned short i(0); i < r * c; i++){
        if(i % c == 0) std::operator<<(o, "  ");
        std::operator<<(o.operator<<(mat[i]), i == r * c - 1 ? '\0' : ',');
        if(i % c == c - 1) std::operator<<(o, '\n');
    }
    return std::operator<<(o, ']');
}

#include <array>

template<unsigned char r, unsigned char c>
class Matrix{

    class Iterator{
        float *ptr;
        
    public:
        inline constexpr Iterator(float *ptr) noexcept: ptr(ptr) {}
        [[nodiscard]] inline constexpr float &operator*() const{
            return *ptr;
        }
        [[nodiscard]] inline constexpr float *operator->() const noexcept{
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
    std::array<float, r * c> inner;
    
    template<typename...Var>
    inline constexpr Matrix(Var &&...args): inner{std::forward<float>(args)...} {
        static_assert(r > '\x00');
        static_assert(c > '\x00');
    }
    
    [[nodiscard]] constexpr inline float &index(unsigned char x, unsigned char y){
        return operator[](x * c + y);
    }
    [[nodiscard]] constexpr inline float index(unsigned char x, unsigned char y) const{
        return operator[](x * c + y);
    }
    
    [[nodiscard]] constexpr Matrix<r, c> operator+(const Matrix<r, c> &m) const noexcept{
        Matrix<r, c> ret;
        for(unsigned short i(0); i < r * c; i++)
            ret.inner[i] = inner[i] + m.inner[i];
        return ret;
    }
    [[nodiscard]] constexpr Matrix<r, c> operator-(const Matrix<r, c> &m) const noexcept{
        Matrix<r, c> ret;
        for(unsigned short i(0); i < r * c; i++)
            ret.inner[i] = inner[i] - m.inner[i];
        return ret;
    }
    [[nodiscard]] constexpr Matrix<r, c> operator*(float d) const noexcept{
        Matrix<r, c> ret;
        for(unsigned short i(0); i < r * c; i++)
            ret.inner[i] = inner[i] * d;
        return ret;
    }
    [[nodiscard]] constexpr Matrix<r, c> operator/(float d) const noexcept{
        Matrix<r, c> ret;
        for(unsigned short i(0); i < r * c; i++)
            ret.inner[i] = inner[i] / d;
        return ret;
    }
    [[nodiscard]] inline constexpr Iterator begin() noexcept{
        return Iterator(&inner[0]);
    }
    [[nodiscard]] inline constexpr Iterator end() noexcept{
        return Iterator(&inner[r * c]);
    }
    
    [[nodiscard]] constexpr inline float &operator[](unsigned char index){
        return inner[index];
    }
    
    [[nodiscard]] constexpr inline float operator[](unsigned char index) const{
        return inner[index];
    }
    
    [[nodiscard]] constexpr Matrix<c, r> operator~() const noexcept{
        Matrix<c, r> ret;
        for(unsigned char i(0); i < r; i++)
            for(unsigned char j(0); j < c; j++)
                ret.index(j, i) = index(i, j);
        return ret;
    }
    
    template<unsigned char n>
    [[nodiscard]] constexpr Matrix<r, n> operator*(const Matrix<c, n> &mat) const noexcept{
        Matrix<r, n> ret;
        for(unsigned char j(0); j < n; j++)
            for(unsigned char k(0); k < c; k++)
                for(unsigned char i(0); i < r; i++)
                    ret.index(i, j) += index(i, k) * mat.index(k, j);
        return ret;
    }
    
    double operator+(){ //determinant logic
        return 0;
    }
    
    constexpr inline void fill(float (*d)(const unsigned char)) noexcept{
        for(unsigned char i(0); i < r * c; i++){
            operator[](i) = d(i);
        }
    }
};

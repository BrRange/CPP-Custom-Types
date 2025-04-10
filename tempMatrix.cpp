template<unsigned char r, unsigned char c>
class Matrix{

    class Iterator{
        double *ptr;
        
    public:
        inline constexpr Iterator(double *ptr) noexcept: ptr(ptr) {}
        inline constexpr double &operator*() const{
            return *ptr;
        }
        inline constexpr double *operator->() const noexcept{
            return ptr;
        }
        inline constexpr Iterator &operator++() noexcept{
            ptr++;
            return *this;
        }  
        inline constexpr Iterator operator++(int) noexcept{
            Iterator tmp = *this;
            ptr++;
            return tmp;
        }
        bool operator==(const Iterator& ite) const{
            return ptr == ite.ptr;
        }
        bool operator!=(const Iterator& ite) const{
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
    
    constexpr Matrix operator+(const Matrix<r, c> &m) const noexcept{
        Matrix<r, c> ret;
        for(unsigned short i(0); i < r * c; i++)
            ret.inner[i] = inner[i] + m.inner[i];
        return ret;
    }
    constexpr Matrix operator+=(const Matrix<r, c> &m) noexcept{
        for(unsigned short i(0); i < r * c; i++)
            inner[i] += m.inner[i];
        return std::move(*this);
    }
    constexpr Matrix operator-(const Matrix<r, c> &m) const noexcept{
        Matrix<r, c> ret;
        for(unsigned short i(0); i < r * c; i++)
            ret.inner[i] = inner[i] - m.inner[i];
        return ret;
    }
    constexpr Matrix operator-=(const Matrix<r, c> &m) noexcept{
        for(unsigned short i(0); i < r * c; i++)
            inner[i] -= m.inner[i];
        return std::move(*this);
    }

    inline constexpr Iterator begin() noexcept{
        return Iterator(&inner[0]);
    }

    inline constexpr Iterator end() noexcept{
        return Iterator(&inner[r * c]);
    }
    
    constexpr inline double get(unsigned char index) const{
        return inner[index];
    }
    
    constexpr inline double set(unsigned char index, double val){
        return inner[index] = val;
    }
    
    constexpr inline void fill(double d) noexcept{
        for(double &id : inner) id = d;
    }
};

template<unsigned char r, unsigned char c>
std::ostream &operator<<(std::ostream &o, const Matrix<r, c> &mat){
    std::operator<<(o, "matrix [\n");
    for(unsigned short i(0); i < r * c; i++){
        if(i % c == 0) std::operator<<(o, "  ");
        std::operator<<(o.operator<<(mat.get(i)), i == r * c - 1 ? '\0' : ',');
        if(i % c == c - 1) std::operator<<(o, '\n');
    }
    return std::operator<<(o, ']');
}

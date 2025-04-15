#include <iostream>
#include "matrix.hpp"

template<unsigned char r, unsigned char c>
std::ostream &operator<<(std::ostream &o, const Matrix<r, c> &mat){
    std::operator<<(o, '[');
    for(unsigned short i(0); i < r * c; i++){
        o.operator<<(mat[i]);
        if(i != r * c - 1) std::operator<<(o, i % c == c - 1 ? "\n " : ", ");
    }
    return std::operator<<(o, ']');
}

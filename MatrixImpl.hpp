#pragma once
#include "Matrix.hpp"
#include <iostream>

#define MAT_FILL(x) [](unsigned short i)->float{return x;}

template<unsigned char r, unsigned char c>
constexpr std::ostream &operator<<(std::ostream &o, const Matrix<r, c> &mat){
    std::operator<<(o, '[');
    for(unsigned short i(0); i < r * c; i++){
        o.operator<<(mat[i]);
        if(i != r * c - 1) std::operator<<(o, i % c == c - 1 ? "\n " : ", ");
    }
    return std::operator<<(o, ']');
}

#include <stdexcept>
#include <cmath>
#include <cstring>
#include "bitset2d.h"

Bitset2D::Bitset2D(int width, int height): width(width), height(height) {
    int bytes = (int)ceil(width * height / 8.0);
    data = new char[bytes];
    memset(data, 0, bytes);
}

Bitset2D::~Bitset2D() {
    delete[] data;
}

bool Bitset2D::get(int x, int y) const {
    if (x < 0 || x >= width || y < 0 || y >= height)
        throw std::runtime_error("Read out of bounds");
    int i = x + y * width;
    return (data[i / 8] & (1 << (i % 8))) != 0;
}

void Bitset2D::set(int x, int y, bool v) {
    if (x < 0 || x >= width || y < 0 || y >= height)
        throw std::runtime_error("Write out of bounds");
    int i = x + y * width;
    char mask = 1 << (i % 8);
    data[i / 8] = (data[i / 8] & ~mask) | ((v ? 1 : 0) << (i % 8));
}

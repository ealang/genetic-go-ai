#include <stdexcept>
#include <cmath>
#include <cstring>
#include "bitset2d.h"

Bitset2D::Bitset2D(int width, int height):
    width(width),
    height(height),
    data(), mask()
{
    if (width > maxSize || height > maxSize) {
        throw std::runtime_error("Invalid size");
    }
    for (int i = 0; i < width * height; i++) {
        mask[i] = 1;
    }
}

bool Bitset2D::get(int x, int y) const {
    if (x < 0 || x >= width || y < 0 || y >= height)
        throw std::runtime_error("Read out of bounds");
    return data[x + y * width];
}

int Bitset2D::count() const {
    return data.count();
}

Bitset2D Bitset2D::operator&(const Bitset2D& other) const {
    throwIfIncomatible(*this, other);
    Bitset2D result(width, height);
    result.data = data & other.data;
    return result;
}

Bitset2D Bitset2D::operator~() const {
    Bitset2D result(width, height);
    result.data = ~data & mask;
    return result;
}

void Bitset2D::set(int x, int y, bool v) {
    if (x < 0 || x >= width || y < 0 || y >= height)
        throw std::runtime_error("Write out of bounds");
    data[x + y * width] = v;
}

Bitset2D& Bitset2D::operator&=(const Bitset2D& other) {
    throwIfIncomatible(*this, other);
    data &= other.data;
    return *this;
}

void Bitset2D::throwIfIncomatible(const Bitset2D& a, const Bitset2D& b) {
    if (!(a.width == b.width && a.height == b.height)) {
        throw std::runtime_error("Incompatible sizes");
    }
}

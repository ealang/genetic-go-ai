#include <stdexcept>
#include <cmath>
#include <cstring>
#include "bitset2d.h"

int countBits(uint32_t i) {
    // http://stackoverflow.com/a/109025
    i = i - ((i >> 1) & 0x55555555);
    i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
    return (((i + (i >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
}

int calcSizeBytes(int width, int height) {
    return (int)ceil(width * height / 8.0);
}

Bitset2D::Bitset2D(int width, int height):
    width(width),
    height(height),
    numSet(0),
    data(std::vector<char>(calcSizeBytes(width, height)))
{ }

bool Bitset2D::get(int x, int y) const {
    if (x < 0 || x >= width || y < 0 || y >= height)
        throw std::runtime_error("Read out of bounds");
    int i = x + y * width;
    return (data[i / 8] & (1 << (i % 8))) != 0;
}

int Bitset2D::count() const {
    return numSet;
}

Bitset2D Bitset2D::operator&(const Bitset2D& other) const {
    throwIfIncomatible(*this, other);
    Bitset2D result(width, height);
    for (unsigned int i = 0; i < result.data.size(); i++) {
        result.data[i] = data[i] & other.data[i];
        result.numSet += countBits(result.data[i]);
    }
    return result;
}

Bitset2D Bitset2D::operator~() const {
    Bitset2D result(width, height);
    for (unsigned int i = 0; i < result.data.size(); i++) {
        result.data[i] = ~data[i];
    }
    result.numSet = width * height - numSet;
    return result;
}

void Bitset2D::set(int x, int y, bool v) {
    if (x < 0 || x >= width || y < 0 || y >= height)
        throw std::runtime_error("Write out of bounds");
    int i = x + y * width;
    char oldVal = data[i / 8],
         mask = 1 << (i % 8),
         newVal = (oldVal & ~mask) | (v << (i % 8));
    if (oldVal != newVal) {
        data[i / 8] = newVal;
        if (v) {
            ++numSet;
        } else {
            --numSet;
        }
    }
}

Bitset2D& Bitset2D::operator&=(const Bitset2D& other) {
    throwIfIncomatible(*this, other);
    numSet = 0;
    for (unsigned int i = 0; i < data.size(); i++) {
        data[i] &= other.data[i];
        numSet += countBits(data[i]);
    }
    return *this;
}

void Bitset2D::throwIfIncomatible(const Bitset2D& a, const Bitset2D& b) {
    if (!(a.width == b.width && a.height == b.height)) {
        throw std::runtime_error("Incompatible sizes");
    }
}

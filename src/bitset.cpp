#include <stdexcept>
#include <cmath>
#include <cstring>
#include "bitset.h"

Bitset::Bitset(int size): size(size) {
    int bytes = (int)ceil(size / 8.0);
    data = new char[bytes];
    memset(data, 0, bytes);
}

Bitset::~Bitset() {
    delete[] data;
}

bool Bitset::get(int i) const {
    if (i < 0 || i >= size)
        throw std::runtime_error("Read out of bounds");
    return (data[i / 8] & (1 << (i % 8))) != 0;
}

void Bitset::set(int i, bool v) {
    if (i < 0 || i >= size)
        throw std::runtime_error("Write out of bounds");
    char mask = 1 << (i % 8);
    data[i / 8] = (data[i / 8] & ~mask) | ((v ? 1 : 0) << (i % 8));
}

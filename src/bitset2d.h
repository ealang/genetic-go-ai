#ifndef BITSET_2D_H
#define BITSET_2D_H

#include <bitset>

class Bitset2D {
    static const int maxSize = 9;
    int width, height;
    std::bitset<maxSize * maxSize> data, mask;
public:
    Bitset2D(int width, int height);

    bool get(int x, int y) const;
    int count() const;

    Bitset2D operator&(const Bitset2D& other) const;
    Bitset2D operator~() const;

    void set(int x, int y, bool v=true);
    Bitset2D& operator&=(const Bitset2D& other);
    static void throwIfIncomatible(const Bitset2D&, const Bitset2D&);
};

#endif

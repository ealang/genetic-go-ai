#ifndef BITSET_2D_H
#define BITSET_2D_H

#include <vector>

class Bitset2D {
    int width, height, numSet;
    std::vector<char> data;
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

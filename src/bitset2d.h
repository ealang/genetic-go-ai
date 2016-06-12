#ifndef BITSET_2D_H
#define BITSET_2D_H

class Bitset2D {
    int width, height;
    char *data;
    Bitset2D(const Bitset2D&) = delete;
    Bitset2D& operator=(const Bitset2D&) = delete;
public:
    Bitset2D(int width, int height);
    virtual ~Bitset2D();
    bool get(int x, int y) const;
    void set(int x, int y, bool v=true);
};

#endif

#ifndef BITSET_H
#define BITSET_H

class Bitset {
    int size;
    char *data;
    Bitset(const Bitset&) = delete;
    Bitset& operator=(const Bitset&) = delete;
public:
    Bitset(int size);
    virtual ~Bitset();
    bool get(int i) const;
    void set(int i, bool v);
};

#endif

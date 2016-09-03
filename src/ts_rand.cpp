#include <random>
#include "ts_rand.h"

int ts_rand() {
    static thread_local std::random_device rd;
    static thread_local std::mt19937 gen(rd());
    static thread_local std::uniform_int_distribution<> dist(0, RAND_MAX);
    return dist(gen);
}

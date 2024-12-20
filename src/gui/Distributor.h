#pragma once

#include "experimental/random"
#include <random>
#include <algorithm>
#include "Item.h"

class Distributor {
public:
    Distributor(int size, item *array);
    void flip(int, int);
    void Srandom();
    void Ssorted();
    void Sreversed();
    void Sshuffle();

private:
    int m_size;
    item *m_array;
    std::default_random_engine generator; // Добавлено для генерации случайных чисел
};
#include "Distributor.h"

Distributor::Distributor(int size, item *array) : m_size(size), m_array(array), generator(std::random_device{}()) { }

void Distributor::flip(int i, int j) {
    std::swap(m_array[i], m_array[j]);
}

void Distributor::Srandom() {
    std::uniform_int_distribution<int> distribution(0, m_size - 1);
    for (int i = 0; i < m_size; i++) {
        flip(i, distribution(generator));
    }
}

void Distributor::Ssorted() {
    int lg = 0;
    for (int i = 1; i < m_size; i <<= 1) {
        lg++;
    }
    std::uniform_int_distribution<int> distribution(0, lg * 2);
    for (int i = 0; i < m_size; i++) {
        int index = i + distribution(generator) - lg;
        if (index < 0) index = 0;
        if (index >= m_size) index = m_size - 1;
        flip(i, index);
    }
}

void Distributor::Sreversed() {
    for (int i = 0; i < m_size / 2; i++) {
        flip(i, m_size - 1 - i);
    }
    Ssorted();
}

void Distributor::Sshuffle() {
    Ssorted();
    for (int i = 0; i < m_size / 2; i += 2) {
        flip(i, m_size - 1 - i);
    }
}
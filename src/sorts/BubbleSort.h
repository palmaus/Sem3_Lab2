// BubbleSort.h
#ifndef BUBBLESORT_H
#define BUBBLESORT_H

#include "ISorter.h"
#include <utility>

template <typename T, typename Comparator>
class BubbleSort : public ISorter<T, Comparator> {
private:
    int i = 0;
    int j = 0;
    int n = 0;
    bool finished = false;
    const Comparator* comparator = nullptr;
    int m_cmpCounter = 0;

public:
    int getComparisons() const override {
        return m_cmpCounter;
    }

    void Sort(SharedPtr<MutableSequence<T>> sequence, const Comparator& comp) override {
        n = sequence->getLength();
        finished = false;
        i = 0;
        j = 0;

        if (n <= 1) {
            finished = true;
        }
        comparator = &comp;
        this->sequence = sequence; // Копируем sequence
    }

    std::pair<int, int> getChangedIndices() override {
        return (j < n - 1) ? std::make_pair(j, j + 1) : std::make_pair(-1, -1);
    }

    bool isFinished() override {
        return finished;
    }

    bool step() override {
        if (finished) return false;

        if (i < n - 1) {
            if (j < n - i - 1) {
                ++m_cmpCounter;
                if ((*comparator)(this->sequence->get(j + 1), this->sequence->get(j))) {
                    std::swap(this->sequence->get(j), this->sequence->get(j + 1));
                    return true;
                }
                j++;
            } else {
                i++;
                j = 0;
            }

        } else {
            finished = true;
        }
        return false;
    }

private:
    SharedPtr<MutableSequence<T>> sequence;
};

#endif // BUBBLESORT_H
#ifndef HEAPSORT_H
#define HEAPSORT_H

#include "ISorter.h"
#include <utility>

template <typename T, typename Comparator>
class HeapSort : public ISorter<T, Comparator> {
private:
    int n;
    int heapSize = 0;
    int i = 0;
    bool finished = false;
    const Comparator* comparator = nullptr;
    int m_cmpCounter = 0;
    std::pair<int, int> lastChangedIndices = std::make_pair(-1, -1);

    void heapify(SharedPtr<MutableSequence<T>> sequence, int i) {
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        ++m_cmpCounter;
        if (left < heapSize && (*comparator)(sequence->get(largest), sequence->get(left))) {
            largest = left;
        }
        ++m_cmpCounter;
        if (right < heapSize && (*comparator)(sequence->get(largest), sequence->get(right))) {
            largest = right;
        }

        if (largest != i) {
            lastChangedIndices = std::make_pair(i, largest);
            std::swap(sequence->get(i), sequence->get(largest));
            heapify(sequence, largest);
        }
    }

    void buildHeap(SharedPtr<MutableSequence<T>> sequence) {
        for (int i = n / 2 - 1; i >= 0; i--) {
            heapify(sequence, i);
        }
    }

public:
    int getComparisons() const override {
        return m_cmpCounter;
    }

    std::pair<int, int> getChangedIndices() override {
        return lastChangedIndices;
    }

    void Sort(SharedPtr<MutableSequence<T>> sequence, const Comparator& comp) override {
        n = sequence->getLength();
        heapSize = n;
        finished = false;
        i = n - 1;
        comparator = &comp;
        this->sequence = sequence;

        if (n <= 1) {
            finished = true;
        }

        buildHeap(sequence);
    }

    bool isFinished() override {
        return finished;
    }

    bool step() override {
        if (finished) return false;

        // Сбрасываем индексы последних изменений
        lastChangedIndices = std::make_pair(-1, -1);

        if (i > 0) {
            // Восстанавливаем кучу только для корня
            heapify(sequence, 0);

            // Перемещаем корень в конец кучи
            lastChangedIndices = std::make_pair(0, i);
            std::swap(sequence->get(0), sequence->get(i));

            // Уменьшаем размер кучи и индекс
            heapSize--;
            i--;
            return true;
        } else {
            finished = true;
        }

        return false;
    }

private:
    SharedPtr<MutableSequence<T>> sequence;
};

#endif // HEAPSORT_H
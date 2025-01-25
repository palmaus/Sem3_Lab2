#ifndef MERGESORT_H
#define MERGESORT_H

#include "ISorter.h"
#include <vector>
#include <utility>
#include "MutableSequence.h"
#include "MutableArraySequence.h"

template <typename T, typename Comparator>
class MergeSort : public ISorter<T, Comparator> {
private:
    int m_cmpCounter = 0;
    int n = 0;
    bool finished = false;
    const Comparator* comparator = nullptr;
    int currentMergeSize = 1;
    int currentLeftStart = 0;
    std::pair<int, int> lastChangedIndices = std::make_pair(-1, -1);

    void merge(SharedPtr<MutableSequence<T>> sequence, int left, int mid, int right) {
        int n1 = mid - left + 1;
        int n2 = right - mid;

        auto  L = MakeShared<MutableArraySequence<T>>();
        auto  R = MakeShared<MutableArraySequence<T>>();

        for (int i = 0; i < n1; i++) {
            L->append(sequence->get(left + i));
        }
        for (int j = 0; j < n2; j++) {
            R->append(sequence->get(mid + 1 + j));
        }

        int i = 0;
        int j = 0;
        int k = left;
        lastChangedIndices = std::make_pair(-1, -1);

        while (i < n1 && j < n2) {
            ++m_cmpCounter;
            if ((*comparator)(L->get(i), R->get(j))) {
                lastChangedIndices = std::make_pair(k, left + i);
                sequence->set(k, L->get(i));
                i++;
            } else {
                lastChangedIndices = std::make_pair(k, mid + 1 + j);
                sequence->set(k, R->get(j));
                j++;
            }
            k++;
        }

        while (i < n1) {
            lastChangedIndices = std::make_pair(k, left + i);
            sequence->set(k, L->get(i));
            i++;
            k++;
        }

        while (j < n2) {
            lastChangedIndices = std::make_pair(k, mid + 1 + j);
            sequence->set(k, R->get(j));
            j++;
            k++;
        }
    }

    void mergeSortStep(SharedPtr<MutableSequence<T>> sequence) {
        if (currentLeftStart < n) {
            int mid = std::min(currentLeftStart + currentMergeSize - 1, n - 1);
            int right = std::min(currentLeftStart + 2 * currentMergeSize - 1, n - 1);
            merge(sequence, currentLeftStart, mid, right);
            currentLeftStart += 2 * currentMergeSize;
        } else {
            currentMergeSize *= 2;
            currentLeftStart = 0;
            if (currentMergeSize >= n)
                finished = true;
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
        finished = false;
        currentMergeSize = 1;
        currentLeftStart = 0;
        comparator = &comp;
        this->sequence = sequence;
        if (n <= 1) {
            finished = true;
        }
    }

    bool isFinished() override {
        return finished;
    }

    bool step() override {
        if (finished) return false;
        mergeSortStep(sequence);
        return true;
    }

private:
    SharedPtr<MutableSequence<T>> sequence;
};

#endif // MERGESORT_H
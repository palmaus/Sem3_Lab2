#ifndef SHELLSORT_H
#define SHELLSORT_H

#include "ISorter.h"
#include <utility>
#include <cmath>
#include <vector>

template <typename T, typename Comparator>
class ShellSort : public ISorter<T, Comparator> {
private:
    int m_cmpCounter = 0;
    int n = 0;
    std::vector<int> gaps;
    int gapIndex = 0;
    int i = 0;
    int j = 0;
    bool finished = false;
    const Comparator* comparator = nullptr;
    void initGaps() {
        gaps.clear();
        int k = 0;
        int gap;
        while (true) {
            if (k % 2 == 0) {
                gap = 9 * pow(4, k / 2) - 9 * pow(2, k / 2) + 1;
            } else {
                gap = 8 * pow(4, (k - 1) / 2) - 6 * pow(2, (k + 1) / 2) + 1;
            }
            if (gap < 1)
                break;

            gaps.insert(gaps.begin(), gap);
            k++;
        }

        gapIndex = 0;
        i = 0;
        j = 0;
    }

public:
    int getComparisons() const override {
        return m_cmpCounter;
    }

    std::pair<int, int> getChangedIndices() override {
        return std::make_pair(j, j - gaps[gapIndex]);
    }

    void Sort(SharedPtr<MutableSequence<T>> sequence, const Comparator& comp) override {
        n = sequence->getLength();
        finished = false;
        comparator = &comp;
        this->sequence = sequence;
        initGaps();
        if (n <= 1) {
            finished = true;
        }
    }

    bool isFinished() override {
        return finished;
    }

    bool step() override {
        if (finished) return false;

        if (gapIndex < gaps.size()) {
            if (i < n) {
                if (j < n) {
                    ++m_cmpCounter;
                    if (j >= gaps[gapIndex] && (*comparator)(sequence->get(j), sequence->get(j - gaps[gapIndex]))) {
                        std::swap(sequence->get(j), sequence->get(j - gaps[gapIndex]));
                        j -= gaps[gapIndex];
                        return true;
                    } else {
                        j++;
                    }
                } else {
                    i++;
                    j = i;
                }
            } else {
                i = 0;
                j = 0;
                gapIndex++;
                if (gapIndex >= gaps.size()) {
                    if (!finished) {
                        gapIndex = gaps.size() - 1;
                        finished = true;
                    }

                }
            }
        }

        if (finished && gaps.back() == 1 && i < n - 1) {
            if (j < n - i - 1) {
                if ((*comparator)(sequence->get(j + 1), sequence->get(j))) {
                    std::swap(sequence->get(j), sequence->get(j + 1));
                    return true;
                }
                j++;
            } else {
                i++;
                j = 0;
            }
            return false;
        }


        return false;
    }

private:
    SharedPtr<MutableSequence<T>> sequence;
};

#endif // SHELLSORT_H
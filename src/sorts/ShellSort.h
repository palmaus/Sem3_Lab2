#ifndef SHELLSORT_H
#define SHELLSORT_H

#include "ISorter.h"
#include <utility>
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
    const Comparator* comparator;
    void initGaps() {
        gaps.clear();
        for (int k = n / 2; k > 0; k /= 2)
        {
            gaps.push_back(k);
        }
        gapIndex = 0;
        i = 0;
        j = 0;
    }

public:
    int getComparisons() const override { return m_cmpCounter; }
    std::pair<int, int> getChangedIndices() override { return std::make_pair(j, j - gaps[gapIndex]); }


    void Sort(SharedPtr<MutableListSequence<T>> sequence, const Comparator& comp) override {
        n = sequence->getLength();
        finished = false;
        comparator = &comp;
        this->sequence = sequence;
        initGaps();
        if (n <= 1) {
            finished = true;
        }
    }
    bool isFinished() override { return finished; }

      bool step() override {
         qDebug() << "ShellSort::step() called. i = " << i << ", j = " << j << ", gapIndex = " << gapIndex;
        if (finished) return false;

         if (gapIndex < gaps.size()) {
            if (i < n) {
              if(j < n)
              {
                   ++m_cmpCounter;
                   if (j >= gaps[gapIndex] && (*comparator)(sequence->get(j - gaps[gapIndex]), sequence->get(j)))
                    {
                        qDebug() << "Before swap, j = " << j;
                         std::swap(sequence->get(j), sequence->get(j - gaps[gapIndex]));
                           qDebug() << "Swapped elements at " << j << " and " << j- gaps[gapIndex];
                        int tempJ = j;
                       j -= gaps[gapIndex];
                       while(j >= gaps[gapIndex] && (*comparator)(sequence->get(j - gaps[gapIndex]), sequence->get(j))) {
                         ++m_cmpCounter;
                         std::swap(sequence->get(j), sequence->get(j - gaps[gapIndex]));
                          qDebug() << "Swapped elements at " << j << " and " << j- gaps[gapIndex] << " in inner loop";
                           j -= gaps[gapIndex];
                       }
                           return true;

                    } else
                      {
                           j++;
                       }
               } else{
                     i++;
                    j = i;
                }
            }
            else {
                gapIndex++;
                qDebug() << "gapIndex incremented to: " << gapIndex;

                i = 0;
                j = 0;
            }
        }
        else {
             finished = true;
            qDebug() << "ShellSort finished";
        }
        return false;
    }

private:
    SharedPtr<MutableListSequence<T>> sequence;
};

#endif
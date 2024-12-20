#ifndef HEAPSORT_H
#define HEAPSORT_H

#include "ISorter.h"
#include <utility>

template <typename T, typename Comparator>
class HeapSort : public ISorter<T, Comparator> {
private:
    int n;
    int heapSize;
    int i = 0;
    bool finished = false;
    const Comparator* comparator;
     int m_cmpCounter = 0;

    void heapify(SharedPtr<MutableListSequence<T>> sequence, int i) {
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
            std::swap(sequence->get(i), sequence->get(largest));
            heapify(sequence, largest);
        }
    }

    void buildHeap(SharedPtr<MutableListSequence<T>> sequence)
    {
      for (int i = n / 2 - 1; i >= 0; i--) {
          heapify(sequence, i);
      }
    }

public:
    int getComparisons() const override { return m_cmpCounter; }
     std::pair<int, int> getChangedIndices() override {
      return  i > 0 ? std::make_pair(0, i) : std::make_pair(-1, -1);
     }

    void Sort(SharedPtr<MutableListSequence<T>> sequence, const Comparator& comp) override {
      n = sequence->getLength();
      heapSize = n;
        finished = false;
        i = n - 1;
       comparator = &comp;
        this->sequence = sequence;

        if (n <= 1) {
            finished = true;
        }
    }
    bool isFinished() override { return finished; }

    bool step() override {
        if (finished) return false;

      if (i > 0)
      {

          buildHeap(sequence);
          std::swap(sequence->get(0), sequence->get(i));
           heapSize--;
          i--;

      } else{
            finished = true;
        }

        return true;
    }

private:
     SharedPtr<MutableListSequence<T>> sequence;
};

#endif
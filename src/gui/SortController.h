// SortController.h
#pragma once

#include <QObject>
#include <QTimer>
#include <utility>

#include "MutableListSequence.h"
#include "ISorter.h"
#include "Comparators.h"


class SortController : public QObject {
    Q_OBJECT
public:
    explicit SortController(QObject *parent = nullptr, int delay = 500);
    void setDelay(int delay);
    void setSequence(SharedPtr<MutableSequence<Student>> seq);
    void setComparator(const StudentComparator& comp);
    void setCurrentSorter(ISorter<Student, StudentComparator>* sorter);
    int  getComparisons() const;

    signals:
        void sortStarted();
        void sortStepCompleted(int index1, int index2);
        void sortFinished();

    public:
        void startSorting();
        void resetSorting();

    private:
        void performSortStep();

private:
    SharedPtr<MutableSequence<Student>> sequence;
    const StudentComparator* comparator = nullptr;
    ISorter<Student, StudentComparator>* currentSorter = nullptr;
    QTimer *timer;
    int delay;


};

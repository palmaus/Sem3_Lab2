#include "SortController.h"

SortController::SortController(QObject *parent, int delay) : QObject(parent), delay(delay) {
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &SortController::performSortStep);
}

void SortController::setDelay(int delay)
{
    this->delay = delay;
    timer->setInterval(delay);
}

void SortController::setSequence(SharedPtr<MutableListSequence<Student>> seq) {
    sequence = seq;
}

void SortController::setComparator(const StudentComparator &comp)
{
    comparator = &comp;
}

void SortController::setCurrentSorter(ISorter<Student, StudentComparator>* sorter)
{
  if (currentSorter) {
        delete currentSorter;
    }
  currentSorter = sorter;
}



void SortController::startSorting() {
    if (sequence && currentSorter && comparator) {
        currentSorter->Sort(sequence, *comparator);
        emit sortStarted();
        timer->start(delay);
    }
}


void SortController::resetSorting() {
  if (currentSorter)
  {
      delete currentSorter;
      currentSorter = nullptr;
  }
  if (sequence) {
    emit sortFinished();
  }
  timer->stop();

}

void SortController::performSortStep() {

    if (!currentSorter || !comparator)
    {
        return;
    }



    if(currentSorter->step())
    {
        std::pair<int, int> changedIndices = currentSorter->getChangedIndices();
        emit sortStepCompleted(changedIndices.first, changedIndices.second);

    }

    if (currentSorter->isFinished()) {
        timer->stop();
        emit sortFinished();
        currentSorter = nullptr;

    }
}

int SortController::getComparisons() const {
    if (currentSorter)
    {
        return currentSorter->getComparisons();
    }
    return 0;
}

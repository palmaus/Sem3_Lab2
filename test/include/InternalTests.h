#pragma once

template<typename T>
class LinkedList;

namespace internal_tests {

    template<typename T>
    bool linkedListsEqual(const LinkedList<T> *list1, const LinkedList<T> *list2);

    void testLinkedList();
    void testMutableListSequence();
    void testSharedPtr();
    void testUniquePtr();
    void testWeakPtr();
}
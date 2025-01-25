#pragma once
#include <string>

template<typename T>
class LinkedList;

template<typename T>
class DynamicArray;

template <typename T, typename Comparator>
class ISorter;

namespace internal_tests {
    template<typename T>
    bool linkedListsEqual(const LinkedList<T> *list1, const LinkedList<T> *list2);
    template<typename T>
    bool dynamicArraysEqual(const DynamicArray<T> *arr1, const DynamicArray<T> *arr2);

    template<typename T, typename Comparator>
    void testSorter(ISorter<T, Comparator>* sorter, const std::string& sorterName);

    void testSortingAlgorithms();
    void testMutableArraySequence();
    void testDynamicArray();
    void testLinkedList();
    void testMutableListSequence();
    void testSharedPtr();
    void testUniquePtr();
    void testWeakPtr();
}
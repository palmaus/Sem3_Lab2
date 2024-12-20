#include "InternalTests.h"
#include "TestRunner.h"
#include "MutableListSequence.h"
#include "UniquePtr.h"
#include "SharedPtr.h"
#include "WeakPtr.h"
#include "LinkedList.h"


namespace internal_tests {
    // Вспомогательная функция для сравнения LinkedList
    template<typename T>
    bool linkedListsEqual(const LinkedList<T> *list1, const LinkedList<T> *list2) {
        if (list1->getLength() != list2->getLength()) {
            return false;
        }
        for (int i = 0; i < list1->getLength(); ++i) {
            if (list1->getByIndex(i) != list2->getByIndex(i)) {
                return false;
            }
        }
        return true;
    }


    // Вспомогательная функция для сравнения Sequence
    template<typename T>
    bool sequencesEqual(const MutableSequence<T> *seq1, const MutableSequence<T> *seq2) {
        if (seq1->getLength() != seq2->getLength()) {
            return false;
        }
        for (int i = 0; i < seq1->getLength(); ++i) {
            if (seq1->get(i) != seq2->get(i)) {
                return false;
            }
        }
        return true;
    }

    void testLinkedList() {
        TestRunner runner;

        // Тест создания пустого списка
        runner.expectNoException("testLinkedList::Empty list creation", []() {
            LinkedList<int> list;
        });

        // Тест добавления элементов
        runner.expectNoException("testLinkedList::Appending elements", []() {
            LinkedList<int> list;
            list.append(1);
            list.append(2);
            list.append(3);
        });

        // Тест получения элементов
        runner.expectNoException("testLinkedList::Getting elements", []() {
            LinkedList<int> list;
            list.append(1);
            list.append(2);
            list.append(3);
            if (list.getFirst() != 1) throw std::runtime_error("Incorrect first element.");
            if (list.getLast() != 3) throw std::runtime_error("Incorrect last element.");
            if (list.getByIndex(1) != 2) throw std::runtime_error("Incorrect element at index 1.");
        });

        // Тест получения подсписка
        runner.expectNoException("testLinkedList::Getting sublist", []() {
            LinkedList<int> list;
            list.append(1);
            list.append(2);
            list.append(3);
            list.append(4);
            auto sublist = list.getSubList(1, 2);
            if (sublist->getLength() != 2) throw std::runtime_error("Incorrect sublist length.");
            if (sublist->getFirst() != 2) throw std::runtime_error("Incorrect first element of sublist.");
            if (sublist->getLast() != 3) throw std::runtime_error("Incorrect last element of sublist.");
        });

        // Тест вставки элемента
        runner.expectNoException("testLinkedList::Inserting element", []() {
            LinkedList<int> list;
            list.append(1);
            list.append(3);
            list.insertAt(1, 2);
            if (list.getByIndex(1) != 2) throw std::runtime_error("Incorrect element at index 1 after insertion.");
        });

        // Тест конкатенации
        runner.expectNoException("testLinkedList::Concatenating lists", []() {
            LinkedList<int> list1;
            list1.append(1);
            list1.append(2);
            LinkedList<int> list2;
            list2.append(3);
            list2.append(4);

            // Создаем ожидаемый список
            int expectedData[] = {1, 2, 3, 4};
            LinkedList<int> expectedList;
            for (int i : expectedData) {
                expectedList.append(i);
            }

            auto *concatenated = list1.concatenate(list2);
            if (!linkedListsEqual(concatenated, &expectedList))
                throw std::runtime_error("Incorrect concatenated list.");
            delete concatenated;
        });

        // Тест исключений
        runner.expectException<std::out_of_range>("testLinkedList::GetFirst from empty list", []() {
            LinkedList<int> list;
            list.getFirst();
        });
        runner.expectException<std::out_of_range>("testLinkedList::GetLast from empty list", []() {
            LinkedList<int> list;
            list.getLast();
        });
        runner.expectException<std::out_of_range>("testLinkedList::GetByIndex with invalid index", []() {
            LinkedList<int> list;
            list.append(1);
            list.getByIndex(1);
        });
        runner.expectException<std::out_of_range>("testLinkedList::GetSubList with invalid indexes", []() {
            LinkedList<int> list;
            list.append(1);
            list.getSubList(0, 1);
        });
        runner.expectException<std::out_of_range>("testLinkedList::InsertAt with invalid index", []() {
            LinkedList<int> list;
            list.append(1);
            list.insertAt(2, 2);
        });

        // Тест вставки элемента в несуществующую позицию
        runner.expectException<std::out_of_range>("testLinkedList::InsertAt to non-existent position", []() {
            LinkedList<int> list;
            list.insertAt(1, 2);
        });

        // Тест копирующего конструктора
        runner.expectNoException("testLinkedList::Copy constructor", []() {
            LinkedList<int> list1;
            list1.append(1);
            list1.append(3);
            list1.append(2);
            LinkedList<int> list2(list1); // Вызов копирующего конструктора
            if (!linkedListsEqual(&list1, &list2))
                throw std::runtime_error("Lists are not equal after copy construction.");
        });

        // Тест оператора присваивания
        runner.expectNoException("testLinkedList::Assignment operator", []() {
            LinkedList<int> list1;
            list1.append(1);
            list1.append(2);
            LinkedList<int> list2;
            list2 = list1; // Вызов оператора присваивания
            if (!linkedListsEqual(&list1, &list2))
                throw std::runtime_error("Lists are not equal after assignment.");
        });
    }

    void testMutableListSequence() {
        TestRunner runner;

        // Тест создания пустой последовательности
        runner.expectNoException("testMutableListSequence::Empty sequence creation", []() {
            MutableListSequence<int> seq;
        });

        // Тест создания последовательности из существующего массива
        runner.expectNoException("testMutableListSequence::Sequence from existing array creation", []() {
            int data[] = {1, 2, 3};
            MutableListSequence<int> seq(data, 3);
        });

        // Тест добавления элементов
        runner.expectNoException("testMutableListSequence::Appending elements", []() {
            MutableListSequence<int> seq;
            seq.append(1);
            seq.append(2);
            seq.append(3);
        });

        // Тест получения элементов
        runner.expectNoException("testMutableListSequence::Getting elements", []() {
            int data[] = {1, 2, 3};
            MutableListSequence<int> seq(data, 3);
            if (seq.getFirst() != 1) throw std::runtime_error("Incorrect first element.");
            if (seq.getLast() != 3) throw std::runtime_error("Incorrect last element.");
            if (seq.get(1) != 2) throw std::runtime_error("Incorrect element at index 1.");
        });

        // Тест получения подпоследовательности
        runner.expectNoException("testMutableListSequence::Getting subsequence", []() {
            int data[] = {1, 2, 3, 4, 5};
            MutableListSequence<int> seq(data, 5);
            auto subseq = seq.getSubSequence(1, 3);
            int expectedData[] = {2, 3, 4};
            MutableListSequence<int> expectedSeq(expectedData, 3);
            if (!sequencesEqual(subseq.get(), &expectedSeq))
                throw std::runtime_error("Incorrect subsequence.");
        });

        // Тест вставки элемента
        runner.expectNoException("testMutableListSequence::Inserting element", []() {
            int data[] = {1, 3};
            MutableListSequence<int> seq(data, 2);
            seq.insertAt(1, 2);
            if (seq.get(1) != 2) throw std::runtime_error("Incorrect element at index 1 after insertion.");
        });

        // Тест конкатенации
        runner.expectNoException("testMutableListSequence::Concatenating sequences", []() {
            int data1[] = {1, 2};
            auto seq1 = SharedPtr<MutableListSequence<int>>(new MutableListSequence<int>(data1, 2));
            int data2[] = {3, 4};
            auto seq2 = SharedPtr<MutableListSequence<int>>(new MutableListSequence<int>(data2, 2));
            seq1->concat(seq2);
            int expectedData[] = {1, 2, 3, 4};
            MutableListSequence<int> expectedSeq(expectedData, 4);
            if (!sequencesEqual(seq1.get(), &expectedSeq))
                throw std::runtime_error("Incorrect concatenated sequence.");
        });

        // Тесты на пустые последовательности
        runner.expectException<std::out_of_range>("testMutableListSequence::GetFirst from empty sequence", []() {
            MutableListSequence<int> seq;
            seq.getFirst();
        });
        runner.expectException<std::out_of_range>("testMutableListSequence::GetLast from empty sequence", []() {
            MutableListSequence<int> seq;
            seq.getLast();
        });
        runner.expectException<std::out_of_range>("testMutableListSequence::Get from empty sequence", []() {
            MutableListSequence<int> seq;
            seq.get(0);
        });
        runner.expectException<std::out_of_range>("testMutableListSequence::GetSubSequence from empty sequence", []() {
            MutableListSequence<int> seq;
            seq.getSubSequence(0, 0);
        });

        // Тесты на отрицательные индексы
        runner.expectException<std::out_of_range>("testMutableListSequence::Get with negative index", []() {
            int data[] = {1, 2, 3};
            MutableListSequence<int> seq(data, 3);
            seq.get(-1);
        });
        runner.expectException<std::out_of_range>("testMutableListSequence::InsertAt with negative index", []() {
            int data[] = {1, 2, 3};
            MutableListSequence<int> seq(data, 3);
            seq.insertAt(-1, 4);
        });
        runner.expectException<std::out_of_range>("testMutableListSequence::GetSubSequence with negative start index", []() {
            int data[] = {1, 2, 3};
            MutableListSequence<int> seq(data, 3);
            seq.getSubSequence(-1, 1);
        });
        runner.expectException<std::out_of_range>("testMutableListSequence::GetSubSequence with negative end index", []() {
            int data[] = {1, 2, 3};
            MutableListSequence<int> seq(data, 3);
            seq.getSubSequence(0, -1);
        });

        // Тесты на индексы вне диапазона
        runner.expectException<std::out_of_range>("testMutableListSequence::Get with index out of range", []() {
            int data[] = {1, 2, 3};
            MutableListSequence<int> seq(data, 3);
            seq.get(3);
        });
        runner.expectException<std::out_of_range>("testMutableListSequence::InsertAt with index out of range", []() {
            int data[] = {1, 2, 3};
            MutableListSequence<int> seq(data, 3);
            seq.insertAt(4, 4);
        });
        runner.expectException<std::out_of_range>("testMutableListSequence::GetSubSequence with start index out of range", []() {
            int data[] = {1, 2, 3};
            MutableListSequence<int> seq(data, 3);
            seq.getSubSequence(3, 2);
        });
        runner.expectException<std::out_of_range>("testMutableListSequence::GetSubSequence with end index out of range", []() {
            int data[] = {1, 2, 3};
            MutableListSequence<int> seq(data, 3);
            seq.getSubSequence(0, 4);
        });

        // Тест копирования мутабельной последовательности
        runner.expectNoException("testMutableListSequence::Copy mutable sequence", []() {
            int data[] = {1, 2};
            MutableListSequence<int> seq1(data, 2);
            MutableListSequence<int> seq2(seq1);

            // Проверяем, что seq2 - копия seq1
            if (!sequencesEqual(&seq1, &seq2)) throw std::runtime_error("Sequences are not equal after copy.");

            // Изменяем seq2 и проверяем, что seq1 не изменилась
            seq2.append(3);
            if (sequencesEqual(&seq1, &seq2)) throw std::runtime_error("Original sequence changed after modifying the copy.");
        });

        // Тест присваивания мутабельной последовательности
        runner.expectNoException("testMutableListSequence::Assign mutable sequence", []() {
            int data[] = {1, 2};
            MutableListSequence<int> seq1(data, 2);

            MutableListSequence<int> seq2;
            seq2 = seq1;

            // Проверяем, что seq2 - копия seq1
            if (!sequencesEqual(&seq1, &seq2)) throw std::runtime_error("Sequences are not equal after assignment.");

            // Изменяем seq2 и проверяем, что seq1 не изменилась
            seq2.append(3);
            if (sequencesEqual(&seq1, &seq2)) throw std::runtime_error("Original sequence changed after modifying the assigned sequence.");
        });

        // Тест методов tryGet, tryGetFirst, tryGetLast
        runner.expectNoException("testMutableListSequence::TryGet methods", []() {
            int data[] = {1, 2, 3};
            MutableListSequence<int> seq(data, 3);

            // tryGet
            auto opt1 = seq.tryGet(1);
            if (opt1.isNone()) throw std::runtime_error("tryGet(1) returned None for existing element.");
            if (opt1.getValue() != 2) throw std::runtime_error("tryGet(1) returned incorrect value.");

            auto opt2 = seq.tryGet(3);
            if (opt2.isSome()) throw std::runtime_error("tryGet(3) returned Some for non-existing element.");

            // tryGetFirst
            auto opt3 = seq.tryGetFirst();
            if (opt3.isNone()) throw std::runtime_error("tryGetFirst returned None for non-empty sequence.");
            if (opt3.getValue() != 1) throw std::runtime_error("tryGetFirst returned incorrect value.");

            // tryGetLast
            auto opt4 = seq.tryGetLast();
            if (opt4.isNone()) throw std::runtime_error("tryGetLast returned None for non-empty sequence.");
            if (opt4.getValue() != 3) throw std::runtime_error("tryGetLast returned incorrect value.");
        });
    }

    void testSharedPtr() {
        TestRunner runner;

        // 1.  Тест создания и уничтожения SharedPtr
        runner.expectNoException("SharedPtr::Basic creation and destruction", []() {
            SharedPtr<int> ptr(new int(10));
        });

        // 2.  Тест подсчета ссылок (use_count)
        runner.expectNoException("SharedPtr::Reference counting", []() {
            SharedPtr<int> ptr1(new int(10));
            {
                SharedPtr<int> ptr2 = ptr1;
                if (ptr1.use_count() != 2 || ptr2.use_count() != 2) {
                    throw std::runtime_error("Incorrect reference count.");
                }
            } // ptr2 выходит за область видимости
            if (ptr1.use_count() != 1) {
                throw std::runtime_error("Incorrect reference count after destruction.");
            }
        });

        // 3.  Тест оператора разыменования (*)
        runner.expectNoException("SharedPtr::Dereference operator", []() {
            SharedPtr<int> ptr(new int(10));
            if (*ptr != 10) {
                throw std::runtime_error("Incorrect dereference value.");
            }
        });

        // 4.  Тест оператора стрелка (->)
        runner.expectNoException("SharedPtr::Arrow operator", []() {
            SharedPtr<std::string> ptr(new std::string("hello"));
            if (ptr->size() != 5) {
                throw std::runtime_error("Incorrect arrow operator access.");
            }
        });

        // 5.  Тест get()
        runner.expectNoException("SharedPtr::get() method", []() {
            SharedPtr<int> ptr(new int(10));
            int* rawPtr = ptr.get();
            if (*rawPtr != 10) {
                throw std::runtime_error("Incorrect raw pointer access.");
            }
        });

        // 6.  Тест reset()
        runner.expectNoException("SharedPtr::reset() method", []() {
            SharedPtr<int> ptr(new int(10));
            ptr.reset();
            if (ptr.get() != nullptr || ptr.use_count() != 0) {
                throw std::runtime_error("Incorrect reset behavior.");
            }
        });

        // 7.  Тест reset() с новым указателем
        runner.expectNoException("SharedPtr::reset() with new pointer", []() {
            SharedPtr<int> ptr(new int(10));
            ptr.reset(new int(20));
            if (*ptr != 20 || ptr.use_count() != 1) {
                throw std::runtime_error("Incorrect reset with new pointer behavior.");
            }
        });

        // 8.  Тест конструктора копирования
        runner.expectNoException("SharedPtr::Copy constructor", []() {
            SharedPtr<int> ptr1(new int(10));
            SharedPtr<int> ptr2(ptr1);
            if (*ptr1 != 10 || *ptr2 != 10 || ptr1.use_count() != 2 || ptr2.use_count() != 2) {
                throw std::runtime_error("Incorrect copy constructor behavior.");
            }
        });

        // 9.  Тест оператора присваивания
        runner.expectNoException("SharedPtr::Assignment operator", []() {
            SharedPtr<int> ptr1(new int(10));
            SharedPtr<int> ptr2;
            ptr2 = ptr1;
            if (*ptr1 != 10 || *ptr2 != 10 || ptr1.use_count() != 2 || ptr2.use_count() != 2) {
                throw std::runtime_error("Incorrect assignment operator behavior.");
            }
        });

        // 10. Тест MakeShared (для одиночных объектов)
        runner.expectNoException("SharedPtr::MakeShared (single object)", []() {
            auto ptr = MakeShared<int>(30);
            if (*ptr != 30 || ptr.use_count() != 1) {
                throw std::runtime_error("Incorrect MakeShared behavior (single object).");
            }
        });
    }
    void testUniquePtr() {
        TestRunner runner;

        // 1. Базовый тест создания и уничтожения
        runner.expectNoException("UniquePtr::Basic creation and destruction", []() {
            UniquePtr<int> ptr(new int(5));
        });

        // 2. Тест оператора разыменования
        runner.expectNoException("UniquePtr::Dereference operator", []() {
            UniquePtr<int> ptr(new int(5));
            if (*ptr != 5) {
                throw std::runtime_error("Incorrect dereference value.");
            }
        });

        // 3. Тест оператора стрелка
        runner.expectNoException("UniquePtr::Arrow operator", []() {
            UniquePtr<std::string> ptr(new std::string("hello"));
            if (ptr->length() != 5) {
                throw std::runtime_error("Incorrect arrow operator usage.");
            }
        });

        // 4. Тест get()
        runner.expectNoException("UniquePtr::get() method", []() {
           UniquePtr<int> ptr(new int(5));
           if (*(ptr.get()) != 5) {
               throw std::runtime_error("Incorrect get() usage.");
           }
        });

        // 5. Тест release()
        runner.expectNoException("UniquePtr::release() method", []() {
            UniquePtr<int> ptr(new int(5));
            int* rawPtr = ptr.release();
            if (ptr.get() != nullptr) {
                throw std::runtime_error("Pointer not released.");
            }
            if (*rawPtr != 5) {
                throw std::runtime_error("Incorrect released pointer value.");
            }
            delete rawPtr; // Необходимо удалить вручную после release()
        });

        // 6. Тест reset()
        runner.expectNoException("UniquePtr::reset() method", []() {
           UniquePtr<int> ptr(new int(5));
           ptr.reset(new int(10));
           if (*ptr != 10) {
               throw std::runtime_error("Incorrect reset() usage.");
           }
        });

        // 7. Тест reset() с nullptr
        runner.expectNoException("UniquePtr::reset() with nullptr", []() {
            UniquePtr<int> ptr(new int(5));
            ptr.reset();
            if (ptr.get() != nullptr) {
                throw std::runtime_error("Pointer not reset to nullptr.");
            }
        });


        // 8. Тест конструктора перемещения
        runner.expectNoException("UniquePtr::Move constructor", []() {
            UniquePtr<int> ptr1(new int(5));
            UniquePtr<int> ptr2(std::move(ptr1));
            if (ptr1.get() != nullptr) {
                throw std::runtime_error("Original pointer not nulled after move.");
            }
            if (*ptr2 != 5) {
                throw std::runtime_error("Incorrect moved value.");
            }
        });

        // 9. Тест оператора перемещения
        runner.expectNoException("UniquePtr::Move assignment operator", []() {
            UniquePtr<int> ptr1(new int(5));
            UniquePtr<int> ptr2;
            ptr2 = std::move(ptr1);
            if (ptr1.get() != nullptr) {
                throw std::runtime_error("Original pointer not nulled after move assignment.");
            }
            if (*ptr2 != 5) {
                throw std::runtime_error("Incorrect moved value.");
            }
        });

        // 10. Тест make_unique
        runner.expectNoException("UniquePtr::make_unique test", []() {
            auto ptr = make_unique<int>(42);
            if (*ptr != 42 || ptr.get() == nullptr) {
                throw std::runtime_error("Incorrect make_unique behavior.");
            }
        });
    }

    void testWeakPtr() {
        TestRunner runner;

        // 1. Базовый тест создания из SharedPtr
        runner.expectNoException("WeakPtr::Basic creation from SharedPtr", []() {
            SharedPtr<int> sharedPtr(new int(10));
            WeakPtr<int> weakPtr(sharedPtr);
        });

        // 2. Тест expired()
        runner.expectNoException("WeakPtr::expired() method", []() {
            SharedPtr<int> sharedPtr(new int(10));
            WeakPtr<int> weakPtr(sharedPtr);
            if (weakPtr.expired()) {
                throw std::runtime_error("WeakPtr should not be expired.");
            }
            sharedPtr.reset();
            if (!weakPtr.expired()) {
                throw std::runtime_error("WeakPtr should be expired.");
            }
        });

        // 3. Тест use_count()
        runner.expectNoException("WeakPtr::use_count() method", []() {
            SharedPtr<int> sharedPtr(new int(10));
            WeakPtr<int> weakPtr1(sharedPtr);
            if (weakPtr1.use_count() != 1) {
                throw std::runtime_error("Incorrect use_count().");
            }
            {
                SharedPtr<int> sharedPtr2 = sharedPtr;
                WeakPtr<int> weakPtr2(sharedPtr);
                if (weakPtr1.use_count() != 2) {
                    throw std::runtime_error("Incorrect use_count().");
                }
            }
            if (weakPtr1.use_count() != 1) {
                throw std::runtime_error("Incorrect use_count() after SharedPtr destruction.");
            }
        });
    }
}

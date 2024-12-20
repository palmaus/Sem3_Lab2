#pragma once

#include "Option.h"
#include "SharedPtr.h"


template<class T>
class MutableSequence {
public:

    ~MutableSequence() = default;

    virtual const T &get(int index) const = 0;

    virtual const T &getFirst() const = 0;

    virtual const T &getLast() const = 0;

    virtual int getLength() const = 0;

    virtual Option<T> tryGetFirst() const = 0;

    virtual Option<T> tryGetLast() const = 0;

    virtual Option<T> tryGet(int index) const = 0;

    virtual SharedPtr<MutableSequence<T>> getSubSequence(int startIndex, int endIndex) const = 0;

    virtual void append(const T &item) = 0;

    virtual void prepend(const T &item) = 0;

    virtual void insertAt(int index, const T &item) = 0;

    virtual void removeAt(int index) = 0;


};
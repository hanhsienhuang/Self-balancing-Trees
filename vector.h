#ifndef VECTOR_H
#define VECTOR_H

#include<cstddef>


template<typename T>
class Vector
{
private:
    void change_capacity(size_t);
    size_t size_;
    size_t capacity_;
    T * ptr;

    class Iterator;
public:
    Vector() noexcept;
    Vector(const Vector & other) noexcept;
    Vector(Vector && other) noexcept;
    Vector & operator=(const Vector & other) noexcept;
    Vector & operator=(Vector && other) noexcept;
    ~Vector() noexcept;

    void push_back(const T&);
    void push_back(T&&);
    T pop_back();
    T& operator[](size_t);
    size_t size() const noexcept;
    size_t capacity() const noexcept;

    Iterator begin();
    Iterator end();

};

template<typename T>
class Vector<T>::Iterator{
public:
    Iterator() = delete;
    Iterator(Vector& , size_t) noexcept;
    Iterator& operator++();
    Iterator& operator--();
    T& operator*();
    bool operator==(const Iterator&) const;
    bool operator!=(const Iterator&) const;
private:
    size_t index;
    Vector& vector;
};


#include"vector.cpp"
#endif
#include"vector.h"
#include<utility>

template<typename T>
Vector<T>::Vector() noexcept 
    :size_(0),
    capacity_(0),
    ptr(nullptr){}

template<typename T>
Vector<T>::Vector(const Vector & other) noexcept
    :size_(other.size_),
    capacity_(other.capacity_),
    ptr(nullptr)
{
    ptr = new T[capacity_];
    for(size_t i=0; i<capacity_; ++i){
        ptr[i] = other.ptr[i];
    }
}

template<typename T>
Vector<T>::Vector(Vector && other) noexcept
    : size_(other.size_),
    capacity_(other.capacity_),
    ptr(other.ptr)
{
    other.ptr = nullptr;
}

template<typename T>
Vector<T> & Vector<T>::operator=(const Vector & other) noexcept
{
    if(this != &other){
        size_ = other.size_;
        capacity_ = other.capacity_;
        delete[] ptr;
        ptr = new T[capacity_];
        for(size_t i=0; i<capacity_; ++i){
            ptr[i] = other.ptr[i];
        }
    }
    return *this;
}

template<typename T>
Vector<T> & Vector<T>::operator=(Vector && other) noexcept{
    size_ = other.size_;
    capacity_ = other.capacity_;
    std::swap(ptr, other.ptr);
    return *this;
}

template<typename T>
Vector<T>::~Vector() noexcept {
    delete[] ptr;
}

#include<stdexcept>

template<typename T>
void Vector<T>::push_back(const T& element){
    if(size_ == capacity_){
        size_t newCapacity = capacity_ == 0?1:(capacity_<<1);
        change_capacity(newCapacity);
    }
    ptr[size_] = element;
    ++size_;
}

template<typename T>
void Vector<T>::push_back(T&& element){
    if(size_ == capacity_){
        size_t newCapacity = capacity_ == 0?1:(capacity_<<1);
        change_capacity(newCapacity);
    }
    ptr[size_] = std::move(element);
    ++size_;
}

template<typename T>
T Vector<T>::pop_back(){
    if(size_ < (capacity_>>2)){
        size_t newCapacity = capacity_>>2;
        change_capacity(newCapacity);
    }
    --size_;
    return std::move(ptr[size_]);
}

template<typename T>
T& Vector<T>::operator[](size_t index){
    if(index > size_){
        throw std::out_of_range("Out of range");
    }
    return ptr[index];
}

template<typename T>
void Vector<T>::change_capacity(size_t newCapacity){
    T * new_ptr = newCapacity==0?(nullptr):(new T[newCapacity]);
    for(size_t i=0; i<size_; ++i){
        new_ptr[i] = ptr[i];
    }
    delete[] ptr;
    ptr = new_ptr;
    capacity_ = newCapacity;
}

template<typename T>
size_t Vector<T>::size() const noexcept {
    return size_;
}

template<typename T>
size_t Vector<T>::capacity() const noexcept {
    return capacity_;
}

template<typename T>
typename Vector<T>::Iterator Vector<T>::begin(){
    return Iterator{*this, 0};
}

template<typename T>
typename Vector<T>::Iterator Vector<T>::end(){
    return Iterator{*this, size_};
}


template<typename T>
Vector<T>::Iterator::Iterator(Vector& vector_, size_t index_) noexcept
:vector(vector_),
index(index_)
{}

template<typename T>
typename Vector<T>::Iterator& Vector<T>::Iterator::operator++(){
    ++index;
    return *this;
}

template<typename T>
typename Vector<T>::Iterator& Vector<T>::Iterator::operator--(){
    --index;
    return *this;
}

template<typename T>
T& Vector<T>::Iterator::operator*(){
    return vector[index];
}

template<typename T>
bool Vector<T>::Iterator::operator==(const Iterator& other) const{
    return index == other.index;
}

template<typename T>
bool Vector<T>::Iterator::operator!=(const Iterator& other) const{
    return index != other.index;
}
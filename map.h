#ifndef MAP_H
#define MAP_H

#include<cstddef>
#include<utility>
#include<stdexcept>





template<typename Key, typename T, template<typename> typename Tree>
class Map{
public:
    size_t size() const noexcept{
        return tree.size()
    };

    T& operator[](const Key&);
    void insert(const Key&, const T&);
    size_t erase(const Key&);


    IteratorMap<Key, T> begin() noexcept{
        return IteratorMap<Key, T>{tree.head()};
    } 

    IteratorMap<Key, T> end() noexcept{
        return IteratorMap<Key, T>{tree.end()};
    }

    IteratorMap<Key, T> find(const Key&); 
    IteratorMap<Key, T> lower_bound(const Key&); 
    IteratorMap<Key, T> upper_bound(const Key&); 

private:
    Tree<Element<Key, T>> tree;
};

template<typename K, typename T>
class IteratorMap{
public:
    IteratorMap() = delete;
    IteratorMap(Element<K, T>& ele) noexcept
    :ptr(&ele)
    {}

    IteratorMap& operator++(){
        if(ptr->next == nullptr){
            throw std::range_error("Try to increment at end of IteratorMap");
        }
        ptr = ptr->next;
        return *this;
    }

    IteratorMap& operator--(){
        if(ptr->prev == nullptr){
            throw std::range_error("Try to decrement at begin of IteratorMap");
        }
        ptr = ptr->prev;
        return *this;
    }

    std::pair<const K&, T&>& operator*(){
        if(ptr == nullptr || ptr->kvpair == nullptr){
            throw std::out_of_range("Error in *IteratorMap");
        }
        return *(ptr->kvpair);
    }

    std::pair<const K&, T&>* operator->(){
        if(ptr == nullptr || ptr->kvpair == nullptr){
            throw std::out_of_range("Error in IteratorMap->");
        }
        return ptr->kvpair;
    }

    bool operator==(const IteratorMap& other) const noexcept{
        return ptr == other.ptr;
    }

    bool operator!=(const IteratorMap& other) const noexcept{
        return ptr != other.ptr;
    }

private:
    Element<K, T>* ptr;
};

#endif
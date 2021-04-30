#ifndef SET_H
#define SET_H

#include<cstddef>
#include<utility>
#include<stdexcept>

template<typename Key, template<typename, typename> typename Tree>
class Set{
private:
    class Iterator;
    class Compare;
    typedef Key Element;
public:
    std::pair<Iterator, bool> insert( const Element& value ){
        auto it = tree.find(value);
        if( it== tree.end()){
            it = tree.insert(value);
            return std::pair<Iterator, bool>{{this, it}, true};
        }
        return std::pair<Iterator, bool>{end(), false};
    }

    std::pair<Iterator, bool> insert( Element&& value ){
        auto it = tree.find(value);
        if( it== tree.end()){
            it = tree.insert(std::move(value));
            return std::pair<Iterator, bool>{{this, it}, true};
        }
        return std::pair<Iterator, bool>{end(), false};
    }

    Iterator begin(){
        return Iterator{this, tree.begin()};
    }

    Iterator end(){
        return Iterator{this, tree.end()};
    }

    size_t size() const noexcept{
        return tree.size();
    }

    void clear() noexcept{
        tree = Tree<Element, Compare>{};
    }

    Iterator erase(Iterator pos){
        if(this != pos.container){
            throw std::invalid_argument("Iterator of wrong Set");
        }
        auto it = tree.erase(pos.it);
        return Iterator{this, it};
    }

    size_t erase(const Key& key){
        auto it = tree.lower_bound(key);
        auto end = tree.end();
        size_t cnt = 0;
        for(; it != end && it->first == key; ++cnt){
            it = tree.erase(it);
        }
        return cnt;
    }

    size_t count(const Key& key){
        size_t c = 0;
        Iterator it = lower_bound(key);
        Iterator end = upper_bound(key);
        for(; it != end; ++it, ++c){}
        return c;
    }

    Iterator find(const Key& key){
        return Iterator{this, tree.find(key)};
    }

    std::pair<Iterator, Iterator> equal_range(const Key& key){
        return std::pair<Iterator, Iterator>{lower_bound(key), upper_bound(key)};
    }

    Iterator lower_bound(const Key& key){
        return Iterator{this, tree.lower_bound(key)};
    }

    Iterator upper_bound(const Key& key){
        return Iterator{this, tree.upper_bound(key)};
    }

private:
    Tree<Element, Compare> tree;
};

template<typename Key, template<typename, typename> typename Tree>
class Set<Key, Tree>::Iterator{
friend Set;
public:
    Iterator() = delete;

    Iterator(Set* container_, const typename Tree<Element, Compare>::Iterator& it_) noexcept
    :container(container_), it(it_)
    {}

    Iterator& operator++(){
        ++it;
        return *this;
    }

    Element& operator*(){
        return *it;
    }

    Element* operator->(){
        return it.operator->();
    }

    bool operator==(const Iterator& other) const noexcept{
        return container == other.container && it == other.it;
    }

    bool operator!=(const Iterator& other) const noexcept{
        return !operator==(other);
    }

private:
    Set* container;
    typename Tree<Element, Compare>::Iterator it;
};

template<typename Key, template<typename, typename> typename Tree>
class Set<Key, Tree>::Compare{
public:
    bool operator()(const Key& a, const Key& b){
        return a<b;
    }
};

#endif
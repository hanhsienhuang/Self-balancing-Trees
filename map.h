#ifndef MAP_H
#define MAP_H

#include<cstddef>
#include<utility>
#include<stdexcept>

template<typename Key, typename T, template<typename, typename> typename Tree>
class Map{
private:
    class Iterator;
    class Compare;
    typedef std::pair<const Key, T> Element;
public:
    T& operator[](const Key& key){
        auto it = tree.find(key);
        if( it== tree.end()){
            it = tree.insert(std::move(Element{key, T{}}));
        }
        return it->second;
    }
    
    T& at(const Key& key){
        auto it = tree.find(key);
        if( it== tree.end()){
            throw std::out_of_range("No key in map");
        }
        return it->second;
    }
    
    std::pair<Iterator, bool> insert( const Element& value ){
        auto it = tree.find(value->first);
        if( it== tree.end()){
            it = tree.insert(value);
            return std::pair<Iterator, bool>{{this, it}, true};
        }
        return std::pair<Iterator, bool>{end(), false};
    }

    std::pair<Iterator, bool> insert( Element&& value ){
        auto it = tree.find(value.first);
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
        if(this != pos.map){
            throw std::invalid_argument("Iterator of wrong map");
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

template<typename Key, typename T, template<typename, typename> typename Tree>
class Map<Key, T, Tree>::Iterator{
friend Map;
public:
    Iterator() = delete;

    Iterator(Map* map_, const typename Tree<Element, Compare>::Iterator& it_) noexcept
    :map(map_), it(it_)
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
        return map == other.map && it == other.it;
    }

    bool operator!=(const Iterator& other) const noexcept{
        return !operator==(other);
    }

private:
    Map* map;
    typename Tree<Element, Compare>::Iterator it;
};

template<typename Key, typename T, template<typename, typename> typename Tree>
class Map<Key, T, Tree>::Compare{
public:
    bool operator()(const Key& a, const Key& b){
        return a<b;
    }

    bool operator()(const Element& a, const Key& b){
        return a.first < b;
    }

    bool operator()(const Key& a, const Element& b){
        return a < b.first;
    }

    bool operator()(const Element& a, const Element& b){
        return a.first < b.first;
    }
};

#endif
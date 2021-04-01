#ifndef B_TREE_H
#define B_TREE_H

#include<utility>
#include<stdexcept>
#include<iostream>

template<typename T, typename Compare, int N>
class BTree{
protected:
    class Node;
    class Iterator;

public:

    BTree() 
    :root(nullptr), size_(0)
    {
        static_assert(N>=3, "N should be >= 3");
    }

    BTree(const BTree & other)
    :root(recursive_copy(other.root)), size_(other.size_)
    {}

    BTree(BTree && other) noexcept
    :root(other.root), size_(other.size_)
    {
        other.root = nullptr;
        other.size_ = 0;
    }

    BTree& operator=(const BTree & other){
        if(this != &other){
            recursive_delete(root);
            root = recursive_copy(other.root);
            size_ = other.size_;
        }
        return *this;
    }

    BTree& operator=(BTree && other) noexcept
    {
        std::swap(root, other.root);
        size_ = other.size_;
        return *this;
    }

    ~BTree() noexcept {
        recursive_delete(root);
    };

    size_t size() const noexcept{
        return size_;
    }

    size_t depth() const noexcept{
        if(root == nullptr){
            return 0;
        }
        Node* node = root;
        int d = 1;
        while(node != nullptr){
            node = node->children[0];
            ++d;
        }
        return d;
    }

    Iterator begin(){
        if(size_ == 0){
            return end();
        }
        Node* node = root;
        while(node->children[0] != nullptr){
            node = node->children[0];
        }
        return Iterator{node, 0};
    }

    Iterator end(){
        return Iterator{nullptr, 0};
    }

    Iterator insert(const T& element){
        return insert_(new T(element));
    }

    Iterator insert(T&& element){
        return insert_(new T(std::move(element)));
    }

    Iterator erase(Iterator& it){
        if(it == end()){
            throw std::out_of_range("Error when trying to erase end");
        }
        auto pair = get_iterator_internal(it);
        Node* node = pair.first;
        int index = pair.second;
        if(index<0 || index>=node->size){
            throw std::out_of_range("Bad iterator in erase");
        }
        --size_;
        delete node->pElements[index];
        node->pElements[index] = nullptr;

        Iterator next_it{it};
        ++next_it;
        pair = get_iterator_internal(next_it);
        Node* next_node = pair.first;
        //int next_index = pair.second;
        if(node->children[index+1] == nullptr){
            erase_node(node, index);
        }else{
            node->pElements[index] = next_node->pElements[0];
            erase_node(next_node, 0);
            std::swap(node, next_node);
            //next_index = index;
        }

        constexpr int min_size = (N-1)/2;
        while(node->size < min_size){
            if(node == root){
                if(node->size == 0){
                    root = node->children[0];
                    delete node;
                    if(root != nullptr){
                        root->parent = nullptr;
                        root->ip = 0;
                    }
                }
                break;
            }
            Node*parent = node->parent;
            int ip = node->ip;
            if(ip>0 && parent->children[ip-1]->size > min_size){
                Node*left = parent->children[ip-1];
                insert_node(node, 0, parent->pElements[ip-1], node->children[0]);
                parent->pElements[ip-1] = left->pElements[left->size-1];
                connect_node(node, left->children[left->size], 0);
                --left->size;
                break;
            }

            if(ip<parent->size && parent->children[ip+1]->size > min_size){
                Node*right = parent->children[ip+1];
                insert_node(node, node->size, parent->pElements[ip], right->children[0]);
                parent->pElements[ip] = right->pElements[0];
                connect_node(right, right->children[1], 0);
                erase_node(right, 0);
                break;
            }

            if(ip>0){
                merge_nodes(parent, ip-1);
            }else{
                merge_nodes(parent, ip);
            }
            node = parent;
        }
        return end();
    }
    
    template<typename K>
    Iterator find(const K& key){
        Node* node = root;
        while(node != nullptr){
            int i = search_lower_bound(node, key);
            if(i<node->size && !less(key, *node->pElements[i])){
                return Iterator{node,i};
            }
            node = node->children[i];
        }
        return end();
    }

    template<typename K>
    Iterator lower_bound(const K& key){
        if(root == nullptr) return end();
        Node* node = root;
        int index;
        while(true){
            index = search_lower_bound(node, key);
            if(node->children[index] == nullptr){
                break;
            }
            node = node->children[index];
        }
        while(node != nullptr && index==node->size){
            index = node->ip;
            node = node->parent;
        }
        return Iterator{node, index};
    }

    template<typename K>
    Iterator upper_bound(const K& key){
        if(root == nullptr) return end();
        Node* node = root;
        int index;
        while(true){
            index = search_upper_bound(node, key);
            if(node->children[index] == nullptr){
                break;
            }
            node = node->children[index];
        }
        while(node != nullptr && index==node->size){
            index = node->ip;
            node = node->parent;
        }
        return Iterator{node, index};
    }

protected:

    template<typename K>
    int search_lower_bound(Node* node, const K& key){
        if(node == nullptr){
            return 0;
        }
        int l=0, r=node->size;
        while(l!=r){
            int mid = (l+r)>>1;
            if(less(*node->pElements[mid], key)){
                l = mid + 1;
            }else{
                r = mid;
            }
        }
        return l;
    }
    
    template<typename K>
    int search_upper_bound(Node* node, const K& key){
        if(node == nullptr){
            return 0;
        }
        int l=0, r=node->size;
        while(l!=r){
            int mid = (l+r)>>1;
            if(less(key, *node->pElement[mid])){
                r = mid;
            }else{
                l = mid + 1;
            }
        }
        return l;
    }

    Node* split_node(Node* node, int index, T* insert_element, Node* insert_node){
        Node* new_node = new Node{};
        constexpr int num_left = (N+1)/2;
        constexpr int num_right = N/2;

        int higher_than_index = 1;

        T* pElement = nullptr;
        Node* pNode = nullptr;
        for(int i=N-1; i>=num_left; --i){
            if(i == index){
                pElement = insert_element;
                pNode = insert_node;
                higher_than_index = 0;
            }else{
                pElement = node->pElements[i-higher_than_index];
                pNode = node->children[i-higher_than_index+1];
            }
            new_node->pElements[i-num_left] = pElement;
            connect_node(new_node, pNode, i-num_left+1);
        }
        for(int i=num_left-1; i>=0; --i){
            if(higher_than_index == 0) break;
            if(i == index){
                pElement = insert_element;
                pNode = insert_node;
                higher_than_index = 0;
            }else{
                pElement = node->pElements[i-1];
                pNode = node->children[i];
            }
            node->pElements[i] = pElement;
            connect_node(node, pNode, i+1);
        }

        connect_node(new_node, node->children[num_left], 0);

        node->size = num_left-1;
        new_node->size = num_right;

        return new_node;
    }

    void connect_node(Node* parent, Node* child, int ip){
        if(parent != nullptr){
            parent->children[ip] = child;
        }
        if(child != nullptr){
            child->parent = parent;
            child->ip = ip;
        }
    }

    void insert_node(Node* node, int index, T* pElement, Node* right){
        if(node->size == N-1){
            throw std::out_of_range("Insert into a node already full");
        }
        if(index<0 || index>node->size){
            throw std::out_of_range("Insert index out of range");
        }

        for(int j=node->size; j>index; --j){
            connect_node(node, node->children[j], j+1);
            node->pElements[j] = node->pElements[j-1];
        }
        connect_node(node, right, index+1);
        node->pElements[index] = pElement;
        ++node->size;
    }

    void erase_node(Node* node, int index){
        for(int i=index+1; i<node->size; ++i){
            node->pElements[i-1] = node->pElements[i];
            connect_node(node, node->children[i+1], i);
        }
        --node->size;
    }

    void merge_nodes(Node* node, int index){
        Node* left = node->children[index], *right = node->children[index+1];
        int s = left->size;
        left->pElements[s] = node->pElements[index];
        connect_node(left, right->children[0], ++s);
        for(int j=0; j<right->size; ++j){
            left->pElements[s] = right->pElements[j];
            connect_node(left, right->children[j+1], ++s);
        }
        left->size += right->size+1;
        right->size = 0;
        delete right;
        erase_node(node, index);
    }

    Iterator insert_(T* pElement_){
        ++size_;
        Node* node = root;
        int index = 0;
        if(root == nullptr){
            root = new Node;
            node = root;
        }else{
            while(true){
                index = search_lower_bound(node, *pElement_);
                if(node->children[index] == nullptr) break;
                node = node->children[index];
            }
        }

        Node* right = nullptr;
        T* pElement = pElement_;
        Node* inserted_node = node;
        int inserted_index = index;
        while(true){
            if(node->size != N-1){
                insert_node(node, index, pElement, right);
                break;
            }
             
            if(node == root){
                root = new Node;
                root->children[0] = node;
                node->parent = root;
                node->ip = 0;
            }

            right = split_node(node, index, pElement, right);
            pElement = node->pElements[node->size];
            index = node->ip;
            node = node->parent;

            if(pElement == pElement_){
                inserted_node = node;
                inserted_index = index;
            }
        }
        return Iterator{inserted_node, inserted_index};
    }

    void connect(Node* parent, Node* child, int ip){
        if( parent!= nullptr){
            parent->children[ip] = child;
        }
        if(child != nullptr){
            child->parent = parent;
            child->ip = ip;
        }
    }

    std::pair<Node*, int> get_iterator_internal(Iterator & it){
        return std::pair<Node*, int>{it.node, it.index};
    }

    Node* recursive_copy(Node * other){
        if(other==nullptr){
            return nullptr;
        }
        Node* new_node = new Node(*other);
        for(int i=0; i <= other->size; ++i){
            new_node.children[i] = recursive_copy(other->children[i]);
        }
        return new_node;
    }

    void recursive_delete(Node * node) noexcept{
        if(node != nullptr){
            for(int i=0; i <= node->size; ++i){
                recursive_delete(node->children[i]);
            }
            delete node;
        }
    }

    Node* root;
    size_t size_;
    Compare less;
};

template<typename T, typename Compare, int N>
class BTree<T,Compare,N>::Node{
public:
    Node()
    {
        for(int i=0; i<N; ++i){
            children[i] = nullptr;
        }
        for(int i=0; i<N-1; ++i){
            pElements[i] = nullptr;
        }
    }

    Node(const Node& other)
    {
        size = other.size;
        for(int i=0; i<N; ++i){
            children[i] = nullptr;
        }
        for(int i=0; i<N-1; ++i){
            pElements[i] = nullptr;
        }
        for(int i=0; i<size; ++i){
            pElements[i] = new T(*other.pElements[i]);
        }
    }

    Node(Node&& other)
    {
        size = other.size;
        for(int i=0; i<N; ++i){
            children[i] = nullptr;
        }
        for(int i=0; i<N-1; ++i){
            pElements[i] = nullptr;
        }
        for(int i=0; i<size; ++i){
            std::swap(pElements[i], other.pElements[i]);
        }
    }

    Node& operator=(const Node& other){
        if(this!=&other){
            for(int i=0; i<N-1; ++i){
                delete pElements[i];
                pElements[i] = new T(*other.pElements[i]);
            }
            size = other.size;
        }
        return *this;
    }

    Node& operator=(Node&& other){
        for(int i=0; i<N-1; ++i){
            std::swap(pElements[i], other.pElements[i]);
        }
        size = other.size;
        return *this;
    }

    ~Node() noexcept{
        for(int i=0; i<size; ++i){
            delete pElements[i];
        }
    }

    Node* parent = nullptr;
    int ip = 0;
    Node* children[N];
    T* pElements[N-1];
    int size = 0;
};

template<typename T, typename Compare, int N>
class BTree<T,Compare, N>::Iterator{
    friend std::pair<Node*, int> BTree<T, Compare, N>::get_iterator_internal(Iterator &);
public:
    Iterator() = delete;

    Iterator(Node* node_, int index_)
    :node(node_), index(index_)
    {}

    Iterator& operator++(){
        if(node == nullptr){
            throw std::out_of_range("Trying to increment at end of iterator");
        }
        if(index < 0 || index >= node->size){
            throw std::out_of_range("Bad iterator");
        }
        ++index;

        if(node->children[index] != nullptr ){
            node = node->children[index];
            while(node->children[0] != nullptr){
                node = node->children[0];
            }
            index = 0;
        }else if(index == node->size ){
            do{
                index = node->ip;
                node = node->parent;
            }while(node!=nullptr && index==node->size);
        }

        return *this;
    }

    bool operator==(const Iterator& other)const{
        return node == other.node && index == other.index;
    }

    bool operator!=(const Iterator& other)const{
        return node != other.node || index != other.index;
    }

    const T& operator*() const{
        if(node == nullptr){
            throw std::out_of_range("Out of range in Tree");
        }
        if(index < 0 || index >= node->size){
            throw std::out_of_range("Bad iterator");
        }
        return *node->pElements[index];
    }

    T& operator*(){
        if(node == nullptr){
            throw std::out_of_range("Out of rang in Tree");
        }
        if(index < 0 || index >= node->size){
            throw std::out_of_range("Bad iterator");
        }
        return *node->pElements[index];
    }
private:
    Node* node;
    int index;
};

template<typename T, typename Compare>
using TwoThreeTree = BTree<T,Compare, 3>;

template<typename T, typename Compare>
using TwoThreeFourTree = BTree<T,Compare, 4>;
#endif

#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

#include<utility>
#include<stdexcept>

template<typename T, typename Compare>
class BinarySearchTree{
protected:
    class Node;
    class Iterator;


public:

    BinarySearchTree() 
    :root(nullptr), size_(0)
    {}

    BinarySearchTree(const BinarySearchTree & other)
    :root(recursive_copy(other.root)), size_(other.size_)
    {}

    BinarySearchTree(BinarySearchTree && other) noexcept
    :root(other.root), size_(other.size_)
    {
        other.root = nullptr;
        other.size_ = 0;
    }

    BinarySearchTree& operator=(const BinarySearchTree & other){
        if(this != &other){
            recursive_delete(root);
            root = recursive_copy(other.root);
            size_ = other.size_;
        }
        return *this;
    }

    BinarySearchTree& operator=(BinarySearchTree && other) noexcept
    {
        std::swap(root, other.root);
        size_ = other.size_;
        return *this;
    }

    ~BinarySearchTree() noexcept {
        recursive_delete(root);
    };

    size_t size() const noexcept{
        return size_;
    }

    size_t depth() const noexcept{
        return depth_recursive(root);
    }

    Iterator begin(){
        if(size_ == 0){
            return end();
        }
        Node* node = root;
        while(node->children[0] != nullptr){
            node = node->children[0];
        }
        return Iterator{node};
    }

    Iterator end(){
        return Iterator{nullptr};
    }

    Iterator insert(const T& element){
        return insert_(get_new_node(element));
    }

    Iterator insert(T&& element){
        return insert_(get_new_node(std::move(element)));
    }

    virtual Iterator erase(Iterator& it){
        if(it == end()){
            throw std::out_of_range("Error when trying to erase end");
        }
        // TODO: check same tree;

        --size_;
        Node* to_be_deleted = get_iterator_internal(it);
        if(to_be_deleted->children[1] == nullptr){
            ++it;
            if(to_be_deleted->parent == nullptr){
                root = to_be_deleted->children[0];
                to_be_deleted->children[0]->parent = nullptr;
            }else{
                to_be_deleted
                    ->parent
                    ->children[to_be_deleted->ip]
                    = to_be_deleted->children[0];
                to_be_deleted->children[0]->parent = to_be_deleted->parent;
                to_be_deleted->children[0]->ip = to_be_deleted->ip;
            }
        }else{
            Iterator next_it{it};
            Node* next = get_iterator_internal(++next_it);
            std::swap(to_be_deleted->pElement, next->pElement);
            to_be_deleted = next;
            Node* right = to_be_deleted->children[1];
            to_be_deleted
                ->parent
                ->children[to_be_deleted->ip]
                = right;
            if(right!=nullptr){
                right->ip = to_be_deleted->ip;
                right->parent = to_be_deleted->parent;
            }
        }

        delete to_be_deleted;
        return it;
    }
    
    template<typename K>
    Iterator find(const K& key){
        Node* node = root;
        while(node != nullptr){
            if(less(key, *(node->pElement))){
                node = node->children[0];
            }else if(less(*(node->pElement), key)){
                node = node->children[1];
            }else{
                return Iterator{node};
            }
        }
        return end();
    }

    template<typename K>
    Iterator lower_bound(const K& key){
        Node* node = root;
        while(node != nullptr){
            if(less(*(node->pElement), key)){
                node = node->children[1];
            } else{
                node = node->children[0];
            }
        }
        while(node != nullptr && node->ip == 1){
            node = node->parent;
        }
        return Iterator{node};
    }

    template<typename K>
    Iterator upper_bound(const K& key){
        Node* node = root;
        while(node != nullptr){
            if(less(key, *(node->pElement))){
                node = node->children[1];
            } else{
                node = node->children[0];
            }
        }
        while(node != nullptr && node->ip == 1){
            node = node->parent;
        }
        return Iterator{node};
    }

protected:
    virtual Iterator insert_(Node* node){
        if(root == nullptr){
            root = node;
            node->parent = nullptr;
            node->ip = 0;
        }else{
            Node* parent = root;
            int ip = less(*node->pElement, *parent->pElement)?0:1;
            while(parent->children[ip] != nullptr){
                parent = parent->children[ip];
                ip = less(*node->pElement, *parent->pElement)?0:1;
            }
            parent->children[ip] = node;
            node->parent = parent;
            node->ip = ip;
        }
        ++size_;
        return Iterator{node};
    }

    virtual Node* get_new_node(T element){
        return new Node{element};
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

    size_t depth_recursive(Node* node) const noexcept{
        if(node == nullptr){
            return 0;
        }
        size_t max = 0;
        for(int i=0; i<2; ++i){
            auto d = depth_recursive(node->children[i]);
            max = d>max?d:max;
        }
        return max + 1;
    }

    Node* get_iterator_internal(Iterator & it){
        return it.node;
    }

    Node* recursive_copy(Node * other){
        if(other==nullptr){
            return nullptr;
        }
        Node* new_node = new Node(*other);
        for(int i=0; i<2; ++i){
            new_node.children[i] = recursive_copy(other->children[i]);
        }
        return new_node;
    }

    void recursive_delete(Node * node) noexcept{
        if(node != nullptr){
            for(int i=0; i<2; ++i){
                recursive_delete(node->children[i]);
            }
            delete node;
        }
    }

    Node* root;
    size_t size_;
    Compare less;
};

template<typename T, typename Compare>
class BinarySearchTree<T,Compare>::Node{
public:
    Node()
    :pElement(nullptr)
    {}

    Node(const T& element_)
    :pElement(new T{element_})
    {}

    Node(T&& element_)
    :pElement(new T{std::move(element_)})
    {}

    virtual ~Node() noexcept{
        delete pElement;
    }

    void update(const T& e){
        delete pElement;
        pElement = new T(e);
    }

    void update(T&& e){
        delete pElement;
        pElement = new T(std::move(e));
    }

    Node* children[2] = {nullptr, nullptr};
    Node* parent = nullptr;
    int ip = 0;
    T* pElement;
};

template<typename T, typename Compare>
class BinarySearchTree<T,Compare>::Iterator{
    friend Node* BinarySearchTree<T, Compare>::get_iterator_internal(Iterator &);
public:
    Iterator() = delete;

    Iterator(Node* node_)
    :node(node_)
    {}

    Iterator& operator++(){
        if(node == nullptr){
            throw std::out_of_range("Trying to increment at end of iterator");
        }
        if(node->children[1] == nullptr ){
            int ip;
            do{
                ip = node->ip;
                node = node->parent;
            }while(node!=nullptr && ip==1);
        }else{
            node = node->children[1];
            while(node->children[0] != nullptr){
                node = node->children[0];
            }
        }
        return *this;
    }

    bool operator==(const Iterator& other)const{
        return node == other.node;
    }

    bool operator!=(const Iterator& other)const{
        return node != other.node;
    }

    const T& operator*() const{
        if(node == nullptr){
            throw std::out_of_range("Out of range in Tree");
        }
        return *node->pElement;
    }

    T& operator*(){
        if(node == nullptr){
            throw std::out_of_range("Out of rang in Tree");
        }
        return *node->pElement;
    }
private:
    Node* node;
};

#endif
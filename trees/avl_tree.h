#ifndef AVL_TREE_H
#define AVL_TREE_H

#include"binary_search_tree.h"
#include<utility>

template<typename T, typename Compare>
class AVLTree:
    public BinarySearchTree<T,Compare>
{
private:
    class Node;

    typedef BinarySearchTree<T, Compare> BST;
    using BST::root;
    using BST::size_;
    using BST::less;
    typedef typename BinarySearchTree<T, Compare>::Node BaseNode;

public:
    typedef typename BinarySearchTree<T, Compare>::Iterator Iterator;
    
    virtual Iterator erase(Iterator& it){
        if(it == this->end()){
            throw std::out_of_range("Error when trying to erase end");
        }
        // TODO: check same tree;

        --size_;
        BaseNode* node = this->get_iterator_internal(it);
        if(node->children[1] == nullptr){
            ++it;
            if(node->children[0] != nullptr){
                std::swap(node->pElement, node->children[0]->pElement);
                node = node->children[0];
            }
        }else{
            Iterator next_it{it};
            BaseNode* next = this->get_iterator_internal(++next_it);
            std::swap(node->pElement, next->pElement);
            node = next;
            if(node->children[1] != nullptr){
                std::swap(node->pElement, node->children[1]->pElement);
                node = node->children[1];
            }
        }

        if(node == root){
            delete node;
            root = nullptr;
            return it;
        }

        Node* parent = dynamic_cast<Node*>(node->parent);
        int ip = node->ip;
        delete node;
        parent->children[ip] = node = nullptr;
        while(parent != nullptr){
            if(parent->balance_factor == 0){
                parent->balance_factor = (ip==0?1:-1);
                break;
            }else if((ip == 0 && parent->balance_factor < 0) 
                    ||(ip == 1 && parent->balance_factor > 0)){
                parent->balance_factor = 0;
                node = parent;
            }else{
                int is = (ip+1)%2;
                Node* sibling = dynamic_cast<Node*>(parent->children[is]);
                
                if(sibling->balance_factor == 0){ 
                    if(parent == root) root = sibling;
                    this->connect(parent->parent, sibling, parent->ip);
                    this->connect(parent, sibling->children[ip], is);
                    this->connect(sibling, parent, ip);
                    sibling->balance_factor = -parent->balance_factor;
                    break;
                }else if(sibling->balance_factor == parent->balance_factor){
                    if(parent == root) root = sibling;
                    this->connect(parent->parent, sibling, parent->ip);
                    this->connect(parent, sibling->children[ip], is);
                    this->connect(sibling, parent, ip);
                    sibling->balance_factor = parent->balance_factor = 0;
                    node = sibling;
                }else{
                    Node* child = dynamic_cast<Node*>(sibling->children[ip]);
                    if(parent == root) root = child;
                    this->connect(parent->parent, child, parent->ip);
                    this->connect(sibling, child->children[is], ip);
                    this->connect(parent, child->children[ip], is);
                    this->connect(child, sibling, is);
                    this->connect(child, parent, ip);

                    sibling->balance_factor = 
                        (child->balance_factor==sibling->balance_factor)?(-sibling->balance_factor):0;
                    parent->balance_factor = 
                        (child->balance_factor==parent->balance_factor)?(-parent->balance_factor):0;
                    child->balance_factor = 0;
                    node = child;
                }
            }
            parent = dynamic_cast<Node*>(node->parent);
            ip = node->ip;
        }

        return it;
    }

private:
    Iterator insert_(BaseNode* node_){
        Node* node = dynamic_cast<Node*>(node_);
        if(root == nullptr){
            root = node;
            node->parent = nullptr;
            node->ip = 0;
        }else{
            BaseNode* parent = root;
            int ip = less(*node->pElement, *parent->pElement)?0:1;
            while(parent->children[ip] != nullptr){
                parent = parent->children[ip];
                ip = less(*node->pElement, *parent->pElement)?0:1;
            }
            this->connect(parent, node, ip);
        }
        ++size_;

        Node* parent = dynamic_cast<Node*>(node->parent);
        int ip = node->ip;
        while(parent!=nullptr){
            int iu = (ip+1)%2;
            if(parent->balance_factor == 0){
                parent->balance_factor = (ip==0?-1:1);
                node = parent;
            }else if((ip == 0 && parent->balance_factor > 0) 
                    ||(ip == 1 && parent->balance_factor < 0)){
                parent->balance_factor = 0;
                break;
            }else if(node->balance_factor == 0){ 
                if(parent == root) root = node;
                this->connect(parent->parent, node, parent->ip);
                this->connect(parent, node->children[iu], ip);
                this->connect(node, parent, iu);
                node->balance_factor = -parent->balance_factor;
            }else if(node->balance_factor == parent->balance_factor){
                if(parent == root) root = node;
                this->connect(parent->parent, node, parent->ip);
                this->connect(parent, node->children[iu], ip);
                this->connect(node, parent, iu);
                node->balance_factor = parent->balance_factor = 0;
                break;
            }else{
                Node* child = dynamic_cast<Node*>(node->children[iu]);
                this->connect(parent->parent, child, parent->ip);
                this->connect(node, child->children[ip], iu);
                this->connect(parent, child->children[iu], ip);
                this->connect(child, node, ip);
                this->connect(child, parent, iu);
                if(parent == root) root = child;

                node->balance_factor = 
                    (child->balance_factor==node->balance_factor)?(-node->balance_factor):0;
                parent->balance_factor = 
                    (child->balance_factor==parent->balance_factor)?(-parent->balance_factor):0;
                child->balance_factor = 0;
                break;
            }
            parent = dynamic_cast<Node*>(node->parent);
            ip = node->ip;
        }
        return Iterator{dynamic_cast<Node*>(node_)};
    }


    Node* get_new_node(T element){
        return new Node{element};
    }
};

template<typename T, typename Compare>
class AVLTree<T,Compare>::Node: public BinarySearchTree<T,Compare>::Node{
public:
    int balance_factor = 0;

    Node(const T& element_)
    :BinarySearchTree<T,Compare>::Node(element_)
    {}

    Node(T&& element_)
    :BinarySearchTree<T,Compare>::Node(std::move(element_))
    {}
    
    ~Node(){}
};
#endif
#ifndef RED_BLACK_TREE_H
#define RED_BLACK_TREE_H

#include"binary_search_tree.h"
#include<utility>

template<typename T, typename Compare>
class RedBlackTree:
    public BinarySearchTree<T,Compare>
{
private:
    class Node;
    enum struct Color: bool{ red, black};

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
        Node* node = dynamic_cast<Node*>(this->get_iterator_internal(it));
        Node* to_be_deleted = node;


        if(node->children[1] == nullptr){
            ++it;
            if(node == root){
                root = node->children[0];
                root->parent = nullptr;
            }else if(node->color == Color::red){
                node->parent->children[node->ip] = nullptr;
            }else if(node->children[0] != nullptr){
                this->connect(node->parent, node->children[0], node->ip);
                dynamic_cast<Node*>(node->children[0])->color = Color::black;
            }else{
                goto BALANCE;
            }
        }else{
            Iterator next_it{it};
            Node* next = dynamic_cast<Node*>(this->get_iterator_internal(++next_it));
            std::swap(to_be_deleted->pElement, next->pElement);
            node = next;
            to_be_deleted = node;
            if(node->color == Color::red){
                node->parent->children[node->ip] = nullptr;
            }else if(node->children[1] != nullptr){
                this->connect(node->parent, node->children[1], node->ip);
                dynamic_cast<Node*>(node->children[1])->color = Color::black;
            }else{
                goto BALANCE;
            }
        }
        delete to_be_deleted;
        return it;

    BALANCE:
        Node* parent = dynamic_cast<Node*>(node->parent);
        int ip = node->ip;
        parent->children[ip] = node = nullptr;
        while(parent != nullptr){
            int is = (ip+1)%2;
            Node* sibling = dynamic_cast<Node*>(parent->children[is]);
            Node* nephew_C = dynamic_cast<Node*>(sibling->children[ip]);
            Node* nephew_D = dynamic_cast<Node*>(sibling->children[is]);
            Color color_p = node_color(parent);
            Color color_c = node_color(nephew_C);
            Color color_d = node_color(nephew_D);
            if(sibling->color == Color::red){
                parent->color = Color::red;
                sibling->color = Color::black;

                this->connect(parent, nephew_C, is);
                this->connect(parent->parent, sibling, parent->ip);
                this->connect(sibling, parent, ip);
                if(parent == root){
                    root = sibling;
                }
            }else if(color_p == Color::black 
                    && color_c == Color::black 
                    && color_d == Color::black){
                sibling->color = Color::red;

                node = parent;
                parent = dynamic_cast<Node*>(node->parent);
                ip = node->ip;
            }else if(color_p == Color::red 
                    && color_c == Color::black 
                    && color_d == Color::black){
                parent->color = Color::black;
                sibling->color = Color::red;
                break;
            }else if(color_d == Color::black){
                sibling->color = Color::red;
                nephew_C->color = Color::black;

                this->connect(sibling, nephew_C->children[is], ip);
                this->connect(parent, nephew_C, is);
                this->connect(nephew_C, sibling, is);
            }else{
                nephew_D->color = Color::black;
                sibling->color = parent->color;
                parent->color = Color::black;

                this->connect(parent->parent, sibling, parent->ip);
                this->connect(parent, sibling->children[ip], is);
                this->connect(sibling, parent, ip);

                if(parent == root){
                    root = sibling;
                }
                break;
            }
        }

        delete to_be_deleted;
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
        Node* grandparent = (parent==nullptr)?nullptr:dynamic_cast<Node*>(parent->parent);
        int ig = parent==nullptr?0:parent->ip;
        while(parent!=nullptr){
            if(parent->color == Color::black) break;
            if(grandparent == nullptr){
                parent->color = Color::black;
                break;
            }
            Node* uncle = dynamic_cast<Node*>(grandparent->children[(ig+1)%2]);
            if(uncle != nullptr && uncle->color == Color::red){
                parent->color = Color::black;
                uncle->color = Color::black;
                grandparent->color = Color::red;
                node = grandparent;
                parent = dynamic_cast<Node*>(node->parent);
                ip = node->ip;
                grandparent = (parent==nullptr)?nullptr:dynamic_cast<Node*>(parent->parent);
                ig = parent==nullptr?0:parent->ip;
            }else{
                if(ip != ig){
                    this->connect(parent, node->children[ig], ip);
                    this->connect(node, parent, ig);
                    this->connect(grandparent, node, ig);

                    std::swap(node, parent);
                    ip = ig;
                }
                int iu = (ip+1)%2;

                if(grandparent == root){
                    root = parent;
                }
                
                this->connect(grandparent, parent->children[iu], ig);
                this->connect(grandparent->parent, parent, grandparent->ip);
                this->connect(parent, grandparent, iu);

                parent->color = Color::black;
                grandparent->color = Color::red;
                break;
            }
        }
        return Iterator{node};
    }

    Color node_color(Node* node){
        return (node==nullptr || node->color==Color::black)?Color::black:Color::red;
    }

    Node* get_new_node(T element){
        return new Node{element};
    }
};

template<typename T, typename Compare>
class RedBlackTree<T,Compare>::Node: public BinarySearchTree<T,Compare>::Node{
public:
    Color color = Color::red;

    Node(const T& element_)
    :BinarySearchTree<T,Compare>::Node(element_)
    {}

    Node(T&& element_)
    :BinarySearchTree<T,Compare>::Node(std::move(element_))
    {}
    
    ~Node(){}
};
#endif
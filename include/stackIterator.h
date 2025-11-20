#pragma once

#include <iostream>


template<typename T>
struct Node{
    T data;
    Node<T>* next;
    
    Node(const T& value) : data(value), next(nullptr) {}
};

template<typename T>
class StackIterator{
private:
    Node<T>* current;

public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;  
    
    explicit StackIterator(Node<T>* node = nullptr) : current(node) {}
        
    reference operator*() const { return current->data; }
    pointer operator->() const { return &current->data; }
        
    StackIterator& operator++() {
        current = current->next;
        return *this;
    }
        
    StackIterator operator++(int) {
        StackIterator temp = *this;
        ++(*this);
        return temp;
    }
        
    bool operator==(const StackIterator& other) const { return current == other.current; }
    bool operator!=(const StackIterator& other) const { return !(*this == other); }

};
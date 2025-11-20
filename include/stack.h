#pragma once

#include "customMemoryRes.h"
#include "stackIterator.h"

template <class T, class allocator_type>
    requires std::is_default_constructible_v<T> && 
             std::is_same_v<allocator_type, std::pmr::polymorphic_allocator<T>>
class Stack{
private:
    Node<T>* topVal = nullptr;
    std::size_t CurSize = 0;

    using node_allocator_type = std::pmr::polymorphic_allocator<Node<T>>;
    node_allocator_type polymorphic_allocator;

public:
    Stack(allocator_type alloc = {}) : polymorphic_allocator(alloc), topVal(nullptr), CurSize(0) {}
    Stack(std::pmr::memory_resource* resource) : polymorphic_allocator(resource), topVal(nullptr), CurSize(0) {}

    void push(T value){
        Node<T>* new_node = polymorphic_allocator.allocate(1);

        polymorphic_allocator.construct(new_node, value);
        new_node->next = topVal;
        topVal = new_node;
        ++CurSize;
    }

    void pop(){
        if(empty()){
            throw std::runtime_error("Pop from empty stack");
        }

        Node<T>* temp = topVal;
        topVal = topVal->next;

        polymorphic_allocator.destroy(temp);
        polymorphic_allocator.deallocate(temp, 1);
        --CurSize;
    }

    T& top() {
        if(empty()){
            throw std::runtime_error("Top from empty stack");
        }
        return topVal->data;
    }

    const T& top() const{
        if(empty()){
            throw std::runtime_error("Top from empty stack");
        }
        return topVal->data;
    }
    
    bool empty() const {return CurSize == 0;}

    size_t size() const{ return CurSize;}

    void clear(){
        while(!empty()){
            pop();
        }
    }

    ~Stack(){ 
        clear();
    }

    StackIterator<T> begin() {return StackIterator<T>(topVal);}
    StackIterator<T> end() {return StackIterator<T>(nullptr);}

};
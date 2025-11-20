#include "include/stack.h"

int main(){
    CustomMemoryResource custom_memory_resource;
    std::pmr::polymorphic_allocator<int> polymorphic_allocator(&custom_memory_resource);

    std::cout << "Test stack of int" << std::endl;

    Stack<int, std::pmr::polymorphic_allocator<int>> intStackFirst, intStackSecond;

    for(int i = 1; i <= 10; ++i){
        intStackFirst.push(i*i);
        intStackSecond.push(i+i);
    }

    std::cout << "top/pop method:" <<std::endl;
    for(int i = 1; i <= 10; ++i){
        std::cout << "i = " << i << " | " << "value in first stack: " << intStackFirst.top() << std::endl;
        intStackFirst.pop();
    }
    std::cout <<"---------------------" << std::endl;
    std::cout << "iterator method:" << std::endl;
    for(auto it = intStackSecond.begin(); it != intStackSecond.end(); ++it){
        std::cout << "value in second stack: " << *it << std::endl;
    }

    std::cout <<"---------------------" << std::endl;
    std::cout << "Test stack of struct" << std::endl;
    struct TestStructure{
        int first_val;
        int second_val;
    };

    Stack<TestStructure, std::pmr::polymorphic_allocator<TestStructure>> structStack;

    structStack.push({0, 1});
    structStack.push({2, 3});
    structStack.push({4, 5});

    size_t index = 1;
    for(auto it = structStack.begin(); it != structStack.end(); ++it){
        std::cout << index++ << " value in struct stack: " << std::endl << "frist = " << (*it).first_val << " | second = " << (*it).second_val << std::endl;
    }
}
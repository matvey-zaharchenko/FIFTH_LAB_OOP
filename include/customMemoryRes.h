#pragma once

#include <map>
#include <iostream>
#include <memory_resource>
#include <memory>
#include <cstdlib>
#include <stack>
#include <malloc.h>


class CustomMemoryResource: public std::pmr::memory_resource{
    std::map<void*, size_t> used_blocks;

public:
    ~CustomMemoryResource(){
        for (auto& [ptr, size]: used_blocks) {
            std::free(ptr);
        }
        used_blocks.clear();
    }

    void* do_allocate(size_t bytes, size_t aligment) override{
        void *ptr = std::aligned_alloc(aligment, bytes);
        if(!ptr){throw std::bad_alloc();}

        used_blocks[ptr] = bytes;
        return ptr;
    }

    void do_deallocate(void *ptr, size_t bytes, size_t aligment) override {
        if(ptr != nullptr){
            auto find_ptr = used_blocks.find(ptr);
            if(find_ptr != used_blocks.end()){
                std::free(ptr);
                used_blocks.erase(find_ptr);
            }else{
                throw std::logic_error("Incorrect block to deallocate");
            }
        }else{
            throw std::logic_error("Try to deallocate not allocated memory");
        }
    }

    bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override{
        return this == &other;
    }
};


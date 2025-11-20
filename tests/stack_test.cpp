#include <gtest/gtest.h>
#include <memory_resource>
#include <vector>
#include "stack.h"
#include "customMemoryRes.h"

// Тестовая структура
struct TestStructure {
    int first_val;
    int second_val;
    
    TestStructure(int f = 0, int s = 0) : first_val(f), second_val(s) {}
    
    bool operator==(const TestStructure& other) const {
        return first_val == other.first_val && second_val == other.second_val;
    }
};

// Тесты для CustomMemoryResource
TEST(CustomMemoryResourceTest, BasicAllocation) {
    CustomMemoryResource resource;
    
    void* ptr = resource.allocate(100, 8);
    EXPECT_NE(ptr, nullptr);
    EXPECT_NO_THROW(resource.deallocate(ptr, 100, 8));
}

// Тесты для Stack с простыми типами
TEST(StackIntTest, BasicOperations) {
    CustomMemoryResource resource;
    Stack<int, std::pmr::polymorphic_allocator<int>> stack(&resource);
    
    EXPECT_TRUE(stack.empty());
    EXPECT_EQ(stack.size(), 0);
    
    stack.push(10);
    stack.push(20);
    
    EXPECT_FALSE(stack.empty());
    EXPECT_EQ(stack.size(), 2);
    EXPECT_EQ(stack.top(), 20);
    
    stack.pop();
    EXPECT_EQ(stack.top(), 10);
    EXPECT_EQ(stack.size(), 1);
}

TEST(StackIntTest, LIFOBehavior) {
    CustomMemoryResource resource;
    Stack<int, std::pmr::polymorphic_allocator<int>> stack(&resource);
    
    for (int i = 1; i <= 3; ++i) {
        stack.push(i);
    }
    
    EXPECT_EQ(stack.top(), 3);
    stack.pop();
    EXPECT_EQ(stack.top(), 2);
    stack.pop();
    EXPECT_EQ(stack.top(), 1);
}

TEST(StackIntTest, EmptyStackExceptions) {
    CustomMemoryResource resource;
    Stack<int, std::pmr::polymorphic_allocator<int>> stack(&resource);
    
    EXPECT_THROW(stack.top(), std::runtime_error);
    EXPECT_THROW(stack.pop(), std::runtime_error);
}

// Тесты для Stack со сложными типами
TEST(StackComplexTest, TestStructureOperations) {
    CustomMemoryResource resource;
    Stack<TestStructure, std::pmr::polymorphic_allocator<TestStructure>> stack(&resource);
    
    TestStructure ts1(1, 10);
    TestStructure ts2(2, 20);
    
    stack.push(ts1);
    stack.push(ts2);
    
    EXPECT_EQ(stack.size(), 2);
    EXPECT_EQ(stack.top().first_val, 2);
    EXPECT_EQ(stack.top().second_val, 20);
    
    stack.pop();
    EXPECT_EQ(stack.top().first_val, 1);
    EXPECT_EQ(stack.top().second_val, 10);
}

TEST(StackComplexTest, DirectConstruction) {
    CustomMemoryResource resource;
    Stack<TestStructure, std::pmr::polymorphic_allocator<TestStructure>> stack(&resource);
    
    // Прямая конструкция объектов
    stack.push(TestStructure(5, 50));
    stack.push(TestStructure(6, 60));
    
    EXPECT_EQ(stack.size(), 2);
    EXPECT_EQ(stack.top().first_val, 6);
    EXPECT_EQ(stack.top().second_val, 60);
}

// Тесты для итераторов
TEST(StackIteratorTest, IteratorTraversal) {
    CustomMemoryResource resource;
    Stack<int, std::pmr::polymorphic_allocator<int>> stack(&resource);
    
    for (int i = 1; i <= 3; ++i) {
        stack.push(i * 10);
    }
    
    std::vector<int> expected = {30, 20, 10};
    std::vector<int> actual;
    
    for (auto it = stack.begin(); it != stack.end(); ++it) {
        actual.push_back(*it);
    }
    
    EXPECT_EQ(actual, expected);
}

TEST(StackIteratorTest, RangeBasedFor) {
    CustomMemoryResource resource;
    Stack<int, std::pmr::polymorphic_allocator<int>> stack(&resource);
    
    stack.push(100);
    stack.push(200);
    stack.push(300);
    
    std::vector<int> elements;
    for (const auto& elem : stack) {
        elements.push_back(elem);
    }
    
    EXPECT_EQ(elements, std::vector<int>({300, 200, 100}));
}

// Интеграционный тест
TEST(IntegrationTest, CompleteWorkflow) {
    CustomMemoryResource resource;
    Stack<TestStructure, std::pmr::polymorphic_allocator<TestStructure>> stack(&resource);
    
    // Заполнение
    for (int i = 0; i < 5; ++i) {
        stack.push(TestStructure(i, i * 10));
    }
    
    EXPECT_EQ(stack.size(), 5);
    EXPECT_EQ(stack.top().first_val, 4);
    EXPECT_EQ(stack.top().second_val, 40);
    
    // Итераторы
    std::vector<int> first_vals;
    for (const auto& item : stack) {
        first_vals.push_back(item.first_val);
    }
    
    EXPECT_EQ(first_vals, std::vector<int>({4, 3, 2, 1, 0}));
    
    // Очистка
    stack.clear();
    EXPECT_TRUE(stack.empty());
    EXPECT_EQ(stack.size(), 0);
}

// Тест на безопасность исключений
TEST(ExceptionSafetyTest, NoLeaksOnDestruction) {
    auto resource = std::make_unique<CustomMemoryResource>();
    auto stack = std::make_unique<Stack<int, std::pmr::polymorphic_allocator<int>>>(resource.get());
    
    for (int i = 0; i < 10; ++i) {
        stack->push(i);
    }
    
    // Не должно быть утечек памяти при разрушении
    EXPECT_NO_THROW(stack.reset());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
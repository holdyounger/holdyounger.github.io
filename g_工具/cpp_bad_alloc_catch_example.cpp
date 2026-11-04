#include <iostream>
#include <memory>
#include <new>
#include <vector>

int main() {
    std::cout << "=== C++ std::bad_alloc 异常捕获示例 ===\n\n";

    // 方法1：捕获 std::bad_alloc（推荐）
    std::cout << "【方法1】捕获 std::bad_alloc\n";
    try {
        // 尝试分配一个超大的内存块，必然失败
        size_t huge_size = 1000000000000ULL; // 1TB，远超系统可用内存
        int* big_array = new int[huge_size];
        
        // 如果能分配成功，这行代码会执行
        std::cout << "分配成功！大小: " << huge_size << "\n";
        delete[] big_array;
        
    } catch (const std::bad_alloc& e) {
        std::cout << "❌ 捕获到 std::bad_alloc 异常: " << e.what() << "\n";
        std::cout << "   原因: 系统内存不足，无法分配请求的内存空间。\n";
    }

    std::cout << "\n";

    // 方法2：捕获所有异常（通用方法）
    std::cout << "【方法2】捕获所有异常\n";
    try {
        // 使用 vector 尝试分配大量内存
        std::vector<int> huge_vector;
        huge_vector.reserve(1000000000000ULL);
        
        // 如果能分配成功，这行代码会执行
        std::cout << "Vector 分配成功！容量: " << huge_vector.capacity() << "\n";
        
    } catch (const std::exception& e) {
        std::cout << "❌ 捕获到 std::exception 异常: " << e.what() << "\n";
        std::cout << "   这包括了 std::bad_alloc、std::logic_error 等所有标准异常。\n";
    } catch (...) {
        std::cout << "❌ 捕获到未知异常（非标准异常）\n";
    }

    std::cout << "\n";

    // 方法3：使用 std::nothrow 避免异常（替代方案）
    std::cout << "【方法3】使用 std::nothrow 避免异常\n";
    try {
        size_t large_size = 100000000; // 100MB，可能成功也可能失败
        int* array = new (std::nothrow) int[large_size];
        
        if (array == nullptr) {
            std::cout << "⚠️ 分配失败，返回 nullptr（无异常抛出）\n";
            std::cout << "   建议：检查系统可用内存，或使用更小的分配请求。\n";
        } else {
            std::cout << "✅ 使用 nothrow 成功分配: " << large_size << " 个整数\n";
            delete[] array;
        }
        
    } catch (...) {
        // 这个 catch 块不会执行，因为 nothrow 不会抛出异常
        std::cout << "这个 catch 块不会被执行\n";
    }

    std::cout << "\n=== 示例结束 ===\n";
    return 0;
}
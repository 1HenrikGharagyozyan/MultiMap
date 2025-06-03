#include <iostream>
#include "MultiMap.h" // ваш заголовочный файл

int main() 
{
    MultiMap<int, std::string> mmap;

    // insert и emplace
    mmap.insert({ 1, "apple" });
    mmap.insert({ 1, "avocado" });
    mmap.insert({ 2, "banana" });
    mmap.emplace(3, "cherry");
    mmap.emplace(2, "blueberry");

    std::cout << "Initial MultiMap content:\n";
    for (const auto& [key, val] : mmap)
        std::cout << key << " -> " << val << '\n';

    // find, count
    std::cout << "\nCount of key 1: " << mmap.count(1) << '\n';
    std::cout << "Find key 2: " << mmap.find(2)->second << '\n';

    // equal_range
    std::cout << "\nValues with key 2:\n";
    auto range = mmap.equal_range(2);
    for (auto it = range.first; it != range.second; ++it)
        std::cout << it->first << " -> " << it->second << '\n';

    // erase by iterator (удалим первое вхождение key = 1)
    auto it = mmap.find(1);
    mmap.erase(it);

    std::cout << "\nAfter erasing one element with key 1:\n";
    for (const auto& [key, val] : mmap)
        std::cout << key << " -> " << val << '\n';

    // erase by key
    mmap.erase(2);

    std::cout << "\nAfter erasing all elements with key 2:\n";
    for (const auto& [key, val] : mmap)
        std::cout << key << " -> " << val << '\n';

    // merge
    MultiMap<int, std::string> other;
    other.emplace(4, "date");
    other.emplace(4, "dragonfruit");
    other.emplace(5, "elderberry");

    mmap.merge(std::move(other));

    std::cout << "\nAfter merging another MultiMap:\n";
    for (const auto& [key, val] : mmap)
        std::cout << key << " -> " << val << '\n';

    std::cout << "\nSize: " << mmap.size() << "\n";

    MultiMap<int, std::string> mmap1;

    mmap1.emplace(10, "ten");
    mmap1.emplace(20, "twenty");
    mmap1.emplace(10, "десять");

    std::cout << "Testing at():\n";

    try 
    {
        std::cout << "mmap.at(10): " << mmap1.at(10) << '\n'; // может вернуть первое значение с ключом 10
        std::cout << "mmap.at(20): " << mmap1.at(20) << '\n';
        std::cout << "mmap.at(30): " << mmap1.at(30) << '\n'; // должно вызвать исключение
    }
    catch (const std::out_of_range& e) 
    {
        std::cout << "Exception caught: " << e.what() << '\n';
    }


    return 0;
}

#pragma once

#include "RedBlackTree.h"

template <typename Key, typename T, typename Compare = std::less<Key>>
class MultiMap
{
private:
	using Tree = RedBlackTree<Key, T, Compare, true>;
	Tree _tree;

public:
    using key_type = Key;
    using mapped_type = T;
    using value_type = std::pair<const Key, T>;
    using size_type = typename Tree::size_type;
    using difference_type = typename Tree::difference_type;
    using key_compare = Compare;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;

    using iterator = typename Tree::iterator;
    using const_iterator = typename Tree::const_iterator;
    using reverse_iterator = typename Tree::reverse_iterator;
    using const_reverse_iterator = typename Tree::const_reverse_iterator;


    MultiMap();
    explicit MultiMap(const Compare& comp);
    MultiMap(std::initializer_list<value_type> init);
    MultiMap(const MultiMap&);
    MultiMap(MultiMap&&) noexcept;
    ~MultiMap();

    template<typename InputIt>
    MultiMap(InputIt first, InputIt last);

    MultiMap& operator=(const MultiMap&);
    MultiMap& operator=(MultiMap&&);
    
    bool empty() const noexcept;
    size_type size() const noexcept;
    void clear() noexcept;

    iterator insert(const value_type& value);
    iterator insert(value_type&& value);

    template<typename... Args>
    iterator emplace(Args&&... args);

    size_type erase(const key_type& key);
    iterator erase(iterator pos);
    iterator erase(iterator first, iterator last);

    void merge(MultiMap&& other);

    void swap(MultiMap& other) noexcept;

    mapped_type& at(const key_type& key);
    const mapped_type& at(const key_type& key) const;

    iterator find(const key_type& key);
    const_iterator find(const key_type& key) const;

    size_type count(const key_type& key) const;

    iterator lower_bound(const key_type& key);
    const_iterator lower_bound(const key_type& key) const;

    iterator upper_bound(const key_type& key);
    const_iterator upper_bound(const key_type& key) const;

    std::pair<iterator, iterator> equal_range(const key_type& key);
    std::pair<const_iterator, const_iterator> equal_range(const key_type& key) const;

    key_compare key_comp() const;
    
    const Tree& internal_tree() const;


    iterator begin() noexcept;
    iterator end() noexcept;
    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;

    reverse_iterator rbegin() noexcept;
    reverse_iterator rend() noexcept;
    const_reverse_iterator rbegin() const noexcept;
    const_reverse_iterator rend() const noexcept;
    const_reverse_iterator crbegin() const noexcept;
    const_reverse_iterator crend() const noexcept;

    template<typename K, typename U, typename C>
    friend bool operator==(const MultiMap<K, U, C>& lhs, const MultiMap<K, U, C>& rhs);

    template<typename K, typename U, typename C>
    friend bool operator!=(const MultiMap<K, U, C>& lhs, const MultiMap<K, U, C>& rhs);
}; 

template<typename Key, typename T, typename Compare>
inline MultiMap<Key, T, Compare>::MultiMap() = default;

template<typename Key, typename T, typename Compare>
inline MultiMap<Key, T, Compare>::MultiMap(const Compare& comp)
    : _tree(comp)
{
}

template<typename Key, typename T, typename Compare>
inline MultiMap<Key, T, Compare>::MultiMap(std::initializer_list<value_type> init)
{
    for (const auto& value : init)
        insert(value);
}

template<typename Key, typename T, typename Compare>
inline MultiMap<Key, T, Compare>::MultiMap(const MultiMap&) = default;

template<typename Key, typename T, typename Compare>
inline MultiMap<Key, T, Compare>::MultiMap(MultiMap&&) noexcept = default;

template<typename Key, typename T, typename Compare>
inline MultiMap<Key, T, Compare>::~MultiMap() = default;

template<typename Key, typename T, typename Compare>
inline MultiMap<Key, T, Compare>& MultiMap<Key, T, Compare>::operator=(const MultiMap&) = default;

template<typename Key, typename T, typename Compare>
inline MultiMap<Key, T, Compare>& MultiMap<Key, T, Compare>::operator=(MultiMap&&) = default;

template<typename Key, typename T, typename Compare>
template<typename InputIt>
inline MultiMap<Key, T, Compare>::MultiMap(InputIt first, InputIt last)
{
    for (; first != last; ++first)
        insert(*first);
}

template<typename Key, typename T, typename Compare>
bool MultiMap<Key, T, Compare>::empty() const noexcept
{
    return _tree.empty();
}

template<typename Key, typename T, typename Compare>
typename MultiMap<Key, T, Compare>::size_type MultiMap<Key, T, Compare>::size() const noexcept
{
    return _tree.size();
}

template<typename Key, typename T, typename Compare>
void MultiMap<Key, T, Compare>::clear() noexcept
{
    return _tree.clear();
}

template<typename Key, typename T, typename Compare>
typename MultiMap<Key, T, Compare>::iterator MultiMap<Key, T, Compare>::insert(const value_type& value)
{
    return _tree.insert(value).first;
}

template<typename Key, typename T, typename Compare>
typename MultiMap<Key, T, Compare>::iterator MultiMap<Key, T, Compare>::insert(value_type&& value)
{
    return _tree.insert(std::move(value)).first;
}

template<typename Key, typename T, typename Compare>
template<typename... Args>
typename MultiMap<Key, T, Compare>::iterator MultiMap<Key, T, Compare>::emplace(Args&&... args)
{
    return _tree.emplace(std::forward<Args>(args)...).first;
}

template<typename Key, typename T, typename Compare>
typename MultiMap<Key, T, Compare>::size_type MultiMap<Key, T, Compare>::erase(const key_type& key)
{
    size_type removed = 0;
    auto [first, last] = equal_range(key);
    while (first != last)
    {
        auto it = first++;
        _tree.erase(it->first);
        ++removed;
    }
    return removed;
}

template<typename Key, typename T, typename Compare>
typename MultiMap<Key, T, Compare>::iterator MultiMap<Key, T, Compare>::erase(iterator pos)
{
    auto next = std::next(pos);
    _tree.erase(pos->first);
    return next;
}

template<typename Key, typename T, typename Compare>
typename MultiMap<Key, T, Compare>::iterator MultiMap<Key, T, Compare>::erase(iterator first, iterator last)
{
    while (first != last)
        first = erase(first);
    return last;
}

template<typename Key, typename T, typename Compare>
inline void MultiMap<Key, T, Compare>::merge(MultiMap&& other)
{
    for (auto it = other.begin(); it != other.end(); ++it)
        insert(std::move(*it));
    other.clear();
}

template<typename Key, typename T, typename Compare>
inline void MultiMap<Key, T, Compare>::swap(MultiMap& other) noexcept
{
    std::swap(_tree, other._tree);
}

template<typename Key, typename T, typename Compare>
typename MultiMap<Key, T, Compare>::mapped_type& MultiMap<Key, T, Compare>::at(const key_type& key)
{
    auto it = find(key);
    if (it == end())
        throw std::out_of_range("MultiMap::at key not found");
    return it->second;
}

template<typename Key, typename T, typename Compare>
const typename MultiMap<Key, T, Compare>::mapped_type& MultiMap<Key, T, Compare>::at(const key_type& key) const
{
    auto it = find(key);
    if (it == end())
        throw std::out_of_range("MultiMap::at key not found");
    return it->second;
}

template<typename Key, typename T, typename Compare>
typename MultiMap<Key, T, Compare>::iterator MultiMap<Key, T, Compare>::find(const key_type& key)
{
    return _tree.find(key);
}

template<typename Key, typename T, typename Compare>
typename MultiMap<Key, T, Compare>::const_iterator MultiMap<Key, T, Compare>::find(const key_type& key) const
{
    return _tree.find(key);
}

template<typename Key, typename T, typename Compare>
typename MultiMap<Key, T, Compare>::size_type MultiMap<Key, T, Compare>::count(const key_type& key) const
{
    auto [first, last] = equal_range(key);
    return std::distance(first, last);
}

template<typename Key, typename T, typename Compare>
typename MultiMap<Key, T, Compare>::iterator MultiMap<Key, T, Compare>::lower_bound(const key_type& key)
{
    return _tree.lower_bound(key);
}

template<typename Key, typename T, typename Compare>
typename MultiMap<Key, T, Compare>::const_iterator MultiMap<Key, T, Compare>::lower_bound(const key_type& key) const
{
    return _tree.lower_bound(key);
}

template<typename Key, typename T, typename Compare>
typename MultiMap<Key, T, Compare>::iterator MultiMap<Key, T, Compare>::upper_bound(const key_type& key)
{
    return _tree.upper_bound(key);
}

template<typename Key, typename T, typename Compare>
typename MultiMap<Key, T, Compare>::const_iterator MultiMap<Key, T, Compare>::upper_bound(const key_type& key) const
{
    return _tree.upper_bound(key);
}

template<typename Key, typename T, typename Compare>
std::pair<typename MultiMap<Key, T, Compare>::iterator, typename MultiMap<Key, T, Compare>::iterator> 
            MultiMap<Key, T, Compare>::equal_range(const key_type& key)
{
    return _tree.equal_range(key);
}

template<typename Key, typename T, typename Compare>
std::pair<typename MultiMap<Key, T, Compare>::const_iterator, typename MultiMap<Key, T, Compare>::const_iterator> 
            MultiMap<Key, T, Compare>::equal_range(const key_type& key) const
{
    return _tree.equal_range(key);
}

template<typename Key, typename T, typename Compare>
typename MultiMap<Key, T, Compare>::key_compare MultiMap<Key, T, Compare>::key_comp() const
{
    return Compare();
}

template<typename Key, typename T, typename Compare>
inline const typename MultiMap<Key, T, Compare>::Tree& MultiMap<Key, T, Compare>::internal_tree() const
{
    return _tree;
}

template<typename Key, typename T, typename Compare>
typename MultiMap<Key, T, Compare>::iterator MultiMap<Key, T, Compare>::begin() noexcept
{
    return _tree.begin();
}

template<typename Key, typename T, typename Compare>
typename MultiMap<Key, T, Compare>::iterator MultiMap<Key, T, Compare>::end() noexcept
{
    return _tree.end();
}

template<typename Key, typename T, typename Compare>
typename MultiMap<Key, T, Compare>::const_iterator MultiMap<Key, T, Compare>::begin() const noexcept
{
    return _tree.begin();
}

template<typename Key, typename T, typename Compare>
typename MultiMap<Key, T, Compare>::const_iterator MultiMap<Key, T, Compare>::end() const noexcept
{
    return _tree.end();
}

template<typename Key, typename T, typename Compare>
typename MultiMap<Key, T, Compare>::const_iterator MultiMap<Key, T, Compare>::cbegin() const noexcept
{
    return _tree.cbegin();
}

template<typename Key, typename T, typename Compare>
typename MultiMap<Key, T, Compare>::const_iterator MultiMap<Key, T, Compare>::cend() const noexcept
{
    return _tree.cend();
}

template<typename Key, typename T, typename Compare>
typename MultiMap<Key, T, Compare>::reverse_iterator MultiMap<Key, T, Compare>::rbegin() noexcept
{
    return _tree.rbegin();
}

template<typename Key, typename T, typename Compare>
typename MultiMap<Key, T, Compare>::reverse_iterator MultiMap<Key, T, Compare>::rend() noexcept
{
    return _tree.rend();
}

template<typename Key, typename T, typename Compare>
typename MultiMap<Key, T, Compare>::const_reverse_iterator MultiMap<Key, T, Compare>::rbegin() const noexcept
{
    return _tree.rbegin();
}

template<typename Key, typename T, typename Compare>
typename MultiMap<Key, T, Compare>::const_reverse_iterator MultiMap<Key, T, Compare>::rend() const noexcept
{
    return _tree.rend();
}

template<typename Key, typename T, typename Compare>
typename MultiMap<Key, T, Compare>::const_reverse_iterator MultiMap<Key, T, Compare>::crbegin() const noexcept
{
    return _tree.crbegin();
}

template<typename Key, typename T, typename Compare>
typename MultiMap<Key, T, Compare>::const_reverse_iterator MultiMap<Key, T, Compare>::crend() const noexcept
{
    return _tree.crend();
}

template<typename K, typename U, typename C>
inline bool operator==(const MultiMap<K, U, C>& lhs, const MultiMap<K, U, C>& rhs)
{
    return lhs._tree == rhs._tree;;
}

template<typename K, typename U, typename C>
inline bool operator!=(const MultiMap<K, U, C>& lhs, const MultiMap<K, U, C>& rhs)
{
    return !(lhs == rhs);
}

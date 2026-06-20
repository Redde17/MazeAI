#pragma once
#include <queue>
template<
    class T,
    class Container = std::vector<T>,
    class Compare = std::less<typename Container::value_type>
> class searchable_priority_queue : public std::priority_queue<T, Container, Compare>
{
public:
    explicit searchable_priority_queue(const Compare& compare = Compare())
        : std::priority_queue<T, Container, Compare>(compare)
    {
    }

    typedef typename
        std::priority_queue<
        T,
        Container,
        Compare>::container_type::const_iterator const_iterator;

    //checks if the element is inside the priority queue 
    bool check(const T& val) const
    {
        auto first = this->c.cbegin();
        auto last = this->c.cend();
        while (first != last) {
            if (*first == val) return true;
            ++first;
        }
        return false;
    }
};
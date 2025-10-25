#pragma once
#include <vector>
#include <functional>

template<typename T>
class EasyArray
{
public:
    void Add(const T& element)
    {
        m_Elements.emplace_back(std::move(element));
    }

    template<typename Pred>
    void Remove(Pred&& predicate)
    {
        std::erase_if(m_Elements, std::forward<Pred>(predicate));
    }

    void Remove(const T& element)
    {
        std::erase(m_Elements, element);
    }

    uint32_t GetSize() const 
    {
        return m_Elements.size();
    }

    void Empty() 
    {
        m_Elements.clear();
    }

    auto begin() 
    {
        return m_Elements.begin();
    }

    auto end()
    {
        return m_Elements.end();
    }

    std::vector<T>& GetContainer() 
    {
        return m_Elements;
    }

private:
    std::vector<T> m_Elements;
};
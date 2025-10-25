#pragma once

#include <queue>
#include "Events.h"
#include <unordered_map>
#include <vector>
#include <functional>
#include <typeindex>

template<typename T>
using EventCallback = std::function<void(const T&)>;

class EventBus 
{
public:
    static EventBus* Get();

    template<typename T>
    void Subscribe(EventCallback<T> callback);

    template<typename T>
    void Fire(const T& event);

private:
    struct CallbackListBase 
    {
    public:
        virtual ~CallbackListBase() = default;
    };

    template<typename T>
    struct CallbackList : CallbackListBase 
    {
    public:
        std::vector<EventCallback<T>> Callbacks;
    };

    std::unordered_map<std::type_index, std::unique_ptr<CallbackListBase>> m_EventMap;
};

template <typename T>
inline void EventBus::Subscribe(EventCallback<T> callback)
{
    std::type_index type = typeid(T);
    if (m_EventMap.contains(type))
    {
        auto* list = static_cast<CallbackList<T>*>(m_EventMap[type].get());
        list->Callbacks.emplace_back(std::move(callback));
    }
    else 
    {
        m_EventMap[type] = std::make_unique<CallbackList<T>>();
    }
}

template <typename T>
inline void EventBus::Fire(const T& event)
{
    std::type_index type = typeid(T);
    if (m_EventMap.contains(type))
    {
        auto* list = static_cast<CallbackList<T>*>(m_EventMap[type].get());
        for (auto& callback : list->Callbacks)
        {
            callback(event);
        }
    }
}

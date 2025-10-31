#pragma once

#include <mutex>
#include <functional>

struct Task 
{
    std::function<void()> Callback;
};
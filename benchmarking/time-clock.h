#include <iostream>
#include <chrono>
#include <utility>
#include <functional>

// Use std::chrono::steady_clock for measuring time intervals, 
// as it is monotonic and not affected by system time changes.
using Clock = std::chrono::steady_clock;

template<typename F, typename... Args>
double measureFunctionTime(F&& func, Args&&... args) 
{
    auto start = Clock::now();

    // Invoke the function using perfect forwarding
    std::forward<F>(func)(std::forward<Args>(args)...);

    auto end = Clock::now();
    
    // Calculate the duration and cast to milliseconds
    std::chrono::duration<double, std::milli> duration = end - start;

    return duration.count();
}
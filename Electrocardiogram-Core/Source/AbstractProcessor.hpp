#pragma once
#include <string>
#include <vector>

template<class T, class U>
class AbstractProcessor
{
public:
    virtual ~AbstractProcessor() = default;
    std::vector<std::string> exportToCsv(const U& obj)
    {
        return static_cast<T*>(this)->exportToCsvImpl(obj);
    }

protected:
    [[nodiscard]] virtual std::vector<std::string> exportToCsvImpl(const U& obj) const = 0;
};

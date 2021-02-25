#pragma once
#include <vector>

class Signal
{
public:
    Signal(std::vector<double_t> readingData);
    virtual ~Signal() = default;
    [[nodiscard]] const std::vector<double_t>& readings() const;
    [[nodiscard]] size_t count() const;
    [[nodiscard]] double_t minimum() const;
    [[nodiscard]] double_t maximum() const;
    [[nodiscard]] double_t sum() const;
    [[nodiscard]] double_t average() const;
    [[nodiscard]] double_t standardDeviation() const;

    Signal operator+(double_t value) const;
    Signal operator-(double_t value) const;
    Signal operator*(double_t value) const;
    Signal& operator+=(double_t value);
    Signal& operator-=(double_t value);
    Signal& operator*=(double_t value);

protected:
    std::vector<double_t> readingData;
    int32_t signalRangeRawMax = std::numeric_limits<int32_t>::max();

    [[nodiscard]] double_t variance() const;
};

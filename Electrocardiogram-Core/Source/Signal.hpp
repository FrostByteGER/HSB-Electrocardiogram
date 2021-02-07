#pragma once
#include <string>
#include <vector>

class Signal
{
public:
    Signal(std::vector<uint16_t> readings);
    virtual ~Signal() = default;
    size_t Count() const;
    uint16_t Minimum() const;
    uint16_t Maximum() const;
    uint64_t Sum() const;
    double_t Average() const;
    double_t StandardDeviation() const;
    std::vector<uint16_t> SmoothReadings(int16_t windowSize) const;

    Signal operator+(uint16_t value) const;
    Signal operator-(uint16_t value) const;
    Signal operator*(uint16_t value) const;
    Signal& operator+=(uint16_t value);
    Signal& operator-=(uint16_t value);
    Signal& operator*=(uint16_t value);

    virtual std::vector<std::string> Export() const;

protected:
    std::vector<uint16_t> readings;

    double_t Variance() const;
};

#pragma once
#include <string>
#include <vector>

class Signal
{
public:
    Signal(std::vector<uint16_t> readings);
    virtual ~Signal() = default;
    [[nodiscard]] size_t Count() const;
    [[nodiscard]] uint16_t Minimum() const;
    [[nodiscard]] uint16_t Maximum() const;
    [[nodiscard]] uint64_t Sum() const;
    [[nodiscard]] double_t Average() const;
    [[nodiscard]] double_t StandardDeviation() const;
    [[nodiscard]] std::vector<uint16_t> SmoothReadings(int16_t windowSize) const;

    Signal operator+(uint16_t value) const;
    Signal operator-(uint16_t value) const;
    Signal operator*(uint16_t value) const;
    Signal& operator+=(uint16_t value);
    Signal& operator-=(uint16_t value);
    Signal& operator*=(uint16_t value);

    [[nodiscard]] virtual std::vector<std::string> Export() const;

protected:
    std::vector<uint16_t> readings;

    [[nodiscard]] double_t Variance() const;
};

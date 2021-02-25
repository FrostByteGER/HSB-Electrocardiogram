#pragma once
#include <string>
#include <vector>

class Signal
{
public:
    Signal(std::vector<double_t> readings);
    virtual ~Signal() = default;
    [[nodiscard]] std::vector<double_t> Readings() const;
    [[nodiscard]] size_t Count() const;
    [[nodiscard]] double_t Minimum() const;
    [[nodiscard]] double_t Maximum() const;
    [[nodiscard]] double_t Sum() const;
    [[nodiscard]] double_t Average() const;
    [[nodiscard]] double_t StandardDeviation() const;
    [[nodiscard]] std::vector<double_t> SmoothReadings(int32_t windowSize) const;

    Signal operator+(double_t value) const;
    Signal operator-(double_t value) const;
    Signal operator*(double_t value) const;
    Signal& operator+=(double_t value);
    Signal& operator-=(double_t value);
    Signal& operator*=(double_t value);

    [[nodiscard]] virtual std::vector<std::string> Export() const;

protected:
    std::vector<double_t> readings;

    [[nodiscard]] double_t Variance() const;
};

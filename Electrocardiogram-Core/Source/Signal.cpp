#include "Signal.hpp"

#include <algorithm>
#include <complex>
#include <numeric>
#include <utility>

Signal::Signal(std::vector<double_t> readings) : readings{std::move(readings)}
{
    if (this->readings.empty())
        throw std::invalid_argument("Size of parameter \"readings\" must be greater than 0!");
}

std::vector<double_t> Signal::Readings() const
{
    return readings;
}

size_t Signal::Count() const
{
    return readings.size();
}

double_t Signal::Minimum() const
{
    return *std::min_element(readings.begin(), readings.end());
}

double_t Signal::Maximum() const
{
    return *std::max_element(readings.begin(), readings.end());
}

double_t Signal::Sum() const
{
    return std::accumulate(readings.begin(), readings.end(), 0.0);
}

double_t Signal::Average() const
{
    return Sum() / static_cast<double_t>(Count());
}

double_t Signal::StandardDeviation() const
{
    return std::sqrt(Variance());
}

std::vector<double_t> Signal::SmoothReadings(const int32_t windowSize) const
{
    const int32_t totalWindowSize = windowSize * 2 + 1;
    std::vector<double_t> smoothedReadings;
    smoothedReadings.reserve(readings.size());
    for (size_t idx = 0; idx < readings.size(); ++idx)
    {
        double_t sum = 0;
        for (int32_t windowIdx = -windowSize; windowIdx < windowSize + 1; ++windowIdx)
        {
            const int32_t offsetIdx = static_cast<int32_t>(idx) + windowIdx;
            const int32_t currentIdx = std::clamp(offsetIdx, 0, static_cast<int32_t>(readings.size()) - 1);
            sum += readings[currentIdx];
        }
        const double_t average = sum / totalWindowSize;
        smoothedReadings.push_back(average);
    }
    return smoothedReadings;
}

Signal Signal::operator+(double_t value) const
{
    Signal signal(*this);
    std::transform(signal.readings.begin(), signal.readings.end(), signal.readings.begin(), [value](const double_t i) {return i + value; });
    return signal;
}

Signal Signal::operator-(double_t value) const
{
    Signal signal(*this);
    std::transform(signal.readings.begin(), signal.readings.end(), signal.readings.begin(), [value](const double_t i) {return i - value; });
    return signal;
}

Signal Signal::operator*(double_t value) const
{
    Signal signal(*this);
    std::transform(signal.readings.begin(), signal.readings.end(), signal.readings.begin(), [value](const double_t i) {return i * value; });
    return signal;
}

Signal& Signal::operator+=(double_t value)
{
    std::transform(this->readings.begin(), this->readings.end(), this->readings.begin(), [value](const double_t i) {return i + value; });
    return *this;
}

Signal& Signal::operator-=(double_t value)
{
    std::transform(this->readings.begin(), this->readings.end(), this->readings.begin(), [value](const double_t i) {return i - value; });
    return *this;
}

Signal& Signal::operator*=(double_t value)
{
    std::transform(this->readings.begin(), this->readings.end(), this->readings.begin(), [value](const double_t i) {return i * value; });
    return *this;
}

std::vector<std::string> Signal::Export() const
{
    std::vector<std::string> exportData;
    exportData.reserve(readings.size());
    for (auto e : readings)
    {
        exportData.push_back(std::to_string(e));
    }
    return exportData;
}

double_t Signal::Variance() const
{
    const double_t average = Average();
    const double_t result =  std::accumulate(readings.begin(), readings.end(), 0.0, [average](const double_t sum, const double_t value)
    {
        const double_t tempResult = value - average;
        return sum + tempResult * tempResult;
    });

    return result / static_cast<double_t>(Count());
}

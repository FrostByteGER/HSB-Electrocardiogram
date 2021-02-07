#include "Signal.hpp"

#include <algorithm>
#include <complex>
#include <numeric>
#include <utility>

Signal::Signal(std::vector<uint16_t> readings) : readings{std::move(readings)}
{
    if (this->readings.empty())
        throw std::invalid_argument("Size of parameter \"readings\" must be greater than 0!");
}

size_t Signal::Count() const
{
    return readings.size();
}

uint16_t Signal::Minimum() const
{
    // TODO: Test if none found
    return *std::min_element(readings.begin(), readings.end());
}

uint16_t Signal::Maximum() const
{
    return *std::max_element(readings.begin(), readings.end());
}

uint64_t Signal::Sum() const
{
    return std::accumulate(readings.begin(), readings.end(), 0);
}

double_t Signal::Average() const
{
    return static_cast<double_t>(Sum()) / Count();
}

double_t Signal::StandardDeviation() const
{
    return std::sqrt(Variance());
}

std::vector<uint16_t> Signal::SmoothReadings(const int16_t windowSize) const
{
    const uint16_t totalWindowSize = windowSize * 2 + 1;
    std::vector<uint16_t> smoothedReadings;
    smoothedReadings.reserve(readings.size());
    for (size_t idx = 0; idx < readings.size(); ++idx)
    {
        uint64_t sum = 0;
        for (int16_t windowIdx = -windowSize; windowIdx < windowSize + 1; ++windowIdx)
        {
            const int64_t offsetIdx = static_cast<int64_t>(idx) + windowIdx;
            const auto currentIdx = std::clamp(offsetIdx, 0LL, static_cast<int64_t>(readings.size()) - 1LL);
            sum += readings[currentIdx];
        }
        // Its okay to cast the result as the average can never be above the max of uint16
        const auto average = static_cast<uint16_t>(sum / totalWindowSize);
        smoothedReadings.push_back(average);
    }
    return smoothedReadings;
}

Signal Signal::operator+(uint16_t value) const
{
    Signal signal(*this);
    std::transform(signal.readings.begin(), signal.readings.end(), signal.readings.begin(), [value](const uint16_t i) {return i + value; });
    return signal;
}

Signal Signal::operator-(uint16_t value) const
{
    Signal signal(*this);
    std::transform(signal.readings.begin(), signal.readings.end(), signal.readings.begin(), [value](const uint16_t i) {return i - value; });
    return signal;
}

Signal Signal::operator*(uint16_t value) const
{
    Signal signal(*this);
    std::transform(signal.readings.begin(), signal.readings.end(), signal.readings.begin(), [value](const uint16_t i) {return i * value; });
    return signal;
}

Signal& Signal::operator+=(uint16_t value)
{
    std::transform(this->readings.begin(), this->readings.end(), this->readings.begin(), [value](const uint16_t i) {return i + value; });
    return *this;
}

Signal& Signal::operator-=(uint16_t value)
{
    std::transform(this->readings.begin(), this->readings.end(), this->readings.begin(), [value](const uint16_t i) {return i - value; });
    return *this;
}

Signal& Signal::operator*=(uint16_t value)
{
    std::transform(this->readings.begin(), this->readings.end(), this->readings.begin(), [value](const uint16_t i) {return i * value; });
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
    const double_t result =  std::accumulate(readings.begin(), readings.end(), 0.0, [average](const double_t sum, const uint16_t value)
    {
        const double_t tempResult = value - average;
        return sum + tempResult * tempResult;
    });

    return result / Count();
}

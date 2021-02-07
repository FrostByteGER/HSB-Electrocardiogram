#include "Signal.hpp"

#include <algorithm>
#include <complex>
#include <numeric>
#include <utility>

Signal::Signal(std::vector<uint16_t> readings) : _readings{std::move(readings)}
{
    if (_readings.empty())
        throw std::invalid_argument("Size of parameter \"readings\" must be greater than 0!");
}

size_t Signal::Count() const
{
    return _readings.size();
}

uint16_t Signal::Minimum() const
{
    // TODO: Test if none found
    return *std::min_element(_readings.begin(), _readings.end());
}

uint16_t Signal::Maximum() const
{
    return *std::max_element(_readings.begin(), _readings.end());
}

uint64_t Signal::Sum() const
{
    return std::accumulate(_readings.begin(), _readings.end(), 0);
}

double_t Signal::Average() const
{
    return static_cast<double_t>(Sum()) / Count();
}

double_t Signal::StandardDeviation() const
{
    return std::sqrt(Variance());
}

Signal Signal::Smooth() const
{
    Signal smoothedSignal(*this);

    return smoothedSignal;
}

Signal Signal::operator+(uint16_t value) const
{
    Signal signal(*this);
    std::transform(signal._readings.begin(), signal._readings.end(), signal._readings.begin(), [value](const uint16_t i) {return i + value; });
    return signal;
}

Signal Signal::operator-(uint16_t value) const
{
    Signal signal(*this);
    std::transform(signal._readings.begin(), signal._readings.end(), signal._readings.begin(), [value](const uint16_t i) {return i - value; });
    return signal;
}

Signal Signal::operator*(uint16_t value) const
{
    Signal signal(*this);
    std::transform(signal._readings.begin(), signal._readings.end(), signal._readings.begin(), [value](const uint16_t i) {return i * value; });
    return signal;
}

Signal& Signal::operator+=(uint16_t value)
{
    std::transform(this->_readings.begin(), this->_readings.end(), this->_readings.begin(), [value](const uint16_t i) {return i + value; });
    return *this;
}

Signal& Signal::operator-=(uint16_t value)
{
    std::transform(this->_readings.begin(), this->_readings.end(), this->_readings.begin(), [value](const uint16_t i) {return i - value; });
    return *this;
}

Signal& Signal::operator*=(uint16_t value)
{
    std::transform(this->_readings.begin(), this->_readings.end(), this->_readings.begin(), [value](const uint16_t i) {return i * value; });
    return *this;
}

std::vector<std::string> Signal::Export() const
{
    std::vector<std::string> exportData(_readings.size());
    for (auto e : _readings)
    {
        exportData.push_back(std::to_string(e));
    }
    return exportData;
}

double_t Signal::Variance() const
{
    const double_t average = Average();
    const double_t result =  std::accumulate(_readings.begin(), _readings.end(), 0.0, [average](const double_t sum, const uint16_t value)
    {
        const double_t tempResult = value - average;
        return sum + tempResult * tempResult;
    });

    return result / Count();
}

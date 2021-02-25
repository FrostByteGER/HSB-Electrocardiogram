#include "Signal.hpp"

#include <algorithm>
#include <complex>
#include <numeric>
#include <utility>

Signal::Signal(std::vector<double_t> readingData) : readingData{std::move(readingData)}
{
    if (this->readingData.empty())
        throw std::invalid_argument("Size of parameter \"readingData\" must be greater than 0!");
}

const std::vector<double_t>& Signal::readings() const
{
    return readingData;
}

size_t Signal::count() const
{
    return readingData.size();
}

double_t Signal::minimum() const
{
    return *std::min_element(readingData.begin(), readingData.end());
}

double_t Signal::maximum() const
{
    return *std::max_element(readingData.begin(), readingData.end());
}

double_t Signal::sum() const
{
    return std::accumulate(readingData.begin(), readingData.end(), 0.0);
}

double_t Signal::average() const
{
    return sum() / static_cast<double_t>(count());
}

double_t Signal::standardDeviation() const
{
    return std::sqrt(variance());
}

double_t Signal::variance() const
{
    const double_t avg = average();
    const double_t result = std::accumulate(readingData.begin(), readingData.end(), 0.0, [avg](const double_t sum, const double_t value)
        {
            const double_t tempResult = value - avg;
            return sum + tempResult * tempResult;
        });

    return result / static_cast<double_t>(count());
}


Signal Signal::operator+(double_t value) const
{
    Signal signal(*this);
    std::transform(signal.readingData.begin(), signal.readingData.end(), signal.readingData.begin(), [value](const double_t i) {return i + value; });
    return signal;
}

Signal Signal::operator-(double_t value) const
{
    Signal signal(*this);
    std::transform(signal.readingData.begin(), signal.readingData.end(), signal.readingData.begin(), [value](const double_t i) {return i - value; });
    return signal;
}

Signal Signal::operator*(double_t value) const
{
    Signal signal(*this);
    std::transform(signal.readingData.begin(), signal.readingData.end(), signal.readingData.begin(), [value](const double_t i) {return i * value; });
    return signal;
}

Signal& Signal::operator+=(double_t value)
{
    std::transform(this->readingData.begin(), this->readingData.end(), this->readingData.begin(), [value](const double_t i) {return i + value; });
    return *this;
}

Signal& Signal::operator-=(double_t value)
{
    std::transform(this->readingData.begin(), this->readingData.end(), this->readingData.begin(), [value](const double_t i) {return i - value; });
    return *this;
}

Signal& Signal::operator*=(double_t value)
{
    std::transform(this->readingData.begin(), this->readingData.end(), this->readingData.begin(), [value](const double_t i) {return i * value; });
    return *this;
}
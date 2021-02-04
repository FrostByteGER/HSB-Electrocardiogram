#pragma once
#include <string>
#include <vector>

class Signal
{
public:
    Signal() = default;

    void ReadFromCSV(const std::string& filename);
    void WriteToCSV(const std::string& filename);
    uint64_t GetCount() const;
    uint16_t GetMinimum() const;
    uint16_t GetMaximum() const;
    uint64_t GetSum() const;
    uint64_t GetAverage() const;
    uint64_t GetStandardDeviation() const;

    void Smooth();

    // TODO: Operator Overload

private:
    std::vector<uint16_t> _readings;
};

#include "SignalProcessor.hpp"
#include <algorithm>
#include "Signal.hpp"


Signal SignalProcessor::constructSignalFromReadings(const std::vector<double_t>& readings)
{
    return Signal(readings);
}

Signal SignalProcessor::smoothSignal(const Signal& signal, const int32_t windowSize) const
{
    const std::vector<double_t>& signalReadings = signal.readings();
    const int32_t totalWindowSize = windowSize * 2 + 1;
    std::vector<double_t> smoothedReadings;
    smoothedReadings.reserve(signalReadings.size());
    for (size_t idx = 0; idx < signalReadings.size(); ++idx)
    {
        double_t sum = 0;
        for (int32_t windowIdx = -windowSize; windowIdx < windowSize + 1; ++windowIdx)
        {
            const int32_t offsetIdx = static_cast<int32_t>(idx) + windowIdx;
            const int32_t currentIdx = std::clamp(offsetIdx, 0, static_cast<int32_t>(signalReadings.size()) - 1);
            sum += signalReadings[currentIdx];
        }
        const double_t average = sum / totalWindowSize;
        smoothedReadings.push_back(average);
    }
    return Signal(smoothedReadings);
}

std::vector<std::string> SignalProcessor::exportToCsvImpl(const Signal& obj) const
{
    std::vector<std::string> exportData;
    exportData.reserve(obj.readings().size());
    for (auto e : obj.readings())
    {
        exportData.push_back(std::to_string(e));
    }
    return exportData;
}

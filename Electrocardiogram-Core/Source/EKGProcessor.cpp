#include "EKGProcessor.hpp"


#include <iostream>

#include "EKG.hpp"

EKG EKGProcessor::constructEkgFromSignal(const Signal& signal, uint32_t samplingIntervalMs,
    int32_t signalRangeMilliVolt, int32_t signalRangeRawMax, int32_t heartbeatTailLength)
{
    const std::vector<double_t> mappedReadings = mapToMillivoltRange(signal.readings(), signalRangeMilliVolt, signalRangeRawMax);
    const std::vector<Heartbeat> heartbeats = detectHeartbeats(mappedReadings, samplingIntervalMs, heartbeatTailLength);
    EKG ecg = EKG(mappedReadings, heartbeats, samplingIntervalMs);
    return ecg;
}

std::vector<std::string> EKGProcessor::exportToCsvImpl(const EKG& obj) const
{
    std::vector<std::string> exportData;
    exportData.reserve(obj.readings().size());
    uint32_t time = 0;
    for (double_t e : obj.readings())
    {
        exportData.push_back(std::to_string(time) + ", " + std::to_string(e));
        time += obj.samplingIntervalMs();
    }
    return exportData;
}

std::vector<Heartbeat> EKGProcessor::detectHeartbeats(const std::vector<double_t>& readingsData, const uint32_t samplingIntervalMs, const int32_t tailLength) const
{
    // Calculate derivate of readingData first
    std::vector<double_t> derivatedReadings;
    derivatedReadings.reserve(readingsData.size());
    for (size_t idx = 0; idx < readingsData.size(); ++idx)
    {
        const size_t prevIdx = std::max(0ULL, idx - 1);
        const size_t nextIdx = std::min(readingsData.size() - 1, idx + 1);
        const double_t previous = readingsData[prevIdx];
        const double_t next = readingsData[nextIdx];

        const double_t result = (next - previous) / (2.0 * samplingIntervalMs);
        derivatedReadings.push_back(result);
    }

    const Signal derivatedSignal = Signal(derivatedReadings);

    const double_t stdDeviationTop = derivatedSignal.average() + derivatedSignal.standardDeviation();
    const double_t stdDeviationBottom = derivatedSignal.average() - derivatedSignal.standardDeviation();
    double_t maxValue = std::numeric_limits<double_t>::min();
    int64_t maxPosition = -tailLength - 1;
    double_t minValue = std::numeric_limits<double_t>::max();
    int64_t minPosition = -tailLength - 1;
    std::vector<Heartbeat> heartbeatPositions;

    const int64_t derivatedReadingsSize = static_cast<int64_t>(derivatedReadings.size());
    for (int64_t i = 0; i < derivatedReadingsSize; ++i)
    {
        const double_t current = derivatedReadings[i];

        if ((current <= minValue || i > minPosition + tailLength) && current <= stdDeviationBottom)
        {
            minValue = current;
            minPosition = i;
        }

        if (minPosition < maxPosition && maxPosition == i - tailLength)
        {
            heartbeatPositions.emplace_back((minPosition - 3) * samplingIntervalMs, (maxPosition + 3) * samplingIntervalMs);
            std::cout << "Heartbeat found at " << minPosition << "|" << maxPosition << std::endl;
        }

        if ((current >= maxValue || i > maxPosition + tailLength) && current >= stdDeviationTop)
        {
            maxValue = current;
            maxPosition = i;
        }
    }

    return heartbeatPositions;
}

std::vector<double_t> EKGProcessor::mapToMillivoltRange(const std::vector<double_t>& readingsData, const int32_t signalRangeMilliVolt, const int32_t signalRangeRawMax)
{
    std::vector<double_t> mappedReadings;
    mappedReadings.reserve(readingsData.size());
    const int32_t newRange = signalRangeMilliVolt - (-signalRangeMilliVolt);
    for (const double_t& i : readingsData)
    {
        mappedReadings.push_back((i * newRange) / signalRangeRawMax + (-signalRangeMilliVolt));
    }
    return mappedReadings;
}

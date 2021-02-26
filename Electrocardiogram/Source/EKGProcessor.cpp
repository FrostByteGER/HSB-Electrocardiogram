#include "EKGProcessor.hpp"

#include <sstream>
#include <iostream>

#include "EKG.hpp"

EKG EKGProcessor::constructEkgFromReadings(const std::vector<double_t>& readings, uint32_t samplingIntervalMs, int32_t heartbeatTailLength)
{
    const std::vector<Heartbeat> heartbeats = detectHeartbeats(readings, samplingIntervalMs, heartbeatTailLength);
    EKG ecg = EKG(readings, heartbeats, samplingIntervalMs);
    return ecg;
}

std::vector<std::string> EKGProcessor::exportToCsvImpl(const EKG& obj) const
{
    std::vector<std::string> exportData;
    exportData.reserve(obj.readings().size());
    uint32_t time = 0;
    std::stringstream stream;
    for (double_t e : obj.readings())
    {
        // Stringstream removes the fraction of the double by default if its 0.
        stream << time << "," << e;
        exportData.push_back(stream.str());
        time += obj.samplingIntervalMs();
        stream.str("");
        stream.clear();
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
        const int64_t prevIdx = std::max(0LL, static_cast<int64_t>(idx) - 1);
        const int64_t nextIdx = std::min(readingsData.size() - 1, idx + 1);
        const double_t previous = readingsData[prevIdx];
        const double_t next = readingsData[nextIdx];

        const double_t result = (next - previous) / (2.0 * samplingIntervalMs);
        derivatedReadings.push_back(result);
    }

    const Signal derivatedSignal = Signal(derivatedReadings);

    const double_t stdDeviationTop = derivatedSignal.average() + derivatedSignal.standardDeviation();
    const double_t stdDeviationBottom = derivatedSignal.average() - derivatedSignal.standardDeviation();
    double_t maxValue = std::numeric_limits<double_t>::min();
    int64_t maxPosition = static_cast<int64_t>(-tailLength) - 1;
    double_t minValue = std::numeric_limits<double_t>::max();
    int64_t minPosition = static_cast<int64_t>(-tailLength) - 1;
    std::vector<Heartbeat> heartbeatPositions;

    const auto derivatedReadingsSize = static_cast<int64_t>(derivatedReadings.size());
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

std::vector<double_t> EKGProcessor::mapToMillivoltRange(const EKG& ecg, const int32_t signalRangeMilliVolt, const int32_t signalRangeRawMax)
{
    std::vector<double_t> mappedReadings;
    mappedReadings.reserve(ecg.readings().size());
    const int32_t newRange = signalRangeMilliVolt - (-signalRangeMilliVolt);
    for (const double_t& i : ecg.readings())
    {
        mappedReadings.push_back((i * newRange) / signalRangeRawMax + (-signalRangeMilliVolt));
    }
    return mappedReadings;
}

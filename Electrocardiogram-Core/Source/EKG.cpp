#include "EKG.hpp"

#include <iostream>

EKG::EKG(std::vector<double_t> readingsData) : Signal(std::move(readingsData))
{
}

EKG::EKG(std::vector<double_t> readingsData, const uint32_t samplingIntervalMs) : Signal(std::move(readingsData)), samplingIntervalMs(samplingIntervalMs)
{
}

EKG::EKG(std::vector<double_t> readingsData, const uint32_t samplingIntervalMs, const int32_t signalRangeMilliVolt, const int32_t signalRangeRawMax) : Signal(std::move(readingsData)), samplingIntervalMs(samplingIntervalMs), signalRangeMillivolt(signalRangeMilliVolt), signalRangeRawMax(signalRangeRawMax)
{
}

uint32_t EKG::HeartbeatCount() const
{
    return 0;
}

double_t EKG::HeatbeatsPerMinute() const
{
    return 0.0;
}

uint32_t EKG::SamplingIntervalMs() const
{
    return samplingIntervalMs;
}

int32_t EKG::SignalRangeMilliVolt() const
{
    return signalRangeMillivolt;
}

int32_t EKG::SignalRangeMax() const
{
    return signalRangeRawMax;
}

std::vector<std::string> EKG::Export() const 
{
    std::vector<std::string> exportData;
    exportData.reserve(readings.size());
    uint32_t time = 0;
    for (double_t e : readings)
    {
        exportData.push_back(std::to_string(time) + ", " + std::to_string(e));
        time += samplingIntervalMs;
    }
    return exportData;
}

std::vector<HeartbeatFrame> EKG::DetectHeartbeats(std::vector<double_t>& readingsData, const int32_t tailLength)
{
    // Calculate derivate of readings first
    std::vector<double_t> derivatedReadings;
    derivatedReadings.reserve(readingsData.size());
    for (size_t idx = 0; idx < readingsData.size(); ++idx)
    {
        const int32_t prevIdx = std::max(0, static_cast<int32_t>(idx) - 1);
        const int32_t nextIdx = std::min(readingsData.size() - 1, idx + 1);
        const double_t previous = readingsData[prevIdx];
        const double_t next = readingsData[nextIdx];

        const double_t result = (next - previous) / (2.0 * samplingIntervalMs);
        derivatedReadings.push_back(result);
    }

    EKG derivatedEKG = EKG(derivatedReadings, samplingIntervalMs, signalRangeMillivolt, signalRangeRawMax);

    const double_t stdDeviationTop = derivatedEKG.Average() + derivatedEKG.StandardDeviation();
    const double_t stdDeviationBottom = derivatedEKG.Average() - derivatedEKG.StandardDeviation();
    double_t maxValue = std::numeric_limits<double_t>::min();
    int64_t maxPosition = -tailLength - 1;
    double_t minValue = std::numeric_limits<double_t>::max();
    int64_t minPosition = -tailLength - 1;
    std::vector<HeartbeatFrame> heartbeatPositions;

    for (int64_t i = 0; i < derivatedReadings.size(); ++i)
    {
        const double_t current = derivatedReadings[i];

        if((current <= minValue || i > minPosition + tailLength) && current <= stdDeviationBottom)
        {
            minValue = current;
            minPosition = i;
        }

        if (minPosition < maxPosition && maxPosition == i - tailLength)
        {
            heartbeatPositions.emplace_back((minPosition - 3) * samplingIntervalMs, (maxPosition + 3) * samplingIntervalMs);
            std::cout << "Heartbeat found at " << minPosition << "|" << maxPosition << std::endl;
        }

        if((current >= maxValue || i > maxPosition + tailLength) && current >= stdDeviationTop)
        {
            maxValue = current;
            maxPosition = i;
        }
    }

    return heartbeatPositions;
}

std::vector<double_t>& EKG::MapToMillivoltRange(std::vector<double_t>& readingsData)
{
    const int32_t newRange = signalRangeMillivolt - (-signalRangeMillivolt);
    for (double_t& i : readingsData)
    {
        i = (i * newRange) / signalRangeRawMax + (-signalRangeMillivolt);
    }
    return readingsData;
}

#include "EKG.hpp"

EKG::EKG(std::vector<uint16_t> readingsData) : Signal(std::move(readingsData))
{
    this->readings = MapToMillivoltRange(this->readings);
}

EKG::EKG(std::vector<uint16_t> readingsData, const uint32_t samplingIntervalMs) : Signal(std::move(readingsData)), samplingIntervalMs(samplingIntervalMs)
{
    this->readings = MapToMillivoltRange(this->readings);
}

EKG::EKG(std::vector<uint16_t> readingsData, const uint32_t samplingIntervalMs, const int32_t signalRangeMilliVolt, const uint16_t signalRangeRawMax) : Signal(std::move(readingsData)), samplingIntervalMs(samplingIntervalMs), signalRangeMillivolt(signalRangeMilliVolt), signalRangeRawMax(signalRangeRawMax)
{
    this->readings = MapToMillivoltRange(this->readings);
}

uint64_t EKG::HeartbeatCount() const
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

uint16_t EKG::SignalRangeMax() const
{
    return signalRangeRawMax;
}

std::vector<std::string> EKG::Export() const 
{
    std::vector<std::string> exportData;
    exportData.reserve(readings.size());
    uint64_t time = 0;
    for (uint16_t e : readings)
    {
        exportData.push_back(std::to_string(time) + ", " + std::to_string(e));
        time += samplingIntervalMs;
    }
    return exportData;
}

std::vector<uint16_t>& EKG::MapToMillivoltRange(std::vector<uint16_t>& readingsData)
{
    for (size_t i = 0; i < readingsData.size(); ++i)
    {
        /*
         * OldRange = (OldMax - OldMin)  
         * NewRange = (NewMax - NewMin)  
         * NewValue = (((OldValue - OldMin) * NewRange) / OldRange) + NewMin
         */
    }
    return readingsData;
}

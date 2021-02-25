#include "EKG.hpp"

#include <iostream>
#include <utility>

EKG::EKG(std::vector<double_t> readingsData, std::vector<Heartbeat> heartbeats, uint32_t samplingIntervalMs)
    : Signal(std::move(readingsData)), heartbeatData(std::move(heartbeats)), samplingIntervalMilliseconds(samplingIntervalMs)
{
}

const std::vector<Heartbeat>& EKG::heartbeats() const
{
    return heartbeatData;
}

size_t EKG::heartbeatCount() const
{
    return heartbeatData.size();
}

double_t EKG::heartbeatsPerMinute() const
{
    return 0.0;
}

uint32_t EKG::samplingIntervalMs() const
{
    return samplingIntervalMilliseconds;
}
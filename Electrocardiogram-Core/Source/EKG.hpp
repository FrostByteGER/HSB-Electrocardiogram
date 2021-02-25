#pragma once
#include "Heartbeat.hpp"
#include "Signal.hpp"

class EKG : public Signal
{
public:
    EKG(std::vector<double_t> readingsData, std::vector<Heartbeat> heartbeats,  uint32_t samplingIntervalMs);
    ~EKG() override = default;

    [[nodiscard]] const std::vector<Heartbeat>& heartbeats() const;
    [[nodiscard]] size_t heartbeatCount() const;
    [[nodiscard]] double_t heartbeatsPerMinute() const;
    [[nodiscard]] uint32_t samplingIntervalMs() const;

private:
    std::vector<Heartbeat> heartbeatData;
    uint32_t samplingIntervalMilliseconds = 5;
};

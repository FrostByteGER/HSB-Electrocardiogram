#pragma once
#include "Signal.hpp"

class EKG : public Signal
{
public:
    EKG(std::vector<uint16_t> readings, const uint32_t samplingIntervalMs) : Signal(std::move(readings)), _samplingIntervalMs(samplingIntervalMs) { }
    ~EKG() override = default;

    uint64_t HeartbeatCount() const;
    double_t HeatbeatsPerMinute() const;

    
    std::vector<std::string> Export() const override;
private:
    uint32_t _samplingIntervalMs = 5;
};

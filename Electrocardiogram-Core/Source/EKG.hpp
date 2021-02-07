#pragma once
#include "Signal.hpp"

class EKG : public Signal
{
public:
    explicit EKG(std::vector<uint16_t> readings) : Signal(std::move(readings)) { }
    EKG(std::vector<uint16_t> readings, const uint32_t samplingIntervalMs) : Signal(std::move(readings)), samplingIntervalMs(samplingIntervalMs) { }
    ~EKG() override = default;

    uint64_t HeartbeatCount() const;
    double_t HeatbeatsPerMinute() const;


    uint32_t SamplingIntervalMs() const;

    std::vector<std::string> Export() const override;
private:
    uint32_t samplingIntervalMs = 5;
};

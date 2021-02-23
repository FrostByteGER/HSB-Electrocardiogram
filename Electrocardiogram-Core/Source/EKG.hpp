#pragma once
#include "Signal.hpp"

class EKG : public Signal
{
public:
    explicit EKG(std::vector<uint16_t> readingsData);
    EKG(std::vector<uint16_t> readingsData, uint32_t samplingIntervalMs);
    EKG(std::vector<uint16_t> readingsData, uint32_t samplingIntervalMs, int32_t signalRangeMilliVolt, uint16_t signalRangeRawMax);
    ~EKG() override = default;

    [[nodiscard]] uint64_t HeartbeatCount() const;
    [[nodiscard]] double_t HeatbeatsPerMinute() const;
    [[nodiscard]] uint32_t SamplingIntervalMs() const;
    [[nodiscard]] int32_t SignalRangeMilliVolt() const;
    [[nodiscard]] uint16_t SignalRangeMax() const;
    [[nodiscard]] std::vector<std::string> Export() const override;

private:
    uint32_t samplingIntervalMs = 5;
    int32_t signalRangeMillivolt = 4;
    uint16_t signalRangeRawMax = std::numeric_limits<uint16_t>::max();

    std::vector<uint16_t>& MapToMillivoltRange(std::vector<uint16_t>& readingsData);
};

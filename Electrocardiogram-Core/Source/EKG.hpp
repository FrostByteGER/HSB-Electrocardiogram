#pragma once
#include "Signal.hpp"


struct HeartbeatFrame
{
    HeartbeatFrame(const size_t frameStartMs, const size_t frameEndMs) : frameStartMs(frameStartMs), frameEndMs(frameEndMs)
    {
    }

    size_t frameStartMs;
    size_t frameEndMs;
};

class EKG : public Signal
{
public:
    explicit EKG(std::vector<double_t> readingsData);
    EKG(std::vector<double_t> readingsData, uint32_t samplingIntervalMs);
    EKG(std::vector<double_t> readingsData, uint32_t samplingIntervalMs, int32_t signalRangeMilliVolt, int32_t signalRangeRawMax);
    ~EKG() override = default;

    [[nodiscard]] uint32_t HeartbeatCount() const;
    [[nodiscard]] double_t HeatbeatsPerMinute() const;
    [[nodiscard]] uint32_t SamplingIntervalMs() const;
    [[nodiscard]] int32_t SignalRangeMilliVolt() const;
    [[nodiscard]] int32_t SignalRangeMax() const;
    [[nodiscard]] std::vector<std::string> Export() const override;
    std::vector<HeartbeatFrame> DetectHeartbeats(std::vector<double_t>& readingsData, int32_t tailLength);

private:
    uint32_t samplingIntervalMs = 5;
    int32_t signalRangeMillivolt = 4;
    int32_t signalRangeRawMax = std::numeric_limits<int32_t>::max();
    
    std::vector<double_t>& MapToMillivoltRange(std::vector<double_t>& readingsData);
};

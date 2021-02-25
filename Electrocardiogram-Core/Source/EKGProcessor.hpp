#pragma once
#include <iosfwd>
#include <vector>


#include "AbstractProcessor.hpp"
#include "Heartbeat.hpp"

class Signal;
class EKG;

class EKGProcessor : public AbstractProcessor<EKGProcessor, EKG>
{
    friend AbstractProcessor;

public:
    [[nodiscard]] EKG constructEkgFromReadings(const std::vector<double_t>& rawReadings, const std::vector<double_t>& smoothedReadings, uint32_t samplingIntervalMs,
        int32_t signalRangeMilliVolt, int32_t signalRangeRawMax, int32_t heartbeatTailLength);

protected:
    [[nodiscard]] std::vector<std::string> exportToCsvImpl(const EKG& obj) const override;

private:
    std::vector<Heartbeat> detectHeartbeats(const std::vector<double_t>& readingsData, const uint32_t samplingIntervalMs, const int32_t tailLength) const;
    std::vector<double_t> mapToMillivoltRange(const std::vector<double_t>& readingsData, const int32_t signalRangeMilliVolt, const int32_t signalRangeRawMax);
};

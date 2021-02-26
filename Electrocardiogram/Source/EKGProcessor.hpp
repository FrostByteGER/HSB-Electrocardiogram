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
    [[nodiscard]] EKG constructEkgFromReadings(const std::vector<double_t>& readings, uint32_t samplingIntervalMs, int32_t heartbeatTailLength);
    std::vector<double_t> mapToMillivoltRange(const EKG& ecg, int32_t signalRangeMilliVolt, int32_t signalRangeRawMax);

protected:
    [[nodiscard]] std::vector<std::string> exportToCsvImpl(const EKG& obj) const override;

private:
    std::vector<Heartbeat> detectHeartbeats(const std::vector<double_t>& readingsData, uint32_t samplingIntervalMs, int32_t tailLength) const;
};

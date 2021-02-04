#pragma once
#include "Signal.hpp"

class EKG : public Signal
{
public:
    void ExportToCSV();
    uint32_t GetHeartbeatCount() const;
    uint32_t GetHeatbeatsPerMinute() const;

private:
    uint32_t _samplingIntervalMs = 5;
};

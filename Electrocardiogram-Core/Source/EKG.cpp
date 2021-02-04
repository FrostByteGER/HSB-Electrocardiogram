#include "EKG.hpp"

uint64_t EKG::HeartbeatCount() const
{
    return 0;
}

double_t EKG::HeatbeatsPerMinute() const
{
    return 0.0;
}

std::vector<std::string> EKG::Export() const
{
    //TODO: Add milliseconds
    std::vector<std::string> exportData(_readings.size());
    for (uint16_t e : _readings)
    {
        exportData.push_back(std::to_string(e));
    }
    return exportData;
}

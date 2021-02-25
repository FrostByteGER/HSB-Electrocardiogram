#pragma once
#include "AbstractProcessor.hpp"

class Signal;

class SignalProcessor : public AbstractProcessor<SignalProcessor, Signal>
{
    friend AbstractProcessor;

public:
    [[nodiscard]] Signal constructSignalFromReadings(const std::vector<double_t>& readings);
    [[nodiscard]] Signal smoothSignal(const Signal& signal, int32_t windowSize) const;

protected:
    [[nodiscard]] std::vector<std::string> exportToCsvImpl(const Signal& obj) const override;
};

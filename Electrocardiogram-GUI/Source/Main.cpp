#include <iostream>

#include "ConfigurationCache.hpp"
#include "ElectrocardiogramGUI.hpp"
#include "FileManager.hpp"
#include "Signal.hpp"
#include <QApplication>

void parseCommandlineArguments(int argc, char* argv[])
{
    for (int i = 1; i < argc; ++i)
    {
        std::string argument = argv[i];
        // We completely ignore non-prefixed arguments here!
        if (argument.rfind("--", 0) == 0 || argument.rfind('-', 0) == 0)
        {
            // Check if we have a value for the current argument, if so insert that one too, otherwise simply add the current argument
            // with an empty string as its value
            if (i + 1 < argc)
            {
                std::string nextArgument = argv[i + 1];
                if (nextArgument.rfind("--", 0) == std::string::npos && nextArgument.rfind('-', 0) == std::string::npos)
                {
                    ConfigurationCache::instance().addConfigValue(argument, nextArgument);
                    continue;
                }
            }
            ConfigurationCache::instance().addConfigValue(argument, std::string());
        }
    }
}

int main(int argc, char* argv[])
{
    try
    {

        parseCommandlineArguments(argc, argv);
        if(ConfigurationCache::instance().keyExists("--nogui"))
        {
            std::cout << "Elecrocardiogram!\n";
            const FileManager fileManager;
            SignalProcessor signalProcessor;
            EKGProcessor ecgProcessor;
            std::string inFilePath;
            if (!ConfigurationCache::instance().tryGetValue("--input-file", &inFilePath))
            {
                throw std::exception("File parameter expected, none was found!");
            }
            std::vector<double_t> rawReadings = fileManager.Import(inFilePath);
            std::cout << "Imported ECG signal from file " << inFilePath << std::endl;

            // Load and smooth signal
            const Signal signal = signalProcessor.constructSignalFromReadings(rawReadings);
            int32_t windowSize = 21;
            ConfigurationCache::instance().tryGetValue("--smoothing-window-size", &windowSize);
            std::cout << "Smoothing signal with window-size " << windowSize << std::endl;
            const Signal smoothedSignal = signalProcessor.smoothSignal(signal, windowSize);

            // Create ECG from Signal
            uint32_t samplingIntervalMs = 5;
            ConfigurationCache::instance().tryGetValue("--sampling-interval", &samplingIntervalMs);
            int32_t signalRangeMilliVolt = 4;
            ConfigurationCache::instance().tryGetValue("--signalrange-millivolts", &signalRangeMilliVolt);
            int32_t signalRangeRawMax = std::numeric_limits<uint16_t>::max();
            ConfigurationCache::instance().tryGetValue("--signalrange-max", &signalRangeRawMax);
            int32_t tailLength = 25;
            ConfigurationCache::instance().tryGetValue("--heartbeat-tail-length", &tailLength);
            const EKG ecg = ecgProcessor.constructEkgFromReadings(rawReadings, smoothedSignal.readings(), samplingIntervalMs, signalRangeMilliVolt, signalRangeRawMax, tailLength);

            std::cout << "ECG Statistics: " << "\n"
                << "Count: " << ecg.count() << "\n"
                << "Min: " << ecg.minimum() << "\n"
                << "Max: " << ecg.maximum() << "\n"
                << "Sum: " << ecg.sum() << "\n"
                << "Average: " << ecg.average() << "\n"
                << "Standard Deviation: " << ecg.standardDeviation() << "\n"
                << "Sampling Interval in Milliseconds: " << ecg.samplingIntervalMs() << "\n"
                << "Heartbeats: " << ecg.heartbeatCount() << "\n"
                << "Heartbeats per Minute: " << ecg.heartbeatsPerMinute()
                << std::endl;
            
            std::string outFilePath;
            if(ConfigurationCache::instance().tryGetValue("--output-file", &outFilePath))
            {
                fileManager.Export(ecgProcessor.exportToCsv(ecg), outFilePath);
                std::cout << "Exported ECG signal to file " << outFilePath << std::endl;
            }
            
        }else
        {
            QApplication a(argc, argv);
            ElectrocardiogramGUI w;
            w.show();
            return QApplication::exec();
        }

    }catch(std::exception& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
        std::getchar();
        return -1;
    }catch(...)
    {
        std::cout << "An unknown error occured! Exception name: " << typeid(std::current_exception()).name() << std::endl;
        std::getchar();
        return -1;
    }
    std::getchar();
    return 0;
}

#include <iostream>


#include "ConfigurationCache.hpp"
#include "FileManager.hpp"
#include "Signal.hpp"

int main(int argc, char* argv[])
{
    try
    {
        std::cout << "Elecrocardiogram!\n";
        std::vector<double_t> data = { 600, 470, 170, 430, 300 };
        const FileManager fileManager;
        for (int i = 1; i < argc; ++i)
        {
            std::string argument = argv[i];
            // We completely ignore non-prefixed arguments here!
            if(argument.rfind("--", 0) == 0 || argument.rfind('-', 0) == 0)
            {
                // Check if we have a value for the current argument, if so insert that one too, otherwise simply add the current argument
                // with an empty string as its value
                if (i + 1 < argc)
                {
                    std::string nextArgument = argv[i + 1];
                    if(nextArgument.rfind("--", 0) == std::string::npos && nextArgument.rfind('-', 0) == std::string::npos)
                    {
                        ConfigurationCache::instance().addConfigValue(argument, nextArgument);
                        continue;
                    }
                }
                ConfigurationCache::instance().addConfigValue(argument, std::string());
            }
        }
        std::string filePath;
        if (ConfigurationCache::instance().tryGetValue("-f", &filePath))
        {
            data = fileManager.Import(filePath);
        }

        if(ConfigurationCache::instance().tryGetValue("-nogui"))
        {
            //TODO: Merge GUI project
        }else
        {
            const Signal signal(data);
            std::cout << "Count: " << signal.Count() << "\n"
                << "Min: " << signal.Minimum() << "\n"
                << "Max: " << signal.Maximum() << "\n"
                << "Sum: " << signal.Sum() << "\n"
                << "Average: " << signal.Average() << "\n"
                << "Standard Deviation: " << signal.StandardDeviation() << std::endl;
            Signal signal2 = signal + 1;
            signal2 = signal - 1;
            signal2 = signal * 2;
            signal2 += 2;
            signal2 -= 2;
            signal2 *= 2;
            std::cout << "";
        }

    }catch(std::exception& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
        return -1;
    }catch(...)
    {
        std::cout << "An unknown error occured! Exception name: " << typeid(std::current_exception()).name() << std::endl;
        return -1;
    }
    return 0;
}

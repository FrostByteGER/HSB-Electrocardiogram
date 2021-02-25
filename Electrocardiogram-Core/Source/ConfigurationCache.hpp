#pragma once
#include <map>
#include <string>
#include <sstream>

/**
 * This class primarily acts as a cache for command-line parameter options.
 */
class ConfigurationCache
{
public:
    static ConfigurationCache& instance()
    {
        static ConfigurationCache instance;
        return instance;
    }

    bool addConfigValue(const std::string& key, const std::string& value);
    bool keyExists(const std::string& key);
    template<typename T>
    bool tryGetValue(const std::string& key, T* outValue);

private:
    ConfigurationCache() = default;
    ConfigurationCache(const ConfigurationCache&) = default;
    ConfigurationCache& operator=(const ConfigurationCache&) = delete; // Don't create a definition as we don't want copy construction

    std::map<std::string, std::string> configValues;
};

template <typename T>
bool ConfigurationCache::tryGetValue(const std::string& key, T* outValue)
{
    const auto iter = configValues.find(key);
    if (iter != configValues.end())
    {
        std::stringstream stream(iter->second);
        if (outValue != nullptr)
        {
            // Prevent overwrite of outValue if the stream fails for some reason.
            T temp = *outValue;
            stream >> temp;
            if (stream.fail())
            {
                throw std::exception(("Failed to cast value of key " + key + " to type " + std::string(typeid(outValue).name())).c_str());
            }
            *outValue = temp;
        }
        return true;
    }
    return false;
}

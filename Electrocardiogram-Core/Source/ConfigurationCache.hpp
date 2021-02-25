#pragma once
#include <map>
#include <string>

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
    bool tryGetValue(const std::string& key, std::string* outValue = nullptr);

private:
    ConfigurationCache() = default;
    ConfigurationCache(const ConfigurationCache&) = default;
    ConfigurationCache& operator=(const ConfigurationCache&) = delete; // Don't create a definition as we don't want copy construction

    std::map<std::string, std::string> configValues;
};

#include "ConfigurationCache.hpp"

bool ConfigurationCache::addConfigValue(const std::string& key, const std::string& value)
{
    const auto iter = configValues.find(key);
    if (iter != configValues.end())
    {
        return false;
    }
    configValues[key] = value;
    return true;
}

bool ConfigurationCache::tryGetValue(const std::string& key, std::string* outValue)
{
    const auto iter = configValues.find(key);
    if(iter != configValues.end())
    {
        if(outValue != nullptr)
        {
            *outValue = iter->second;
        }
        return true;
    }
    return false;
}

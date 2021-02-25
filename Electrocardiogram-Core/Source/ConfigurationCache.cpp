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

bool ConfigurationCache::keyExists(const std::string& key)
{
    return configValues.find(key) != configValues.end();
}

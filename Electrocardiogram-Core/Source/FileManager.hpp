#pragma once
#include <string>
#include <fstream>
#include "Signal.hpp"

//TODO: Move to cpp
// Why its own class? So we dont have to pass filename as constructor arg to Signal or use a static factory
class FileManager
{
public:
    std::vector<uint16_t> Import(const std::string& filename) const
    {
        std::vector<uint16_t> data;
        std::ifstream inputStream(filename);
        if(!inputStream)
        {
            throw std::ios_base::failure("Failed to open " + filename);
        }
        std::string line;
        while (std::getline(inputStream, line))
        {
            const unsigned long rawValue = std::stoul(line);
            // Value may be beyond (2^16)-1
            constexpr uint16_t maxValue = std::numeric_limits<uint16_t>::max();
            if(rawValue > maxValue)
            {
                throw std::overflow_error(std::to_string(rawValue) + " exceeds " + std::to_string(maxValue));
            }
            data.push_back(static_cast<uint16_t>(rawValue));
        }
        return data;
    }

    //TODO: Test
    void Export(const std::vector<std::string>& data, const std::string& filename)
    {
        std::ofstream outputStream(filename);
        if(!outputStream)
        {
            throw std::ios_base::failure("Failed to open " + filename);
        }
        for (const std::string& e : data)
        {
            // Its okay to flush after every line.
            outputStream << e << std::endl;
        }
        outputStream.close();
    }
};

#pragma once
#include <string>
#include <fstream>
#include <vector>

//TODO: Move to cpp
// Why its own class? So we dont have to pass filename as constructor arg to Signal or use a static factory
class FileManager
{
public:
    [[nodiscard]] std::vector<double_t> Import(const std::string& filename) const
    {
        std::vector<double_t> data;
        std::ifstream inputStream(filename);
        if(!inputStream)
        {
            throw std::ios_base::failure("Failed to open " + filename);
        }
        std::string line;
        while (std::getline(inputStream, line))
        {
            const double_t rawValue = std::stod(line);
            data.push_back(rawValue);
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

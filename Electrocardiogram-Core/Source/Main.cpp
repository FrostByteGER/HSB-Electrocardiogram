#include <iostream>

#include "FileManager.hpp"
#include "Signal.hpp"

int main(int argc, char* argv[])
{
    try
    {
        std::cout << "Elecrocardiogram!\n";
        std::vector<double_t> data = { 600, 470, 170, 430, 300 };
        const FileManager fileManager;
        bool useGui = false;
        for (size_t i = 1; i < argc; ++i)
        {
            std::string argument(argv[i]);
            if (argument == "-f")
            {
                data = fileManager.Import(argv[i + 1]);
            }else if(argument == "-nogui")
            {
                useGui = false;
            }
        }
        if(useGui)
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

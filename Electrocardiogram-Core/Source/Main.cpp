#include <iostream>

#include "FileManager.hpp"
#include "Signal.hpp"

int main()
{
    try
    {
        std::cout << "Elecrocardiogram!\n";
        const FileManager fileManager;
        const std::vector<uint16_t> data = fileManager.Import("Data/ecg.dat");  //{ 600, 470, 170, 430, 300 };
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

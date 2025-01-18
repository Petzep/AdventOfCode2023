// Day1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <numeric>
#include <ranges>

int main()
{
    // Load file
    std::ifstream file("input.txt");
    //std::ifstream file("example.txt");
    std::string line;
    std::vector<int> numbers;

    // Read file
    while(std::getline(file, line))
    {
        int first_nr = 0;
        int second_nr = 0;


        constexpr auto i1 = ranges::find_last_if(line.begin(), line.end(), pred, abs);

        try
        {
            auto first_nr_pos = line.find_first_of("0123456789");
            first_nr = std::stoi(line.substr(first_nr_pos, 1));

            auto last_nr_pos = line.find_last_of("0123456789");
            second_nr = std::stoi(line.substr(last_nr_pos, 1));
        }
        catch(const std::exception& ex)
        {
            std::cerr << "Error parsing line: " << line << std::endl;
            std::cerr << ex.what() << std::endl;
        }

        numbers.push_back(first_nr * 10 + second_nr);
    }

    auto sum = std::accumulate(numbers.begin(), numbers.end(), 0);
    std::cout << "Sum: " << sum << std::endl;
}

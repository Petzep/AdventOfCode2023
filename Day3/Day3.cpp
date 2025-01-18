// Day3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <ranges>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

// Number info
struct NumberInfo {
    // Create a number info object and set the counts to false
    // Assuming all numbers do not count initially
    NumberInfo(int number, size_t start, size_t end)
          : number(number), start(start), end(end), counts(false) { }

    int    number; // The value of the number
    size_t start;  // The start position of the number
    size_t end;    // The end position of the number
    bool   counts; // If the number counts or not
};

// Globals because we are lazy
std::vector<std::string>             lines;        // The array of lines
std::vector<std::vector<NumberInfo>> numberVector; // The vector of number positions

// Bug detection
void printLines(bool exclude) {
    // Print the lines but excluded the counted numbers
    // Go through all lines and check for a symbol
    for(size_t y = 0; y < lines.size(); y++)
    {
        // Go through all the characters in the line
        for(size_t x = 0; x < lines[y].size(); x++)
        {
            // If the char is not a digit, print it
            if(!std::isdigit(lines[y][x]))
            {
                std::cout << lines[y][x];
                continue;
            }

            // Otherwise, check if the number counts
            for(const auto& numberInfo : numberVector[y])
            {
                // Check if the symbol is in the range of the number
                if(x >= numberInfo.start && x <= numberInfo.end)
                {
                    // If the number counts
                    auto number_size = numberInfo.end - numberInfo.start;

                    // Detect big numbers, something is wrong
                    if(number_size > 3)
                    {
                        // Note to self:, found a number with an end that is 18446744073709551615
                        // std::cout << "Number size is bigger than 1, something is wrong!";
                    }

                    // Another bug, found, somewhere around number 321 during inclusion
                    if(numberInfo.number == 321 && !exclude)
                    {
                        // Don't print, but put a breakpoint here
                        // std::cout << "Found number 321, something is wrong around this!";
                    }

                    // Be sure to increase the x position -1, because the loop will increase it by 1
                    x += number_size - 1;

                    // if exclude is true, we want to print numbers that count
                    // or
                    // If exclude is false, we want to print numbers that don't count
                    if(numberInfo.counts && exclude
                       || (!numberInfo.counts && !exclude))
                    {
                        // Print a dot instead
                        for(int i = 0; i < number_size; i++)
                        {
                            std::cout << ".";
                        }
                    }
                    else
                    {
                        // If we print the number
                        std::cout << numberInfo.number;
                    }

                    // Break this inner loop
                    // We found the number, no need to check the rest
                    break;
                }
            }
        }

        // Print a new line and the end of the line
        std::cout << std::endl;
    }
}

int main() {
    // Load file
    //std::ifstream file("input.txt");
    //std::ifstream file("example.txt");
    std::ifstream file("bugggedInput.txt");
    std::string   line;

    // Read file and parse the info
    while(std::getline(file, line))
    {
        // store the lines in a vector
        lines.push_back(line);

        // Create a number info vector that stores all number info's of that line
        auto numberPositions = std::vector<NumberInfo>();

        // Save the position of every number in the line
        size_t offset = 0;
        while(offset < line.size())
        {
            auto numberStart = line.find_first_of("0123456789", offset);
            if(numberStart == std::string::npos)
            {
                break;
            }
            auto numberEnd = line.find_first_not_of("0123456789", numberStart);
            // To prevent numbers at the edge, we need to bound the number end
            numberEnd         = std::min(numberEnd, line.size()); // The end of the number
            auto numberCount  = numberEnd - numberStart;
            auto numberString = line.substr(numberStart, numberCount);
            int  number       = std::stoi(numberString);

            // Create the number info
            auto numberInfo = NumberInfo(number, numberStart, numberEnd);
            numberPositions.push_back(numberInfo);
            offset = numberEnd;
        }

        // Add the numberPositions to the numberVector array
        numberVector.push_back(numberPositions);
    }

    // Go through all lines and check for a symbol
    for(size_t y = 0; y < lines.size(); y++)
    {
        // Go through all the characters in the line
        for(size_t x = 0; x < lines[y].size(); x++)
        {
            auto ch = lines[y][x];
            // The symbol can not be a digit
            if(std::isdigit(ch))
            {
                continue;
            }

            // The . does not count as a symbol
            if(ch == '.')
            {
                continue;
            }

            // What is left must be a symbol
            // One line above, the current line and one line below
            size_t y_start = std::max(y - 1, size_t{0}); //(size_t{0} is the same as (size_t)(0), but we need to specify the type, as sd::max only wants to compare the same types)
            size_t y_end   = std::min(y + 1, lines.size() - 1);

            // Search for all numbers in the vicinity of the symbol
            for(size_t y_n = y_start; y_n <= y_end; y_n++)
            {
                // one character to the left, the current character and one character to the right
                size_t x_start = std::max(x - 1, size_t{0});
                size_t x_end   = std::min(x + 1, lines[y_n].size() - 1);

                for(int x_n = x_start; x_n <= x_end; x_n++)
                {
                    // Go through all the numbers in the line
                    // Be sure to addrees the numberInfo by reference, so we can change the counts value
                    for(auto& numberInfo : numberVector[y_n])
                    {
                        // Check if the symbol is in the range of the number
                        if(x_n >= numberInfo.start && x_n <= numberInfo.end)
                        {
                            numberInfo.counts = true;
                        }
                    }
                }
            }
        }
    }

    // All set and done, now num the number that count
    int sum = 0;
    for(const auto& numberPositions : numberVector)
    {
        for(const auto& numberInfo : numberPositions)
        {
            if(numberInfo.counts)
            {
                sum += numberInfo.number;
            }
        }
    }

    // Print the sum
    std::cout << "\nSum: " << sum << std::endl;

    // Found some bugs, print more info! :-D
    std::cout << "Included:\n--------------------------" << std::endl;
    printLines(true);
    std::cout << "Excluded:\n--------------------------" << std::endl;
    printLines(false);

    return 0;
}
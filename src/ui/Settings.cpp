#include "Settings.hpp"
#include <fstream>

void Settings::load()
{
    std::ifstream inputFile("settings.dat");
    if (inputFile.fail())
    {
        soundVol = 5;
    }
    else
    {
        inputFile >> soundVol;
    }
}

void Settings::save()
{
    std::ofstream outputFile("settings.dat");
    if (outputFile.fail())
    {
        return;
    }
    outputFile << soundVol;
}

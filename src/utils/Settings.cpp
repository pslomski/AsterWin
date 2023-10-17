#include "Settings.hpp"
#include <fstream>

void Settings::load()
{
    std::ifstream inputFile("settings.dat");
    if (inputFile.fail())
    {
        musicVol = 5;
        soundVol = 5;
    }
    else
    {
        inputFile >> musicVol >> soundVol;
    }
}

void Settings::save()
{
    std::ofstream outputFile("settings.dat");
    if (outputFile.fail())
    {
        return;
    }
    outputFile << musicVol << " " << soundVol;
}

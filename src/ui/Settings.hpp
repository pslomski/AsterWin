#pragma once

class Settings
{
public:
    void load();
    void save();

    int musicVol{5}; // Range: 0-10
    int soundVol{5}; // Range: 0-10
};

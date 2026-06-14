#pragma once

class Settings
{
public:
    void load();
    void save();

    int soundVol{5}; // Range: 0-10
};

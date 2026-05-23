#pragma once
#include <string>

enum class EItemType 
{
    Potion,
    Weapon,
    Armor,
    Junk
};

struct FItem 
{
    std::string Name;
    EItemType Type;
    int StatValue; 
    int Price;
    std::string Description;
};